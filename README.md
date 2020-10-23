# Blasteroids
Remember the game Blasteroids from the 80s and early 90s?
This is basically a more basic version of it, which was built using only lines to draw everything on a 2D plane.

## About the project
The purpose behind this project is completely educational. I finished reading Head First C as my introduction to C,
then I decided that building a game without using a game framework for the 2D transformations or the collision detection
would be a perfect step to pick up the Linear Algebra skills needed to solve these problems, and of course the necessary C skills.

I obviously used Allegro 5 to handle the lower-lever graphic problems, as well as handling the event queue and listener.
Otherwise, everything else in the game is written by me and implemented by me. This project, though isn't completed,
has taught me a lot of very valuable skills as a programmer in general,
such as Memory Management, Game Loops, 2D Transformations in computational graphics as well as
the necessary debugging skills to debug syntax, runtime, logic and behavourial errors using important C tools (mainly Valgrind, AddressSanitizer and GDB).

## How does it work?
* Use right and left arrow keys to rotate the ship
* Use the upper arrow key to move the ship in the direction it's directed at
* Use the space bar to fire your blasts!

## What does the game lack so far?
1. There's no collision detection between the asteroids and the ship yet
2. The game only works on linux so far
3. Although there's a score calculater, the game's behaviour doesn't change based on your score, i.e., there's no leveling up