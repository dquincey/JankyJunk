#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <raylib.h>
#include <string>

const int WIDTH = 32;
const int HEIGHT = 32;
const float SIZE = 5.0f;

Camera3D camera = {};
namespace Snake {
enum block_type {
    empty,
    body,
    food,
};
struct int_vector {
    int x;
    int y;
    int z;

    int_vector() = default;
    int_vector(int x, int y, int z) : x(x), y(y), z(z) {}
    int_vector(const Vector3 &new_pos) {
        x = static_cast<int>(new_pos.x);
        y = static_cast<int>(new_pos.y);
        z = static_cast<int>(new_pos.z);
    }
    int_vector operator+(const int_vector &other) const { return {other.x + x, other.y + y, other.z + z}; }
    int_vector operator+(const int &other) const { return {other + x, other + y, other + z}; }
    int_vector operator-(const int_vector &other) const { return {x - other.x, y - other.y, z - other.z}; }
    int_vector operator*(const int_vector &other) const { return {other.x * x, other.y * y, other.z * z}; }
    int_vector operator*(const int &other) const { return {other * x, other * y, other * z}; }
    int_vector &operator+=(const int_vector &other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
    int_vector operator%(const int &divisor) const { return {x % divisor, y % divisor, z % divisor}; }
    bool operator!=(const int_vector &other) {
        bool return_bool = false;
        if (other.x != x || other.y != y || other.z != z) {
            return_bool = true;
        }
        return return_bool;
    }
    bool operator==(const int_vector &other) {
        bool return_bool = false;
        if (other.x == x && other.y == y && other.z == z) {
            return_bool = true;
        }
        return return_bool;
    }
    friend std::ostream &operator<<(std::ostream &stream, const int_vector &other) {
        return stream << other.x << ", " << other.y << "," << other.z << std::endl;
    }
    int_vector abs() { return int_vector(std::abs(x), std::abs(y), std::abs(z)); }
};
struct vec2 {
    int x;
    int y;
};
namespace direction {
vec2 up = {0, 1};
vec2 down = {0, -1};
vec2 left = {-1, 0};
vec2 right = {1, 0};
vec2 Pup = {1, 0};
vec2 Pdown = {0, 2};
vec2 Pleft = {2, 1};
vec2 Pright = {0, 1};
} // namespace direction
const int GRID_Z = 5;
const int GRID_X = 5;
const int GRID_Y = 5;

std::array<std::array<std::array<int, GRID_Z>, GRID_Y>, GRID_X> grid;

void set(int_vector coords, block_type value) {
    // Checks whether it's within the array
    bool within = true;
    if (coords.x >= GRID_X || coords.x < 0) {
        within = false;
        std::cout << "Wasn't within X\n";
    }
    if (coords.y >= GRID_Y || coords.y < 0) {
        within = false;
        std::cout << "Wasn't within y\n";
    }
    if (coords.z >= GRID_Z || coords.z < 0) {
        within = false;
        std::cout << "Wasn't within z\n";
    }
    if (within) {
        grid[coords.z][coords.y][coords.x] = value;
    }
}
void render_grid() {
    for (int z = 0; z < static_cast<int>(grid.size()); z++) {
        for (int y = 0; y < static_cast<int>(grid[z].size()); y++) {
            for (int x = 0; x < static_cast<int>(grid[z][y].size()); x++) {
                if (grid[z][y][x] != empty) {
                    DrawCube({x * SIZE, y * SIZE, z * SIZE}, SIZE, SIZE, SIZE, WHITE);
                    DrawCubeWires({x * SIZE, y * SIZE, z * SIZE}, SIZE, SIZE, SIZE, BLACK);
                }
            }
        }
    }
}
void clear_body() {
    for (std::array<std::array<int, GRID_X>, GRID_Y> &z : grid) {
        for (std::array<int, GRID_X> &y : z) {
            for (int &x : y) {
                if (x == body) {
                    x = empty;
                }
            }
        }
    }
}
int_vector rotate_vector(int_vector vector, int_vector rotation_change) {
    for (int i = 0; i < std::abs(rotation_change.x); i++) {
        int rotation_change_direction = std::abs(rotation_change.x) / rotation_change.x;
        std::swap(vector.y, vector.z);
        if (rotation_change_direction == 1) {
            vector.z *= -1;
        } else if (rotation_change_direction == -1) {
            vector.y *= -1;
        }
    }
    for (int i = 0; i < std::abs(rotation_change.y); i++) {
        int rotation_change_direction = std::abs(rotation_change.y) / rotation_change.y;
        std::swap(vector.x, vector.z);
        if (rotation_change_direction == 1) {
            vector.z *= -1;
        } else if (rotation_change_direction == -1) {
            vector.x *= -1;
        }
    }
    for (int i = 0; i < std::abs(rotation_change.z); i++) {
        int rotation_change_direction = std::abs(rotation_change.z) / rotation_change.z;
        std::swap(vector.x, vector.y);
        if (rotation_change_direction == 1) {
            vector.x *= -1;
        } else if (rotation_change_direction == -1) {
            vector.y *= -1;
        }
    }
    return vector;
}
namespace camera {

int_vector camera_facing(float pitch, float yaw) {
    int_vector return_vector = {0, 0, 0};

    std::array<int, 4> yz = {1, 0, -1, 0};
    std::array<int, 4> yy = {-1, 0, 1};
    std::array<int, 4> yx = {0, 1, 0, -1};

    int return_vector_y = std::floor((RAD2DEG * pitch + 90.0f) / 60.0f) - 1;

    int yaw_rounded = static_cast<int>((std::fmod(std::round(RAD2DEG * yaw / 90.0f), 4.0f)) + 4.0f) % 4;
    return {yx[yaw_rounded], return_vector_y, yz[yaw_rounded]};
    // return return_vector;
}
} // namespace camera
} // namespace Snake

class head {
  private:
    Snake::int_vector rotation;
    Snake::int_vector facing_amount = {0, 0, 0};
    Snake::int_vector default_facing;

