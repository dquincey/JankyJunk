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
    struct grid_vector {
        int x;
        int y;
        int z;

        grid_vector operator+(const grid_vector &other) const {
            return {other.x + x, other.y + y, other.z + z};
        }
        void operator+=(const grid_vector &other) {
            *this = {other.x + x, other.y + y, other.z + z};
        }
        grid_vector operator%(const int &divisor) {
            return {x % divisor, y % divisor, z % divisor};
        }
        friend std::ostream& operator<<(std::ostream& stream,const grid_vector& other){
            return stream << other.x << ", " << other.y << ","<<other.z << std::endl;
        }
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
    } // namespace direction
    const int GRID_Z = 5;
    const int GRID_X = 5;
    const int GRID_Y = 5;

    std::array<std::array<std::array<int, GRID_Z>, GRID_Y>, GRID_X> grid;

    void set(grid_vector coords, block_type value) {
        // Checks whether it's within the array
        bool within = true;
        if (coords.x > GRID_X || coords.x < 0) {
            within = false;
            std::cout << "Wasn't within X\n";
        }
        if (coords.y > GRID_Y || coords.y < 0) {
            within = false;
            std::cout << "Wasn't within y\n";
        }
        if (coords.z > GRID_Z || coords.z < 0) {
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
                        DrawCubeWires({x * SIZE, y * SIZE, z * SIZE}, SIZE, SIZE, SIZE,
                            BLACK);
                    }
                }
            }
        }
    }
    void clear_body(){
        for (std::array<std::array<int, GRID_X>,GRID_Y>& z : grid){
            for (std::array<int, GRID_X>& y : z ){
                for (int& x : y){
                    if (x == body){
                        x = empty;
                    }
                }
            }
        }
    }
} // namespace Snake

class head {
    private:
        Snake::grid_vector pos;
        Snake::grid_vector facing;
        Snake::grid_vector rotation;
        Snake::grid_vector facing_amount;
        Snake::grid_vector default_facing;
        std::string axisfacing = "x";

        void facing_change(Snake::grid_vector rotation_change) {
            std::array<int, 4> positives {0,1,0,-1};
            std::cout << "rotation_change "<< rotation_change.x << rotation_change.y << rotation_change.z << std::endl;
            facing_amount += rotation_change;
            facing_amount = ((facing_amount% 4) + Snake::grid_vector{4, 4, 4}) % 4;
            facing = {positives[facing_amount.x],positives[facing_amount.y], positives[facing_amount.z]};

        }
    public:
        head(const Snake::grid_vector &new_pos,
            const Snake::grid_vector &new_direction)
        : pos(new_pos), facing(new_direction), default_facing(new_direction), rotation({0,0,0})
        {
            facing_amount = {1,0,0};
            print_status();
        }
        head() : pos({1, 1, 1}), facing({1, 0, 0}), rotation({0, 0, 0}) {}

        void move() { pos += facing; }

        void turn(Snake::vec2 where){
            if (std::abs(facing.x) == 1) {
                std::cout <<"was facing x\n";
                rotation.y += where.x;
                rotation.z += where.y;
                std::cout << "y rotation: " <<where.y << " z rotation: " << where.x << std::endl;
            }
            if (std::abs(facing.y) == 1) {
                std::cout <<"was facing y\n";
                rotation.x += where.x;
                rotation.z += where.y;
                std::cout << "x rotation: " <<where.y << " z rotation: " << where.x << std::endl;
            }
            if (std::abs(facing.z) == 1) {
                std::cout <<"was facing z\n";
                rotation.y += where.x;
                rotation.x += where.y;
                std::cout << "y rotation: " <<where.x << " x rotation: " << where.y << std::endl;
            }
            facing = default_facing;
            facing_amount = default_facing;
            std::cout << "facing becomes " << default_facing.x <<default_facing.y <<default_facing.z <<std::endl;
            facing_change({0, rotation.x, rotation.x});
            facing_change({rotation.y, 0, rotation.y});
            facing_change({rotation.z, rotation.z,0 });
            if (std::abs(facing.x) == 1) {
                std::cout <<"now facing x\n";
            }
            if (std::abs(facing.y) == 1) {
                std::cout <<"now facing y\n";
            }
            if (std::abs(facing.z) == 1) {
                std::cout <<"now facing z\n";
            }
            Snake::clear_body();
            Snake::set(facing + pos, Snake::body);
            std::cout << facing + pos << std::endl;
        }

        void print_status() {
            std::cout << "facing amount " << facing_amount.x << facing_amount.y <<facing_amount.z << std::endl;
            std::cout << "facing " << facing.x << facing.y << facing.z << std::endl;
            std::cout << "rotation " << rotation.x<< ","<< rotation.y << "," << rotation.z<< std::endl;
            std::cout << "\n";

        }
};
int main() {

    float pitch = 45.0f * DEG2RAD;
    float yaw = 45.0f * DEG2RAD;
    float distance = 70.0f;

    InitWindow(1920, 1080, "HELLO");
    SetTargetFPS(60);
    DisableCursor();

    camera.position = {40, 40, 0};
    camera.target = {5, 5, 5};
    camera.up = {0, 1.0f, 0};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    head h({1, 1, 1}, {1, 0, 0} );
    Snake::set({1, 1, 1}, Snake::food);

    while (!WindowShouldClose()) {
        Vector2 mouse_delta = GetMouseDelta();
        pitch -= mouse_delta.y * 0.005f;
        yaw -= mouse_delta.x * 0.005;
        pitch = std::clamp(pitch, -85.0f * DEG2RAD, 85.0f * DEG2RAD);

        camera.position.x = camera.target.x + distance * cosf(pitch) * sinf(yaw);
        camera.position.y = camera.target.y + distance * sinf(pitch);
        camera.position.z = camera.target.z + distance * cosf(pitch) * cosf(yaw);

        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode3D(camera);

        Snake::render_grid();
        // Origin
        DrawSphere({0, 0, 0}, 0.2f, RED);

        DrawGrid(10, SIZE);

        if (IsKeyPressed(KEY_W)) {
            h.turn(Snake::direction::up);
            h.print_status();
        }
        if (IsKeyPressed(KEY_S)) {
            h.turn(Snake::direction::down);
            h.print_status();
        }
        if (IsKeyPressed(KEY_A)) {
            h.turn(Snake::direction::left);
            h.print_status();
        }
        if (IsKeyPressed(KEY_D)) {
            h.turn(Snake::direction::right);
            h.print_status();
        }
        EndMode3D();

        EndDrawing();
    }
    CloseWindow();
}
