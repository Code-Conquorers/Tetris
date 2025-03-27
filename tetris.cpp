#include <iostream>
#include <windows.h>
#include <conio.h>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

const int WIDTH = 10;
const int HEIGHT = 20;

class Piece {
public:
    int x, y;
    vector<vector<char>> shape;

    Piece(vector<vector<char>> shape) {
        this->shape = shape;
        x = WIDTH / 2 - 1;
        y = 0;
    }

    bool checkCollision(int newX, int newY, vector<vector<char>>& board) {
        for (int i = 0; i < shape.size(); i++) {
            for (int j = 0; j < shape[i].size(); j++) {
                if (shape[i][j] != ' ') {
                    int boardX = newX + j;
                    int boardY = newY + i;
                    if (boardX < 0 || boardX >= WIDTH || boardY >= HEIGHT || 
                        (boardY >= 0 && board[boardY][boardX] != ' '))
                        return true;
                }
            }
        }
        return false;
    }

    void rotate(vector<vector<char>>& board) {
        int size = shape.size();
        vector<vector<char>> rotated(size, vector<char>(size, ' '));

        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                rotated[j][size - 1 - i] = shape[i][j];

        if (!checkCollision(x, y, board))
            shape = rotated;
    }
};

class Game {
private:
    vector<vector<char>> board;
    vector<vector<vector<char>>> tetrominoes;
    Piece* currentPiece;
    int score;
    bool gameOver;
    bool paused;

public:
    Game() {
        board = vector<vector<char>>(HEIGHT, vector<char>(WIDTH, ' '));
        score = 0;
        gameOver = false;
        paused = false;

        tetrominoes = {
            {{' ', 'I', ' ', ' '}, {' ', 'I', ' ', ' '}, {' ', 'I', ' ', ' '}, {' ', 'I', ' ', ' '}}, // I
            {{'O', 'O'}, {'O', 'O'}}, // O
            {{' ', 'T', ' '}, {'T', 'T', 'T'}, {' ', ' ', ' '}}, // T
            {{' ', 'S', 'S'}, {'S', 'S', ' '}}, // S
            {{'Z', 'Z', ' '}, {' ', 'Z', 'Z'}}, // Z
            {{'J', ' ', ' '}, {'J', 'J', 'J'}}, // J
            {{' ', ' ', 'L'}, {'L', 'L', 'L'}}  // L
        };

        srand(time(0));
        spawnPiece();
    }

    void drawBoard() {
        system("cls");
        cout << "Tetris - Score: " << score << endl;
        cout << "← Move Left | → Move Right | ↑ Rotate | ↓ Soft Drop | Space Hard Drop | ESC Pause/Quit" << endl;

        vector<vector<char>> display = board;
        for (int i = 0; i < currentPiece->shape.size(); i++)
            for (int j = 0; j < currentPiece->shape[i].size(); j++)
                if (currentPiece->shape[i][j] != ' ')
                    display[currentPiece->y + i][currentPiece->x + j] = currentPiece->shape[i][j];

        for (int i = 0; i < HEIGHT + 2; i++) {
            for (int j = 0; j < WIDTH + 2; j++) {
                if (i == 0 || i == HEIGHT + 1 || j == 0 || j == WIDTH + 1)
                    cout << '#';
                else
                    cout << display[i - 1][j - 1];
            }
            cout << endl;
        }
    }

    void spawnPiece() {
        if (currentPiece != nullptr)
            delete currentPiece;

        currentPiece = new Piece(tetrominoes[rand() % tetrominoes.size()]);

        if (currentPiece->checkCollision(currentPiece->x, currentPiece->y, board))
            gameOver = true;
    }

    void mergePiece() {
        for (int i = 0; i < currentPiece->shape.size(); i++)
            for (int j = 0; j < currentPiece->shape[i].size(); j++)
                if (currentPiece->shape[i][j] != ' ')
                    board[currentPiece->y + i][currentPiece->x + j] = currentPiece->shape[i][j];
    }

    void clearLines() {
        for (int i = HEIGHT - 1; i >= 0; i--) {
            bool full = true;
            for (int j = 0; j < WIDTH; j++)
                if (board[i][j] == ' ')
                    full = false;

            if (full) {
                board.erase(board.begin() + i);
                board.insert(board.begin(), vector<char>(WIDTH, ' '));
                score += 100;
            }
        }
    }

    void processInput() {
        if (GetAsyncKeyState(VK_LEFT)) {
            if (!currentPiece->checkCollision(currentPiece->x - 1, currentPiece->y, board))
                currentPiece->x--;
        }
        if (GetAsyncKeyState(VK_RIGHT)) {
            if (!currentPiece->checkCollision(currentPiece->x + 1, currentPiece->y, board))
                currentPiece->x++;
        }
        if (GetAsyncKeyState(VK_DOWN)) {
            if (!currentPiece->checkCollision(currentPiece->x, currentPiece->y + 1, board))
                currentPiece->y++;
        }
        if (GetAsyncKeyState(VK_UP)) {
            currentPiece->rotate(board);
        }
        if (GetAsyncKeyState(VK_SPACE)) {
            while (!currentPiece->checkCollision(currentPiece->x, currentPiece->y + 1, board))
                currentPiece->y++;
        }
        if (GetAsyncKeyState(VK_ESCAPE)) {
            paused = !paused;
            if (paused) {
                system("cls");
                cout << "Game Paused! Press ESC to resume." << endl;
                while (paused) {
                    if (GetAsyncKeyState(VK_ESCAPE)) {
                        paused = false;
                        break;
                    }
                    Sleep(100);
                }
            }
        }
    }

    void run() {
        while (!gameOver) {
            drawBoard();
            processInput();

            if (!currentPiece->checkCollision(currentPiece->x, currentPiece->y + 1, board))
                currentPiece->y++;
            else {
                mergePiece();
                clearLines();
                spawnPiece();
            }

            Sleep(100);
        }

        system("cls");
        cout << "Game Over! Final Score: " << score << endl;
    }
};

int main() {
    Game tetris;
    tetris.run();
    return 0;
}