    void facing_change(Snake::int_vector rotation_change) {
        std::array<int, 4> positives{0, 1, 0, -1};
        facing_amount += rotation_change;
        facing_amount = ((facing_amount % 4) + Snake::int_vector{4, 4, 4}) % 4;
        facing = {positives[facing_amount.x], positives[facing_amount.y], positives[facing_amount.z]};
    }
    void trial_turn(Snake::int_vector vector, Snake::int_vector rotation) {
        Snake::int_vector cam;
        if (vector.y == 0) {
            cam = vector;
        } else {
            cam = {0, vector.y, 0};
        }
        if (Snake::rotate_vector(cam, rotation) == facing) {
            std::cout << "rotation is equal to facing: " << std::endl
                      << "rotation: " << Snake::rotate_vector(cam, rotation) << std::endl
                      << "facing: " << facing << std::endl;
            facing = Snake::rotate_vector(cam, rotation * 2);
        } else {
            std::cout << "it worked!\n";
            facing = Snake::rotate_vector(cam, rotation);
        }
    }

  public:
    Snake::int_vector facing;
    Snake::int_vector pos;
    head(const Snake::int_vector &new_pos, const Snake::int_vector &new_direction)
        : pos(new_pos), facing(new_direction), default_facing(new_direction), rotation(0, 0, 0) {
        facing_amount = {1, 0, 0};
    }
    head() : pos(1, 1, 1), facing(1, 0, 0), rotation(0, 0, 0) {}

    void move() { pos += facing; }

