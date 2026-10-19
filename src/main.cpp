#include "raylib.h"
#include <array>
#include <iostream>
const int GRID_WIDTH= 10;
const int GRID_HEIGHT = 20;
const int OFFSET_X = 16;
const int OFFSET_Y = 16;
const int BLOCK_SPACING = 16;
const int BLOCK_WIDTH = 16;
const int BLOCK_HEIGHT= 16;
std::array<std::array<int, GRID_WIDTH>,GRID_HEIGHT> tetrisGrid;

namespace Tetris{
    enum directions{
        up,
        down,
        left,
        right
    };
    enum block_states{
        empty,
        moving,
        frozen
    };
    void draw_blocks(){
        ClearBackground(BLACK);
        for (int y = 0; y < static_cast<int>(tetrisGrid.size()); y++){
            for (int x = 0; x < static_cast<int>(tetrisGrid[y].size()); x++){
                if (tetrisGrid[y][x] == 1){
                    DrawRectangle(x * BLOCK_SPACING + OFFSET_X, y * BLOCK_SPACING + OFFSET_Y, BLOCK_WIDTH, BLOCK_HEIGHT, WHITE);
                }
            }
        }
    }
    void set(int x, int y, int value){
        if ((x< 10 && x>= 0) && (y< 20 && y>= 0)){
            tetrisGrid[y][x] = value;
        }
        else{
            std::cout << "Out of bounds" << std::endl;
        }
    }
    void move(int x, int y, directions direction){
        set(x,y,0);
        switch (direction){
            case up:
                set(x, y - 1, 1);
            case down:
                set(x, y + 1, 1);
            case left:
                set(x -1 ,y , 1);
            case right:
                set(x + 1, y, 1);
        }
    }
    void clear_all_moving(){
        int clearCounter = 0;
        for (int y = 0; y < static_cast<int>(tetrisGrid.size()); y++){
            for (int x = 0; x < static_cast<int>(tetrisGrid[y].size()); x++){
                if (tetrisGrid[y][x] == 1){
                    set(x,y,0);
                    clearCounter++;
                }
            }
        }
    }
}

int main()
{
    InitWindow(BLOCK_SPACING * GRID_WIDTH + OFFSET_Y * 2, BLOCK_SPACING * GRID_HEIGHT + OFFSET_Y*2, "My window");
    SetTargetFPS(60);
    for(std::array y : tetrisGrid){
        for (int x : y){
            x = 0;
        }
    }
    Tetris::set(9, 10, 1);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        Tetris::draw_blocks();
        if(IsKeyPressed(KEY_SPACE)){
            Tetris::clear_all_moving();
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
