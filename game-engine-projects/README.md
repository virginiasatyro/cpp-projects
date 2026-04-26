# C++ Game Engine Projects

## [A* Path Finder](./a-path-finder/)

This project implements an interactive A* pathfinding visualizer using `olcPixelGameEngine`, where a grid of nodes is searched in real time to find the shortest route from a green start node to a red goal node while avoiding white obstacles. The algorithm tracks local cost, heuristic distance, global estimated cost, visited nodes, and parent pointers, then draws the final route in yellow so you can see how the search explores the map and reconstructs the best path.

<img src="./images/astar-path-finder.gif" alt="A* Path Finder" width="20%" height="20%">

## [Game of Life](./game-of-life)

This program implements Conway’s Game of Life using the olcPixelGameEngine: it stores the grid as two 1D arrays (state for the current generation and output as a snapshot of the previous generation), initializes state randomly, then on each frame copies state into output, iterates over internal cells, counts the eight Moore neighbors for each cell, and applies the standard Life rules (a live cell survives with 2 or 3 neighbors, a dead cell becomes alive with exactly 3) to compute the next state; the code renders live cells in green and dead cells in black, updates all cells simultaneously via the double-buffer approach, and provides a simple pause control (SPACE) while using 1D indexing (index = y * width + x) for efficient access.

<img src="./images/game-of-life.gif" alt="Game of Life" width="20%" height="20%">

## [Matrix Rain](./matrix-rain/)

This program implements the well knwon Matrix Rain using the olcPixelGameEngine.

<img src="./images/matrix-rain.gif" alt="Matrix Rain" width="20%" height="20%">

## [Maze Creator](./maze-creator)

This program implements the Recursive Backtracking (depth‑first search) algorithm to generate a perfect maze: starting from a random cell it repeatedly chooses a random unvisited neighboring cell, carves a bidirectional path to it, marks it visited and pushes it onto a stack (thereby moving forward). When the current cell has no unvisited neighbors, the algorithm backtracks by popping the stack until it finds a cell with available neighbors, continuing until every cell has been visited. The code represents the grid as a 1D array with bitwise flags for visited state and open walls (N/E/S/W), so carving and testing paths is efficient, and the process is visualized step‑by‑step to show how the maze grows.

<img src="./images/maze-creator.gif" alt="Maze Generator Animation" width="20%" height="20%">

## [Maze Creator Random Prim](./maze-creator-random-prim/)

This implementation uses the randomized Prim (frontier) method: it begins from a random starting cell, marks it visited and adds its neighbors to a frontier list; then repeatedly picks a random cell from the frontier, connects it to a randomly chosen already-visited neighbor (carving a bidirectional passage), marks that frontier cell visited, adds its unvisited neighbors to the frontier, and removes the processed cell—continuing until the frontier is empty. The result is a perfect maze (no loops, a unique path between any two cells); the code stores the grid as a 1D array with bitwise flags for open walls and visited state, which makes carving, checking neighbors, and visual stepwise rendering efficient.

<img src="./images/maze-creator-random-prim.gif" alt="Maze Generator Animation" width="20%" height="20%">

## [Perlin Noise 1D](./perlin-noise-1d/)

This project demonstrates 1D Perlin noise by generating a random seed array, smoothing it with multiple octaves, and drawing the result as a white waveform across the screen. It is a compact example of procedural noise fundamentals: interpolation, octave layering, scaling bias, reseeding, and how random values can be transformed into smooth natural-looking motion or terrain-like curves.

<img src="./images/perlin-noise-1d.png" alt="Perlin Noise" width="20%" height="20%">

## [Perlin Noise 2D](./perlin-noise-2d/)

This project extends the Perlin noise idea into 2D by generating a full noise field and rendering it as either a heat map or a black-and-white dithered image. The implementation samples and blends random seed values across X and Y, combines multiple octaves for larger and smaller details, and uses the resulting values to create smooth procedural patterns that resemble height maps, clouds, fire, terrain, or temperature maps.

<img src="./images/perlin-noise-2d-2.png" alt="Perlin Noise" width="20%" height="20%">


## [Splines](./splines/)

This program implements an interactive Catmull-Rom spline visualizer that allows real-time manipulation of control points to generate smooth interpolating curves; it demonstrates the mathematical foundation of cubic splines by computing both the position and gradient (tangent vector) at any parameter value along the curve, which enables practical applications like camera paths, AI navigation, and smooth animations—the code supports both open and looped spline modes, renders the curve continuously by sampling at fine intervals, and includes an animated agent (marker) that follows the path while a blue line visualizes the instantaneous direction of travel based on the spline's derivative.

<img src="./images/splines.gif" alt="Splines" width="20%">

## [Television Crazy Pixels](./television-crazy-pixels/)

This program generates a TV static effect by continuously iterating through every pixel on the screen (256×240 resolution) each frame and drawing each one with a randomly generated RGB color value; since this happens at high speed (~60 FPS), the result is a flickering, colorful noise pattern that mimics old television snow, demonstrating fundamental concepts like the game loop architecture, direct pixel manipulation, real-time rendering, and random number generation in a graphics context.

<img src="./images/television-crazy-pixels.gif" alt="Television Crazy Pixels" width="20%">

## [Webcam to Pixels](./webcam-to-pixels/)

This project captures frames from the default webcam through Windows Media Foundation, copies the raw RGB data into a frame buffer, and samples that buffer pixel-by-pixel inside `olcPixelGameEngine` to create a low-resolution, pixelated live camera view. By rendering the webcam image at `320x240` with scaled-up pixels, the program produces a simple retro-style webcam effect while demonstrating real-time frame acquisition, buffer access, coordinate remapping, and direct pixel drawing.

<img src="./images/webcam-capture.png" alt="Webcam Capture" width="20%" height="10%">

## [Webcam to Pixels - Black and White](./webcam-to-pixels-black-and-white/)

This variation builds on the webcam-to-pixels pipeline but converts each sampled RGB pixel to grayscale before drawing it, allowing different black-and-white conversion techniques to be tested in real time. The code includes weighted luminance, simple average, desaturation, and threshold-based conversion methods, making the project a compact demonstration of how different grayscale formulas affect contrast, brightness perception, and the final visual style of a pixelated webcam feed.

<img src="./images/webcam-capture-pb.png" alt="Webcam Capture - Black and White" width="20%" height="1%">

## [Webcam to Pixels - Dither Colors](./webcam-to-pixels-dither-colors/)

This variation turns the live webcam feed into a color-dithered pixel image by converting each sampled RGB pixel into HSV, grouping the hue into strong color bands, and using a 2x2 dither pattern to represent brightness between foreground and background colors. The effect keeps the same Windows Media Foundation capture pipeline as the webcam projects, but replaces direct color drawing with a stylized retro palette made from red, yellow, green, cyan, blue, magenta, black, and white.

<img src="./images/web-cam-dither-colors.png" alt="Webcam Capture - Dither Colors" width="20%" height="20%">

## [Webcam to Pixels - Dither Black and White](./webcam-to-pixels-dither/)

This version renders the webcam feed as a grayscale dither image by converting each sampled pixel to luminance, splitting the brightness into black, dark gray, gray, and white shade bands, and mixing those shades with a simple 2x2 pattern. Compared with a hard black-and-white threshold, the dithered output preserves more facial detail, edges, and lighting variation while still keeping the chunky low-resolution webcam style.

<img src="./images/web-cam-dither.png" alt="Webcam Capture - Black and White" width="20%" height="20%">
