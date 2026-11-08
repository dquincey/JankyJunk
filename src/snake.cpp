#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <raylib.h>
#include <raymath.h>
#include <vector>

const int WIDTH = 32;
const int HEIGHT = 32;
const float SIZE = 5.0f;

Camera3D camera = {};
namespace Snake {
int score = 0;
bool game_over = false;
auto speed = std::chrono::milliseconds(270);
auto input_buffer = std::chrono::milliseconds(270);
void die() {
    Snake::game_over = true;
    std::cout << "Game over!\n";
}
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
    Vector3 operator-(const Vector3 &other) const {
        return {static_cast<float>(x) - other.x, static_cast<float>(y) - other.y, static_cast<float>(z) - other.z};
    }
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
    bool operator>(const int_vector &other) {
        bool return_bool = false;
        if (other.x < x && other.y < y && other.z < z) {
            return_bool = true;
        }
        return return_bool;
    }
    bool operator<(const int_vector &other) {
        bool return_bool = false;
        if (x < other.x && other.y > y && other.z > z) {
            return_bool = true;
        }
        return return_bool;
    }
    bool operator<=(const int_vector &other) {
        bool return_bool = false;
        if (x <= other.x && y <= other.y && z <= other.z) {
            return_bool = true;
        }
        return return_bool;
    }
    bool operator>=(const int_vector &other) {
        bool return_bool = false;
        if (x >= other.x && y >= other.y && z >= other.z) {
            return_bool = true;
        }
        return return_bool;
    }
    friend std::ostream &operator<<(std::ostream &stream, const int_vector &other) {
        return stream << other.x << ", " << other.y << "," << other.z << std::endl;
    }
    int_vector abs() { return int_vector(std::abs(x), std::abs(y), std::abs(z)); }
    Vector3 int_to_float() { return {static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)}; }
};
struct vec2 {
    int x;
    int y;
};
std::vector<vec2> input_list = {};
namespace direction {
vec2 up = {0, 1};
vec2 down = {0, -1};
vec2 left = {-1, 0};
vec2 right = {1, 0};
} // namespace direction
const int GRID_Z = 15;
const int GRID_X = 15;
const int GRID_Y = 15;

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
                if (grid[z][y][x] == food) {
                    DrawCube({x * SIZE, y * SIZE, z * SIZE}, SIZE, SIZE, SIZE, GREEN);
                    DrawCubeWires({x * SIZE, y * SIZE, z * SIZE}, SIZE, SIZE, SIZE, BLACK);
                } else if (grid[z][y][x] == body) {
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
void spawn_food() {
    int_vector rand_pos =
        int_vector(GetRandomValue(0, GRID_X - 1), GetRandomValue(0, GRID_Y - 1), GetRandomValue(0, GRID_Z - 1));
    while (grid[rand_pos.z][rand_pos.y][rand_pos.x] == body) {
        rand_pos = int_vector(GetRandomValue(0, GRID_X), GetRandomValue(0, GRID_Y), GetRandomValue(0, GRID_Z));
    }
    set(rand_pos, food);
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
float lerp(float from, float to, float time) {
    time = std::clamp(time, 0.0f, 1.0f);
    return (to - from) * time + from;
}
float move_towards(float from, float to, float speed) {
    if (to - from > 0) {
        return std::clamp(speed + from, from, to);

    } else if (to - from < 0) {
        return std::clamp(from - speed, to, from);
    } else {
        return from;
    }
}
} // namespace Snake

class head {
  private:
    Snake::int_vector facing;
    Snake::int_vector pos;
    std::vector<Snake::int_vector> body = {};
    void trial_turn(Snake::int_vector vector, Snake::int_vector rotation) {
        Snake::int_vector cam;
        if (vector.y == 0) {
            cam = vector;
        } else {
            cam = {0, vector.y, 0};
        }
        if (Snake::rotate_vector(cam, rotation) == facing) {
            facing = Snake::rotate_vector(cam, rotation * 2);
        } else if (Snake::rotate_vector(cam, rotation) == facing * -1) {
            facing = cam;
        } else {
            facing = Snake::rotate_vector(cam, rotation);
        }
    }

    bool can_move() {
        Snake::int_vector fp = facing + pos;
        if (fp.x >= Snake::GRID_X || fp.x < 0 || fp.y >= Snake::GRID_Y || fp.y < 0 || fp.z >= Snake::GRID_Z ||
            fp.z < 0) {
            return false;
        } else if (Snake::grid[fp.z][fp.y][fp.x] == Snake::body) {
            return false;
        } else {
            return true;
        }
    }

  public:
    head(const Snake::int_vector &new_pos, const Snake::int_vector &new_direction)
        : pos(new_pos), facing(new_direction) {
        body.emplace_back(Snake::int_vector(pos.x, pos.y, pos.z - 1));
        body.emplace_back(Snake::int_vector(pos.x, pos.y, pos.z - 2));
        body.emplace_back(Snake::int_vector(pos.x, pos.y, pos.z - 3));
    }
    head() : pos(1, 1, 1), facing(1, 0, 0) {}

    Vector3 get_position() { return pos.int_to_float() * SIZE; }
    Vector3 get_camera_target(Vector3 camera_pos) {
        static Vector3 base_pos = {pos.int_to_float() * SIZE};
        Vector3 real_pos = pos.int_to_float() * SIZE;
        Vector3 current_pos = camera_pos;
        float distance = Vector3Distance(base_pos, pos.int_to_float() * SIZE);
        float frames_per_step = Snake::speed.count() / (GetFrameTime() * 1000);
        float velocity = distance / frames_per_step;
        float amount_to_move_per_frame = velocity;

        Vector3 return_value = camera_pos;
        if (velocity > 0) {
            return_value = {
                Snake::move_towards(camera_pos.x, real_pos.x, velocity),
                Snake::move_towards(camera_pos.y, real_pos.y, velocity),
                Snake::move_towards(camera_pos.z, real_pos.z, velocity),
            };
        }
        if (return_value == real_pos) {
            base_pos = real_pos;
        }
        return return_value;
    }
    void move(Snake::int_vector camera_facing) {
        if (Snake::input_list.size() > 0) {
            turn(Snake::input_list[0], camera_facing);
            Snake::input_list.erase(Snake::input_list.begin());
        }
        if (can_move() == true) {
            if (Snake::grid[facing.z + pos.z][facing.y + pos.y][facing.x + pos.x] == Snake::food) {
                Snake::spawn_food();
                add_length();
                Snake::score++;
            }
        } else {
            Snake::die();
        }
        for (int i = body.size() - 1; i >= 0; i--) {
            if (i == 0) {
                body[i] = pos;
            } else {
                body[i] = body[i - 1];
            }
        }
        pos += facing;
    }

    void turn(Snake::vec2 where, Snake::int_vector camera_facing) {
        if (camera_facing.y == 0) {
            if (camera_facing.x == 1) {
                trial_turn(camera_facing, {0, where.x * -1, where.y});
                // std::cout << "0y+x\n";
            }
            if (camera_facing.z == 1) {
                trial_turn(camera_facing, {where.y, where.x * -1, 0});
                // std::cout << "0y+z\n";
            }
            if (camera_facing.x == -1) {
                trial_turn(camera_facing, {0, where.x * -1, where.y * -1});
                // std::cout << "0y-x\n";
            }
            if (camera_facing.z == -1) {
                trial_turn(camera_facing, {where.y * -1, where.x * -1, 0});
                // std::cout << "0y-z\n";
            }
        } else if (camera_facing.y == 1) {
            if (camera_facing.x == 1) {
                trial_turn(camera_facing, {where.x * -1, 0, where.y});
                // std::cout << "+y+x\n";
            }
            if (camera_facing.z == 1) {
                trial_turn(camera_facing, {where.y, 0, where.x});
                // std::cout << "+y+z\n";
            }
            if (camera_facing.x == -1) {
                trial_turn(camera_facing, {where.x, 0, where.y * -1});
                // std::cout << "+y-x\n";
            }
            if (camera_facing.z == -1) {
                trial_turn(camera_facing, {where.y * -1, 0, where.x});
                // std::cout << "+y-z\n";
            }
        } else if (camera_facing.y == -1) {
            if (camera_facing.x == 1) {
                trial_turn(camera_facing, {where.x * -1, 0, where.y});
                // std::cout << "-y+x\n";
            }
            if (camera_facing.z == 1) {
                trial_turn(camera_facing, {where.y, 0, where.x});
                // std::cout << "-y+z\n";
            }
            if (camera_facing.x == -1) {
                trial_turn(camera_facing, {where.x, 0, where.y * -1});
                // std::cout << "-y-x\n";
            }
            if (camera_facing.z == -1) {
                trial_turn(camera_facing, {where.y * -1, 0, where.x * -1});
                // std::cout << "-y-z\n";
            }
        }
    }
    void add_length() { body.emplace_back(body[body.size() - 1]); }
    void apply_body() {
        Snake::set(pos, Snake::body);
        for (Snake::int_vector i : body) {
            Snake::set(i, Snake::body);
        }
    }
};
int main() {

    float pitch = 45.0f * DEG2RAD;
    float yaw = 45.0f * DEG2RAD;
    float distance = 70.0f;

    head snake_head({static_cast<int>(std::floor(Snake::GRID_X / 2.0f)),
                     static_cast<int>(std::floor(Snake::GRID_Y / 2.0f)),
                     static_cast<int>(std::floor(Snake::GRID_Z / 2.0f))},
                    {1, 0, 0});

    SetTraceLogLevel(LOG_NONE);

    InitWindow(1920, 1080, "HELLO");
    SetTargetFPS(60);
    DisableCursor();

    Snake::spawn_food();
    camera.position = {snake_head.get_position()};
    camera.target = {snake_head.get_camera_target(camera.position)};
    camera.up = {0, 1.0f, 0};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    auto old_time_game_loop = std::chrono::steady_clock::now();
    auto old_time_buffer = std::chrono::steady_clock::now();

    while (!Snake::game_over) {
        Vector2 mouse_delta = GetMouseDelta();
        pitch += mouse_delta.y * 0.005f;
        yaw -= mouse_delta.x * 0.005;
        pitch = std::clamp(pitch, -85.0f * DEG2RAD, 85.0f * DEG2RAD);

        camera.position.x = camera.target.x + distance * cosf(pitch) * sinf(yaw);
        camera.position.y = camera.target.y + distance * sinf(pitch);
        camera.position.z = camera.target.z + distance * cosf(pitch) * cosf(yaw);
        camera.target = snake_head.get_camera_target(camera.target);
        distance -= GetMouseWheelMove() * 5;
        distance = std::clamp(distance, 0.15f, 140.0f);

        Snake::clear_body();
        // Snake::set(h.pos, Snake::body);
        // Snake::set(h.pos + h.facing, Snake::body);
        snake_head.apply_body();

        // camera facing
        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode3D(camera);

        Snake::render_grid();
        // Origin
        DrawSphere({0, 0, 0}, 0.2f, RED);
        DrawCubeWires({(Snake::GRID_X * SIZE) / 2.0f, (Snake::GRID_Y * SIZE) / 2.0f, (Snake::GRID_Z * SIZE) / 2.0f},
                      Snake::GRID_X * SIZE, Snake::GRID_Y * SIZE, Snake::GRID_Z * SIZE, WHITE);

        // DrawGrid(10, SIZE);
        if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) {
            Snake::input_list.emplace_back(Snake::direction::up);
            // snake_head.turn(Snake::direction::up, Snake::camera::camera_facing(pitch, yaw));
        }
        if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) {
            Snake::input_list.emplace_back(Snake::direction::down);
            // snake_head.turn(Snake::direction::down, Snake::camera::camera_facing(pitch, yaw));
        }
        if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_RIGHT)) {
            Snake::input_list.emplace_back(Snake::direction::right);
            // snake_head.turn(Snake::direction::left, Snake::camera::camera_facing(pitch, yaw));
        }
        if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_LEFT)) {
            Snake::input_list.emplace_back(Snake::direction::left);
            // snake_head.turn(Snake::direction::right, Snake::camera::camera_facing(pitch, yaw));
        }
        if (IsKeyPressed(KEY_SPACE)) {
            snake_head.add_length();
            Snake::spawn_food();
        }
        if (WindowShouldClose()) {
            Snake::game_over = true;
        }
        if (std::chrono::steady_clock::now() - old_time_game_loop >= Snake::speed) {
            snake_head.move(Snake::camera::camera_facing(pitch, yaw));
            old_time_game_loop = std::chrono::steady_clock::now();
        }
        EndMode3D();

        EndDrawing();
    }
    CloseWindow();
    std::cout << "Score: " << Snake::score << std::endl;
}
