# Janky Junk
_This is where I'm gonna be storing all my projects!_

So far, I've made Tetris using C++ and Raylib. But I'm currently working on 3D snake.

## Tetris
It's just basic Tetris (Not that impressive). Doesn't involve as much quality of life or colour as official Tetris, 
but it has the basics like *falling blocks*, *rotation* and *line clearing*. When you start it up, a block will start falling and you can move it to where you want it to go as it falls,
or you can make it fall instantly. If you create a full line, it will clear it, and based on how many ones you got consecutively (4 being the max), it will update your score and print it out
when you _exit_ or _lose_

<img width="337" height="648" alt="image" src="https://github.com/user-attachments/assets/ea9aab7c-da80-4f8a-82ae-23846e3dc9ce" />

I made this mostly as a way to get more familiar with Raylib and C++. But it was fun to test!
### Controls
- Right/Left/Down -> move the current block
- Up -> rotate
- Space -> Instant fall
- Esc -> exit

### Installation
It **only works on Linux**, unfortunately, but if you do run on Linux, go to the releases page on the right and follow the instructions there.

## 3D snake

This is snake, but in 3D. It has the actual snake body, random fruit generation, a score, dying, a bit of an input buffer, and a camera that smoothly follows the head of the snake.

The _turning system_ was a bit of a challenge, it had to take in many variables and was a bit hard to implement. In 2D snake, you have the possible 
controls of the snake alternating between **left and right** and **up and down**, but in 3D, you are able to use all of the directions at once, and where you 
want it to point varies significantly based on the **camera** and the **current facing direction** and the **input** given.

<img width="1915" height="1076" alt="image" src="https://github.com/user-attachments/assets/292ddf09-d24d-4ae8-98ce-72015be1cd29" />

I was originally going to make this just regular 2D snake, but I saw the thumbnail of a youtube video with snake but 3D, and was immediately inspired.
This is my exercise to learn more about 3D, and also that it's just a cool idea. 
### Controls 
 - Esc -> exit 
 - Mouse -> Orbit snake head.
 - Scroll wheel -> Zoom in and out.
 - WASD/Arrow keys -> Left, Right, Up, Down based on the camera angle.
#### Gameplay Tips 
 - It works based on the camera angle, so try to *always point the camera at a 90 degree angle* to the snake head.
 - Try to imagine yourself as the snake head, this will make understanding where you are actually going to turn easier.
 - If you try to turn towards the current axis that the snake is facing, it will push it to the next axis. With this, you can **press *up* twice** in order to move up or down a layer, depending on the camera angle a layer, and vise-versa with down.
 - Part of the challenge is using the camera. It's essential to the game, so use the mouse.
 - Try to do axis one at a time, so move the mouse, get it at the same level as the fruit and repeat.
### Installation
Go to the releases -> Snake and follow the instructions there.

