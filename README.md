# Tetris Game (C++)

A simple implementation of the classic Tetris game using C++ and a console-based interface. The player manipulates falling blocks to create and clear full horizontal lines. The game ends when the blocks reach the top of the screen. Developed for Windows OS.

## Table of Contents
- Project Description
- Installation
- Usage
- Contributing

## Project Description
This is a console-based Tetris game written in C++ that replicates the classic game where players rotate and position falling tetrominoes to clear lines and score points. The game becomes progressively challenging as blocks fall faster over time.

### Key Features:
- Simple console-based interface.
- Tetrominoes can be moved left, right, rotated, and dropped.
- Rows disappear when completely filled.
- Speed increases as more rows are cleared.
- Game ends when blocks reach the top of the screen.

## Installation
This game requires the following C++ libraries:
- `<iostream>` – Used for input and output operations (printing to the console).
- `<vector>` – Used for handling tetromino shapes and managing the game grid.
- `<ctime>` – Used for seeding the random number generator to generate tetromino shapes.
- `<conio.h>` – Provides functions like `_kbhit()` and `_getch()` for detecting user input without waiting for Enter (used to control tetromino movement and rotation).
- `<windows.h>` – Used to control the sleep interval (game speed) and clear the console screen between frames (via the `Sleep()` function).
- `<cstdlib>` – Used for random number generation.

To run the Tetris game on your local machine, follow these steps:

1. Clone the repository:
   ```sh
   git clone https://github.com/Code-Conquorers/Tetris-Game.git
   ```

2. Navigate to the project directory:
   ```sh
   cd tetrisgame
   ```

3. Compile the C++ code:
   ```sh
   g++ tetris.cpp -o tetris.exe
   ```

4. Run the game:
   ```sh
   ./tetris
   ```

## Usage
1. **Start the game:** Run the program, and tetrominoes will begin to fall from the top of the screen.
2. **Move the blocks:** Use the following keys to control the tetrominoes:
   - `←` - Move left
   - `→` - Move right
   - `↑` - Rotate the tetromino
   - `↓` - Soft drop (move down faster)
   - `Space` - Hard drop (instantly place piece)
   - `ESC` - Pause/Quit the game
3. **Objective:** Fit the tetrominoes together to create and clear complete rows. The game gets faster as more rows are cleared. The game ends when the blocks reach the top of the screen.

## Contributing
We welcome contributions to the Tetris game! To contribute, please follow these steps:

1. Fork the repository.
2. Create a new branch:
   ```sh
   git checkout -b feature-name
   ```
3. Make your changes and test them.
4. Commit your changes:
   ```sh
   git commit -am "Add new feature or fix bug"
   ```
5. Push to the branch:
   ```sh
   git push origin feature-name
   ```
6. Create a pull request.

Enjoy playing Tetris!

