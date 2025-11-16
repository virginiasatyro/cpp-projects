# C++ Projects

## [Maze Creator](./src/maze-creator)

This program implements the Recursive Backtracking (depth‑first search) algorithm to generate a perfect maze: starting from a random cell it repeatedly chooses a random unvisited neighboring cell, carves a bidirectional path to it, marks it visited and pushes it onto a stack (thereby moving forward). When the current cell has no unvisited neighbors, the algorithm backtracks by popping the stack until it finds a cell with available neighbors, continuing until every cell has been visited. The code represents the grid as a 1D array with bitwise flags for visited state and open walls (N/E/S/W), so carving and testing paths is efficient, and the process is visualized step‑by‑step to show how the maze grows.

![Maze Generator Animation](./img/maze-creator.gif)

## [Maze Creator Random Prim](./src/maze-creator-random-prim/)

This implementation uses the randomized Prim (frontier) method: it begins from a random starting cell, marks it visited and adds its neighbors to a frontier list; then repeatedly picks a random cell from the frontier, connects it to a randomly chosen already-visited neighbor (carving a bidirectional passage), marks that frontier cell visited, adds its unvisited neighbors to the frontier, and removes the processed cell—continuing until the frontier is empty. The result is a perfect maze (no loops, a unique path between any two cells); the code stores the grid as a 1D array with bitwise flags for open walls and visited state, which makes carving, checking neighbors, and visual stepwise rendering efficient.

![Maze Generator Animation](./img/maze-creator-random-prim.gif)

## [Television Crazy Pixels](./src/television-crazy-pixels/)

This program generates a TV static effect by continuously iterating through every pixel on the screen (256×240 resolution) each frame and drawing each one with a randomly generated RGB color value; since this happens at high speed (~60 FPS), the result is a flickering, colorful noise pattern that mimics old television snow, demonstrating fundamental concepts like the game loop architecture, direct pixel manipulation, real-time rendering, and random number generation in a graphics context.

![Television Crazy Pixels](./img/television-crazy-pixels.gif)