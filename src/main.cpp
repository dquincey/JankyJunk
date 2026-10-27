#include <cstdlib>
#include <ostream>
#include <raylib.h>
#include <array>
#include <iostream>
const int WIDTH = 32;
const int HEIGHT = 32;
const float SIZE = 5.0f;
Camera3D camera = {};

namespace Snake{
    enum block_type{
        empty,
        body,
        food,
    };
    struct grid_vector{
        int x;
        int y;
        int z;

        grid_vector operator+(const grid_vector& other) const{
            return {other.x + x, other.y+y,other.z+z};
        }
        void operator+=(const grid_vector& other){
            *this = {other.x + x, other.y+y,other.z+z};
        }
        grid_vector operator%(const int& divisor){
            return {x % divisor, y % divisor, z % divisor};
        }
    };
    struct vec2{
        int x;
        int y;
    };
    namespace direction{
        vec2 up = {0,1};
        vec2 down = {0,-1};
        vec2 left = {-1 ,0};
        vec2 right = {1,0};
    }
    const int GRID_Z = 5;
    const int GRID_X = 5;
    const int GRID_Y = 5;

    std::array<std::array<std::array<int,GRID_Z>,GRID_Y>,GRID_X>grid;

    void set(grid_vector coords, block_type value){
        //Checks whether it's within the array
        bool within = true;
        if (coords.x > GRID_X || coords.x < 0){
            within = false;
            std::cout<<"Wasn't within X\n";
        }
        if (coords.y > GRID_Y || coords.y < 0){
            within = false;
            std::cout<<"Wasn't within y\n";
        }
        if (coords.z > GRID_Z || coords.z < 0){
            within = false;
            std::cout<<"Wasn't within z\n";
        }
        if (within){
            grid[coords.z][coords.y][coords.x] = value;
        }
    }
    void render_grid(){
        for (int z = 0; z < static_cast<int>(grid.size()); z++){
            for (int y = 0; y < static_cast<int>(grid[z].size()); y++){
                for (int x = 0; x < static_cast<int>(grid[z][y].size()); x++){
                    if (grid[z][y][x] == body){
                        DrawCube({x*SIZE,y*SIZE,z*SIZE}, SIZE, SIZE , SIZE, WHITE);
                        DrawCubeWires({x*SIZE,y*SIZE,z*SIZE}, SIZE, SIZE , SIZE, BLACK);
                    }
                }
            }
        }
    }
}
class head{
    private:
        Snake::grid_vector pos;
        Snake::grid_vector facing;
        Snake::grid_vector rotation;
        std::array<int,4> axisrotation= {0,1,0,-1};
    public:
        head(const Snake::grid_vector& new_pos, const Snake::grid_vector& new_direction, const Snake::grid_vector& new_rotation) :pos(new_pos) , facing(new_direction), rotation(new_rotation)
        {
            std::cout << facing.x << facing.y << facing.z << std::endl;
        }
        head() :pos({0,0,0}) , facing({1,0,0}), rotation({0,0,0})
        {}
        void move(){
            pos += facing;
        }
        void change_rotation(Snake::grid_vector rotation_change){
            rotation += rotation_change;
            rotation = ((rotation % 4) + Snake::grid_vector{4,4,4}) % 4;
        }
        void change_facing(Snake::vec2 direction_change){
            std::cout << "direction change: " << direction_change.x << "," << direction_change.y <<std::endl;
            if (std::abs(facing.x)== 1){
                std::cout << "facing x!\n";
                change_rotation({0, direction_change.x, direction_change.y});
                facing = {0,axisrotation[rotation.y],axisrotation[rotation.z]};
            }
            else if (std::abs(facing.y)== 1){
                std::cout << "facing y!\n";
                change_rotation({direction_change.x, 0, direction_change.y});
                facing = {axisrotation[rotation.x],0,axisrotation[rotation.z]};
            }
            else if (std::abs(facing.z)== 1){
                std::cout << "facing z!\n";
                change_rotation({direction_change.x, direction_change.y, 0});
                facing = {axisrotation[rotation.x],axisrotation[rotation.y],0};
            }
        }
        void print_facing(){
            std::cout << "facing " << facing.x << facing.y << facing.z<<std::endl;
            std::cout << "rotation " << rotation.x << rotation.y << rotation.z << std::endl;
        }
};
int main(){
    InitWindow(1920, 1080, "HELLO");
    SetTargetFPS(60);

   camera.position = {20, 20, 0};
   camera.target = {0,0,0};
   camera.up = {0,1.0f,0};
   camera.fovy = 45.0f;
   camera.projection = CAMERA_PERSPECTIVE;

   head h({0,0,0},{1,0,0},{0,0,0});
   Snake::set({1,1,1}, Snake::body);
   while (!WindowShouldClose()) {


       BeginDrawing();
       ClearBackground(BLACK);
       BeginMode3D(camera);

       Snake::render_grid();
       //Origin
       DrawSphere({0,0,0}, 0.2f, RED);

       DrawGrid(10, SIZE);

       if (IsKeyPressed(KEY_W)){
           h.change_facing(Snake::direction::up);
           h.print_facing();
       }
       if (IsKeyPressed(KEY_S)){
           h.change_facing(Snake::direction::down);
           h.print_facing();
       }
       if (IsKeyPressed(KEY_A)){
           h.change_facing(Snake::direction::left);
           h.print_facing();
       }
       if (IsKeyPressed(KEY_D)){
           h.change_facing(Snake::direction::right);
           h.print_facing();
       }
       EndMode3D();

       EndDrawing();
   }
   CloseWindow();

}
