#include "raylib.h"
#include <array>
#include <iostream>
#include <vector>
const int GRID_WIDTH= 10;
const int GRID_HEIGHT = 20;
const int OFFSET_X = 16;
const int OFFSET_Y = 16;
const int BLOCK_SPACING = 16;
const int BLOCK_WIDTH = 16;
const int BLOCK_HEIGHT= 16;
std::array<std::array<int, GRID_WIDTH>,GRID_HEIGHT> tetrisGrid;

namespace Tetris{
    struct position{
        int x;
        int y;
    };
    enum directions{
        up,
        down,
        left,
        right,
        nil
    };
    enum block_states{
        empty,
        moving,
        frozen
    };
    bool check(position pos,directions direction = nil){
        position proposedPos;
        switch (direction){
            case up:
                proposedPos = {pos.x, pos.y - 1};
            case down:
                proposedPos = {pos.x, pos.y + 1};
            case left:
                proposedPos = {pos.x -1, pos.y};
            case right:
                proposedPos = {pos.x + 1, pos.y};
            default:
                proposedPos = {pos.x, pos.y};
        }
        if (proposedPos.x < 10 && proposedPos.x >= 0 && proposedPos.y < 20 && proposedPos.y >= 0 && tetrisGrid[proposedPos.y][proposedPos.x] != 2){
            return true;
        }
        else {
            std::cout << "wasn't allowed" <<std::endl;
            return false;
        }

    }
    void draw_blocks(){
        ClearBackground(BLACK);
        for (int y = 0; y < static_cast<int>(tetrisGrid.size()); y++){
            for (int x = 0; x < static_cast<int>(tetrisGrid[y].size()); x++){
                if (tetrisGrid[y][x] == moving || tetrisGrid[y][x] == frozen){
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
            default:
                break;
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
class Tetromino{
    public:
        std::vector<std::array<Tetris::position, 4>> blockPositions;
        Tetris::position pos = {0,0};
        int currentRotation = 0;

        void applyPositions(){
            Tetris::clear_all_moving();
            for (Tetris::position i : blockPositions[currentRotation]){
                Tetris::set(pos.x + i.x, pos.y+ i.y, 1);
            }
        }
        bool positionsCorrect(){
            bool correct = true;
            for (Tetris::position i : blockPositions[currentRotation]){
                if (Tetris::check({pos.x + i.x, pos.y + i.y}) == false){
                    correct = false;
                }
            }
            return correct;
        }
        void move (Tetris::directions direction){
            Tetris::position oldPos = pos;
            for (Tetris::position i : blockPositions[currentRotation]){
            }
            switch (direction){
                case Tetris::up:
                    pos.y -= 1;
                case Tetris::down:
                    pos.y += 1;
                case Tetris::left:
                    pos.x -= 1;
                case Tetris::right:
                    pos.x += 1;
                default:
                    break;
            }
            if(positionsCorrect() == true){
                applyPositions();
            }
            else{
                pos = oldPos;
            }
        }
        void rotate(){
            int oldRot = currentRotation;
            for (Tetris::position i : blockPositions[currentRotation]){
            }
            currentRotation = currentRotation < static_cast<int>(blockPositions.size()) -1 ? currentRotation + 1 : 0;
            if(positionsCorrect() == true){
                applyPositions();
            }
            else{
                currentRotation = oldRot;
            }
        }
};
class iBlock : public Tetromino{
    public:
        iBlock(){
            blockPositions = {
                {{ {0,0},{0,1},{0,2},{0,3} }},
                {{ {0,0},{1,0},{2,0},{3,0} }},
            };
        }
};

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
    iBlock block;
    block.applyPositions();
    while (!WindowShouldClose())
    {
        BeginDrawing();
        Tetris::draw_blocks();
        if(IsKeyPressed(KEY_SPACE)){
            block.move(Tetris::down);
        }
        if(IsKeyPressed(KEY_UP)){
            block.rotate();
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