    void turn(Snake::vec2 where, Snake::int_vector camera_facing) {
        if (camera_facing.y == 0) {
            if (camera_facing.x == 1) {
                trial_turn(camera_facing, {0, where.x, where.y});
            }
            if (camera_facing.z == 1) {
                trial_turn(camera_facing, {where.y, where.x, 0});
            }
            if (camera_facing.x == -1) {
                trial_turn(camera_facing, {0, where.x, where.y * -1});
            }
            if (camera_facing.z == -1) {
                trial_turn(camera_facing, {where.y * -1, where.x, 0});
            }
        } else if (camera_facing.y == 1) {
            if (camera_facing.x == 1) {
                trial_turn(camera_facing, {where.x, 0, where.y});
            }
            if (camera_facing.z == 1) {
                trial_turn(camera_facing, {where.y, 0, where.x});
            }
            if (camera_facing.x == -1) {
                trial_turn(camera_facing, {where.x * -1, 0, where.y * -1});
            }
            if (camera_facing.z == -1) {
                trial_turn(camera_facing, {where.y * -1, 0, where.x * -1});
            }
        } else if (camera_facing.y == -1) {
            if (camera_facing.x == 1) {
                trial_turn(camera_facing, {where.x * -1, 0, where.y});
            }
            if (camera_facing.z == 1) {
                trial_turn(camera_facing, {where.y, 0, where.x * -1});
            }
            if (camera_facing.x == -1) {
                trial_turn(camera_facing, {where.x, 0, where.y});
            }
            if (camera_facing.z == -1) {
                trial_turn(camera_facing, {where.y, 0, where.x});
            }
        }
    }

    void print_status() {
        std::cout << "facing amount " << facing_amount.x << facing_amount.y << facing_amount.z << std::endl;
        std::cout << "facing " << facing.x << facing.y << facing.z << std::endl;
        std::cout << "rotation " << rotation.x << "," << rotation.y << "," << rotation.z << std::endl;
        std::cout << "\n";
    }
};
int main() {

    float pitch = 45.0f * DEG2RAD;
    float yaw = 45.0f * DEG2RAD;
    float distance = 70.0f;

    head h({1, 1, 1}, {1, 0, 0});

    SetTraceLogLevel(LOG_NONE);

    InitWindow(1920, 1080, "HELLO");
    SetTargetFPS(60);
    DisableCursor();

    camera.position = {40, 40, 0};
    camera.target = {5, 5, 5};
    camera.up = {0, 1.0f, 0};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Snake::set({1, 1, 1}, Snake::food);

    while (!WindowShouldClose()) {
        Vector2 mouse_delta = GetMouseDelta();
        pitch -= mouse_delta.y * 0.005f;
        yaw -= mouse_delta.x * 0.005;
        pitch = std::clamp(pitch, -85.0f * DEG2RAD, 85.0f * DEG2RAD);

        camera.position.x = camera.target.x + distance * cosf(pitch) * sinf(yaw);
        camera.position.y = camera.target.y + distance * sinf(pitch);
        camera.position.z = camera.target.z + distance * cosf(pitch) * cosf(yaw);

        // print the camera pos

        Snake::clear_body();
        Snake::set(h.facing + Snake::int_vector(1, 1, 1), Snake::body);

        // camera facing
        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode3D(camera);

        Snake::render_grid();
        // Origin
        DrawSphere({0, 0, 0}, 0.2f, RED);

        DrawGrid(10, SIZE);

        if (IsKeyPressed(KEY_W)) {
            h.turn(Snake::direction::up, Snake::camera::camera_facing(pitch, yaw));
        }
        if (IsKeyPressed(KEY_S)) {
            h.turn(Snake::direction::down, Snake::camera::camera_facing(pitch, yaw));
        }
        if (IsKeyPressed(KEY_A)) {
            h.turn(Snake::direction::left, Snake::camera::camera_facing(pitch, yaw));
        }
        if (IsKeyPressed(KEY_D)) {
            h.turn(Snake::direction::right, Snake::camera::camera_facing(pitch, yaw));
        }
        EndMode3D();

        EndDrawing();
    }
    CloseWindow();
}
