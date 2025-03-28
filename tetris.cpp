#include <iostream>
#include <windows.h>
#include <conio.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

const int WIDTH = 10;
const int HEIGHT = 20;
const int BASE_FALL_DELAY = 500;  // time before piece move down(milisecond)

class Piece {
public:
    int x, y;
    vector<vector<char>> shape;  // 2D vector

    Piece(vector<vector<char>> shape) : shape(shape) {   // condition for shape top center
        x = WIDTH / 2 - shape[0].size() / 2;
        y = 0;
    }

    bool checkCollision(int newX, int newY, const vector<vector<char>>& board) const {// moving a Tetromino to a new position 
        for (size_t i = 0; i < shape.size(); i++) {
            for (size_t j = 0; j < shape[i].size(); j++) {
                if (shape[i][j] != ' ') {
                    int boardX = newX + j;
                    int boardY = newY + i;
                    if (boardX < 0 || boardX >= WIDTH || boardY >= HEIGHT ||  // collision detected
                        (boardY >= 0 && board[boardY][boardX] != ' ')) {
                        return true;
                    }
                }
            }
        }
        return false;
    }


    // this is rotate like matrix 2* 3 or 3*2
    void rotate(const vector<vector<char>>& board) {
        vector<vector<char>> rotated(shape[0].size(), vector<char>(shape.size(), ' '));   // 2d matrix rotete formula

        for (size_t i = 0; i < shape.size(); i++) {
            for (size_t j = 0; j < shape[i].size(); j++) {
                rotated[j][shape.size() - 1 - i] = shape[i][j];   //  90-degree clockwise rotation formula 
            }
        }

        if (!checkCollision(x, y, board)) {  // if hit a wall and other spices no rotation
            shape = rotated;
        }   
    }
};

class Game {
private:
    vector<vector<char>> board;  // game board 
    vector<vector<vector<char>>> tetrominoes;  // store terminose shape
    Piece* currentPiece;  // curruntly fall spice
    int score;
    int level;
    int linesCleared;
    bool gameOver;
    bool paused;
    HANDLE consoleHandle;

public:
    Game(); // initilize game variable
    ~Game();  // delete spices and free memory
    void drawBoard();
    void spawnPiece();
    void mergePiece();
    void clearLines();
    void processInput();
    void resetGame();
    void run();
};

Game::Game() : score(0), level(1), linesCleared(0), gameOver(false), paused(false), currentPiece(nullptr) {   // initilize game variable
    consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO cursorInfo;    // Hides the console cursor
    GetConsoleCursorInfo(consoleHandle, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);

    board.resize(HEIGHT, vector<char>(WIDTH, ' '));

    tetrominoes = {
        {{'I','I','I','I'}},
        {{'O','O'}, {'O','O'}},
        {{' ','T',' '}, {'T','T','T'}},
        {{' ','S','S'}, {'S','S',' '}},
        {{'Z','Z',' '}, {' ','Z','Z'}},
        {{'J',' ',' '}, {'J','J','J'}},
        {{' ',' ','L'}, {'L','L','L'}}
    };

    srand(static_cast<unsigned>(time(nullptr)));
    spawnPiece();
}

Game::~Game() {
    delete currentPiece;
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(consoleHandle, &cursorInfo);
    cursorInfo.bVisible = true;
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}

void Game::drawBoard() {
    COORD coord = {0, 0};
    SetConsoleCursorPosition(consoleHandle, coord);

    cout << "TETRIS - Score: " << score << " | Level: " << level << " | Lines: " << linesCleared << "    \n";
    cout << "Controls: ← → ↓ ↑ | Space: Hard Drop | ESC: Pause" << "    \n";

    vector<vector<char>> display = board;
    if (!gameOver && currentPiece) {
        for (size_t i = 0; i < currentPiece->shape.size(); i++) {
            for (size_t j = 0; j < currentPiece->shape[i].size(); j++) {
                if (currentPiece->shape[i][j] != ' ') {
                    int displayY = currentPiece->y + i;
                    int displayX = currentPiece->x + j;
                    if (displayY >= 0 && displayY < HEIGHT && displayX >= 0 && displayX < WIDTH) {
                        display[displayY][displayX] = currentPiece->shape[i][j];
                    }
                }
            }
        }
    }

    for (int i = 0; i < HEIGHT + 2; i++) {
        for (int j = 0; j < WIDTH + 2; j++) {
            if (i == 0 || i == HEIGHT + 1 || j == 0 || j == WIDTH + 1) {
                cout << '#';  // Border
            } else {
                char cell = display[i - 1][j - 1];
                cout << (cell == ' ' ? ' ' : cell); // No dots, just spaces for empty cells
            }
        }
        cout << "\n";
    }

    if (gameOver) {
        cout << "\nGAME OVER! Press R to Restart or Q to Quit\n";
    }
}


