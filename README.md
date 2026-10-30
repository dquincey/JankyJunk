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

This is my attempt at making 3D snake. I've spent most of my time being confused, at 3D and how I'm going to do the turning system. So this is currently
a work in progress and there is no actual game loop. Just a square in the middle of a black void. But, the camera can orbit, and from here I hope it will
be easier to create the systems as what I've done so far is create tools and functions to make the harder parts hopefully easier.
<img width="1920" height="1080" alt="image" src="https://github.com/user-attachments/assets/9f8061c1-e8be-464c-a982-cb2d54f84ef5" />

I was originally going to make this just regular 2D snake, but I saw the thumbnail of a youtube video with snake but 3D, and was immediately inspired.
This is my exercise to learn more about 3D, and also that it's just a cool idea. 

### If you wanna try it for some reason
Go to the releases -> Snake and follow the instructions there.

