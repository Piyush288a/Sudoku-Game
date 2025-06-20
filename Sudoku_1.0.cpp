#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <string>
#include <chrono>  // only for timing
#include <algorithm>
#include <random>
#include <bitset>

using namespace std;

// Timer Class without threads
class Timer {
    int totalSeconds;
    chrono::steady_clock::time_point startTime;

public:
    Timer(int minutes) {
        totalSeconds = minutes * 60;
    }

    void start() {
        startTime = chrono::steady_clock::now();
    }

    bool isTimeUp() {
        auto now = chrono::steady_clock::now();
        int elapsed = chrono::duration_cast<chrono::seconds>(now - startTime).count();
        int remaining = totalSeconds - elapsed;

        if (remaining <= 0) {
            return true;
        }

        int minutes = remaining / 60;
        int seconds = remaining % 60;
        cout << "\rTime Left: " << minutes << "m " << seconds << "s   " << flush;
        return false;
    }
};

// Sudoku Class
class Sudoku {
    vector<vector<int>> board;
    vector<vector<int>> solution;
    int mistakes;

public:
    Sudoku() : board(9, vector<int>(9, 0)), solution(9, vector<int>(9, 0)), mistakes(0) {}

    void generateBoard(string difficulty, int level) {
        srand(time(0));
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                board[i][j] = rand() % 9 + 1;
                solution[i][j] = board[i][j];
            }
        }
    }

    void displayLargeSudoku() {
        cout << "\nYour Sudoku Board (in Binary 0/1 format):\n";
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (board[i][j] == 0)
                    cout << " " << setw(4) << ".";
                else
                    cout << " " << setw(4) << bitset<4>(board[i][j]);
            }
            cout << "\n";
        }
    }

    void playGame(Timer &timer) {
        int row, col, val;
        while (mistakes < 5) {
            if (timer.isTimeUp()) {
                cout << "\n\n⏰ Time's up! You lost the game!\n";
                return;
            }

            cout << "\nEnter row (1-9), column (1-9) and value (1-9): ";
            cin >> row >> col >> val;
            row--; col--;

            if (solution[row][col] == val) {
                cout << "✅ Correct!\n";
                board[row][col] = val;
            } else {
                cout << "❌ Wrong! Mistake #" << (mistakes + 1) << "\n";
                mistakes++;
            }

            if (mistakes >= 5) {
                cout << "\nToo many mistakes! Game over.\n";
                break;
            }
        }
    }
};

int main() {
    cout << "========== Welcome to Sudoku (Binary Format) ==========\n";
    cout << "Do you want to start the game? (y/n): ";
    char start;
    cin >> start;

    while (start == 'y' || start == 'Y') {
        cout << "Choose difficulty (easy/medium/hard): ";
        string difficulty;
        cin >> difficulty;

        int minutes = 0;
        if (difficulty == "easy") minutes = 10;
        else if (difficulty == "medium") minutes = 7;
        else if (difficulty == "hard") minutes = 5;
        else minutes = 10;

        cout << "Select level (1-10): ";
        int level;
        cin >> level;

        Sudoku game;
        game.generateBoard(difficulty, level);
        game.displayLargeSudoku();

        Timer timer(minutes);
        
        cout << "\nGame starts now!\n";
        timer.start();

        game.playGame(timer);

        cout << "\nDo you want to play again? (y/n): ";
        cin >> start;
    }

    cout << "Thank you for playing Sudoku! 🎉\n";
    return 0;
}

/*
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <iomanip> // for setw
#include <string>
#include <chrono> // for timer
#include <thread> // comment/remove this if compiler error
#include <algorithm> // for shuffle
#include <random> // random device
#include <bitset> // for binary printing
using namespace std;

// Timer Class
class Timer {
    int totalSeconds;
public:
    Timer(int minutes) {
        totalSeconds = minutes * 60;
    }

    void start() {
        auto start = chrono::steady_clock::now();
        while (true) {
            auto now = chrono::steady_clock::now();
            int elapsed = chrono::duration_cast<chrono::seconds>(now - start).count();
            int remaining = totalSeconds - elapsed;

            if (remaining <= 0) {
                cout << "\n\n⏰ Time's up! You lost the game!\n";
                break;
            }

            int minutes = remaining / 60;
            int seconds = remaining % 60;
            cout << "\rTime Left: " << minutes << "m " << seconds << "s   " << flush;

            // Simple wait
            for (volatile int i = 0; i < 10000000; ++i); // Simple delay
        }
    }
};

// Sudoku Class
class Sudoku {
    vector<vector<int>> board;
    vector<vector<int>> solution;
    int mistakes;

public:
    Sudoku() : board(9, vector<int>(9, 0)), solution(9, vector<int>(9, 0)), mistakes(0) {}

    void generateBoard(string difficulty, int level) {
        // Random Sudoku Generator (simple dummy for now)
        srand(time(0));
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                board[i][j] = rand() % 9 + 1;
                solution[i][j] = board[i][j];
            }
        }
    }

    void displayLargeSudoku() {
        cout << "\nYour Sudoku Board (in Binary Format):\n";
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (board[i][j] == 0)
                    cout << " " << setw(4) << ".";
                else
                    cout << " " << setw(4) << bitset<4>(board[i][j]);
            }
            cout << "\n";
        }
    }

    void playGame() {
        int row, col, val;
        while (mistakes < 5) {
            cout << "\nEnter row (1-9), column (1-9) and value (1-9): ";
            cin >> row >> col >> val;
            row--; col--;

            if (solution[row][col] == val) {
                cout << "✅ Correct!\n";
                board[row][col] = val;
            } else {
                cout << "❌ Wrong! Mistake #" << (mistakes + 1) << "\n";
                mistakes++;
            }

            if (mistakes >= 5) {
                cout << "\nToo many mistakes! Game over.\n";
                break;
            }
        }
    }
};

int main() {
    cout << "========== Welcome to Sudoku ==========\n";
    cout << "Do you want to start the game? (y/n): ";
    char start;
    cin >> start;

    while (start == 'y' || start == 'Y') {
        cout << "Choose difficulty (easy/medium/hard): ";
        string difficulty;
        cin >> difficulty;

        int minutes = 0;
        if (difficulty == "easy") minutes = 10;
        else if (difficulty == "medium") minutes = 7;
        else if (difficulty == "hard") minutes = 5;
        else minutes = 10;

        cout << "Select level (1-10): ";
        int level;
        cin >> level;

        Sudoku game;
        game.generateBoard(difficulty, level);
        game.displayLargeSudoku();

        Timer timer(minutes);
        
        cout << "\nGame starts now!\n";
        // Start Timer in simple way
        timer.start();

        game.playGame();

        cout << "\nDo you want to play again? (y/n): ";
        cin >> start;
    }

    cout << "Thank you for playing Sudoku! 🎉\n";
    return 0;
}
*/