void Game::spawnPiece() {
    delete currentPiece;
    currentPiece = new Piece(tetrominoes[rand() % tetrominoes.size()]);

    if (currentPiece->checkCollision(currentPiece->x, currentPiece->y, board)) {
        gameOver = true;
    }
}

void Game::mergePiece() {
    for (size_t i = 0; i < currentPiece->shape.size(); i++) {
        for (size_t j = 0; j < currentPiece->shape[i].size(); j++) {
            if (currentPiece->shape[i][j] != ' ') {
                int boardY = currentPiece->y + i;
                int boardX = currentPiece->x + j;
                if (boardY >= 0 && boardY < HEIGHT && boardX >= 0 && boardX < WIDTH) {
                    board[boardY][boardX] = currentPiece->shape[i][j];
                }
            }
        }
    }

    score += 5;  // Increase score by 5 when a piece locks in place
}

void Game::clearLines() {
    for (int i = HEIGHT - 1; i >= 0; i--) {
        if (all_of(board[i].begin(), board[i].end(), [](char c) { return c != ' '; })) {
            board.erase(board.begin() + i);
            board.insert(board.begin(), vector<char>(WIDTH, ' '));
            score += 100;
            linesCleared++;
        }
    }
}

void Game::processInput() {
    if (_kbhit()) {
        int key = _getch();
        if (key == 224 || key == 0) { // Arrow keys
            key = _getch();
            switch (key) {
                case 75: // Left
                    if (!currentPiece->checkCollision(currentPiece->x - 1, currentPiece->y, board))
                        currentPiece->x--;
                    break;
                case 77: // Right
                    if (!currentPiece->checkCollision(currentPiece->x + 1, currentPiece->y, board))
                        currentPiece->x++;
                    break;
                case 80: // Down
                    if (!currentPiece->checkCollision(currentPiece->x, currentPiece->y + 1, board))
                        currentPiece->y++;
                    break;
                case 72: // Up
                    currentPiece->rotate(board);
                    break;
            }
        } else {
            switch (key) {
                case ' ': // Hard drop
                    while (!currentPiece->checkCollision(currentPiece->x, currentPiece->y + 1, board)) {
                        currentPiece->y++;
                        score++;
                    }
                    break;
                case 27: // ESC - pause
                    paused = true;
                    drawBoard();
                    cout << "PAUSED - Press ESC to continue";
                    while (paused && _getch() != 27) {}
                    paused = false;
                    break;
                case 'r': case 'R': // Restart
                    if (gameOver) resetGame();
                    break;
                case 'q': case 'Q': // Quit
                    gameOver = true;
                    break;
            }
        }
    }
}

void Game::resetGame() {
    board = vector<vector<char>>(HEIGHT, vector<char>(WIDTH, ' '));
    score = 0;
    level = 1;
    linesCleared = 0;
    gameOver = false;
    paused = false;
    spawnPiece();
}

void Game::run() {
    auto lastFall = GetTickCount();
    
    while (!gameOver) {
        auto currentTime = GetTickCount();
        int fallDelay = max(50, BASE_FALL_DELAY - level * 50);

        processInput();

        // Automatic falling
        if (currentTime - lastFall > fallDelay) {
            lastFall = currentTime;
            if (!currentPiece->checkCollision(currentPiece->x, currentPiece->y + 1, board)) {
                currentPiece->y++;
            } else {
                mergePiece();
                clearLines();
                spawnPiece();
            }
        }

        drawBoard();
        Sleep(10);
    }

    // Game over loop
    while (gameOver) {
        drawBoard();
        if (_kbhit()) {
            int key = _getch();
            if (key == 'r' || key == 'R') {
                resetGame();
                run();
                return;
            } else if (key == 'q' || key == 'Q') {
                return;
            }
        }
        Sleep(100);
    }
}

int main() {
    Game tetris;
    tetris.run();
    return 0;
}
