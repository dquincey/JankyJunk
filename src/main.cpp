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
    struct grid_coords{
        int x;
        int y;
        int z;
    };
    const int GRID_Z = 5;
    const int GRID_X = 5;
    const int GRID_Y = 5;

    std::array<std::array<std::array<int,GRID_Z>,GRID_Y>,GRID_X>grid;

    void set(grid_coords coords, block_type value){
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
int main(){
    InitWindow(1920, 1080, "HELLO");
    SetTargetFPS(60);

   camera.position = {20, 20, 0};
   camera.target = {0,0,0};
   camera.up = {0,1.0f,0};
   camera.fovy = 45.0f;
   camera.projection = CAMERA_PERSPECTIVE;

   float x = 0.0f;
   float y = 0.0f;
   float z = 0.0f;

   Snake::set({1,0,2}, Snake::body);
   while (!WindowShouldClose()) {


       BeginDrawing();
       ClearBackground(BLACK);
       BeginMode3D(camera);

       Snake::render_grid();
       //Origin
       DrawSphere({0,0,0}, 0.2f, RED);

       DrawGrid(10, SIZE);

       if (IsKeyDown(KEY_W)){
           x -= 0.1f;
       }
       if (IsKeyDown(KEY_S)){
           x += 0.1f;
       }
       if (IsKeyDown(KEY_A)){
           z += 0.1f;
       }
       if (IsKeyDown(KEY_D)){
           z -= 0.1f;
       }
       EndMode3D();

       EndDrawing();
   }
   CloseWindow();

}
