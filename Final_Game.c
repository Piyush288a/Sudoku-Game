#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

// Forward declaration for scroll effect function
void scrollSudoku(int speed, int repeats);

// Structure to represent a Sudoku board
typedef struct {
    char difficulty[10];
    int level;
    int board[9][9];         // Original board
    int current[9][9];       // Current state
    int previous[9][9];      // Previous state for undo
    int mistakeCount;
} Sudoku;

// Function prototypes
void initializeSudoku(Sudoku* sudoku, int puzzle[9][9]);
bool undoMove(Sudoku* sudoku);
bool isOriginalCell(Sudoku* sudoku, int row, int col);
bool isValidMove(Sudoku* sudoku, int row, int col, int num);
bool makeMove(Sudoku* sudoku, int row, int col, int num);
bool isSolved(Sudoku* sudoku);
void loadPuzzle(Sudoku* sudoku, const char* filename);
void displayBoard(Sudoku* sudoku, int timeLeft);
void displayRules();
bool playGame(Sudoku* sudoku);

// File loading functions
void loadEasyPuzzle(Sudoku* sudoku) {
    strcpy(sudoku->difficulty, "easy");
    loadPuzzle(sudoku, "easy.txt");
}

void loadMediumPuzzle(Sudoku* sudoku) {
    strcpy(sudoku->difficulty, "medium");
    loadPuzzle(sudoku, "medium.txt");
}

void loadHardPuzzle(Sudoku* sudoku) {
    strcpy(sudoku->difficulty, "hard");
    loadPuzzle(sudoku, "hard.txt");
}

// Scroll effect implementation
void scrollSudoku(int speed, int repeats) {
    const char* frames[] = {
        "S     ",
        "SU    ",
        "SUD   ",
        "SUDO  ",
        "SUDOK ",
        "SUDOKU"
    };
    
    int numFrames = sizeof(frames) / sizeof(frames[0]);
    
    for (int i = 0; i < repeats; i++) {
        for (int j = 0; j < numFrames; j++) {
            printf("\r%s", frames[j]);
            fflush(stdout);
            
            // Sleep for specified milliseconds
            struct timespec ts;
            ts.tv_sec = speed / 1000;
            ts.tv_nsec = (speed % 1000) * 1000000;
            nanosleep(&ts, NULL);
        }
    }
    printf("\n");
}

// Initialize Sudoku board
void initializeSudoku(Sudoku* sudoku, int puzzle[9][9]) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            sudoku->board[i][j] = puzzle[i][j];
            sudoku->current[i][j] = puzzle[i][j];
            sudoku->previous[i][j] = puzzle[i][j];
        }
    }
    sudoku->mistakeCount = 0;
}

// Undo last move
bool undoMove(Sudoku* sudoku) {
    bool different = false;
    
    for (int i = 0; i < 9 && !different; i++) {
        for (int j = 0; j < 9 && !different; j++) {
            if (sudoku->current[i][j] != sudoku->previous[i][j]) {
                different = true;
            }
        }
    }
    
    if (!different) {
        return false;
    }
    
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            sudoku->current[i][j] = sudoku->previous[i][j];
        }
    }
    
    return true;
}

// Check if cell is from original puzzle
bool isOriginalCell(Sudoku* sudoku, int row, int col) {
    return sudoku->board[row][col] != 0;
}

// Check if a move is valid
bool isValidMove(Sudoku* sudoku, int row, int col, int num) {
    if (sudoku->board[row][col] != 0) {
        return false;
    }
    
    // Check row
    for (int j = 0; j < 9; j++) {
        if (sudoku->current[row][j] == num) {
            return false;
        }
    }
    
    // Check column
    for (int i = 0; i < 9; i++) {
        if (sudoku->current[i][col] == num) {
            return false;
        }
    }
    
    // Check 3x3 box
    int boxRow = row - row % 3;
    int boxCol = col - col % 3;
    
    for (int i = boxRow; i < boxRow + 3; i++) {
        for (int j = boxCol; j < boxCol + 3; j++) {
            if (sudoku->current[i][j] == num) {
                return false;
            }
        }
    }
    
    return true;
}

// Make a move
bool makeMove(Sudoku* sudoku, int row, int col, int num) {
    if (isValidMove(sudoku, row - 1, col - 1, num)) {
        // Save current state to previous
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                sudoku->previous[i][j] = sudoku->current[i][j];
            }
        }
        
        sudoku->current[row - 1][col - 1] = num;
        return true;
    }
    return false;
}

// Check if the puzzle is solved
bool isSolved(Sudoku* sudoku) {
    // Check for empty cells
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (sudoku->current[i][j] == 0) {
                return false;
            }
        }
    }
    
    // Check rows
    for (int i = 0; i < 9; i++) {
        bool seen[10] = {false};
        for (int j = 0; j < 9; j++) {
            int num = sudoku->current[i][j];
            if (num == 0 || seen[num]) {
                return false;
            }
            seen[num] = true;
        }
    }
    
    // Check columns
    for (int j = 0; j < 9; j++) {
        bool seen[10] = {false};
        for (int i = 0; i < 9; i++) {
            int num = sudoku->current[i][j];
            if (num == 0 || seen[num]) {
                return false;
            }
            seen[num] = true;
        }
    }
    
    // Check 3x3 boxes
    for (int boxRow = 0; boxRow < 9; boxRow += 3) {
        for (int boxCol = 0; boxCol < 9; boxCol += 3) {
            bool seen[10] = {false};
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    int num = sudoku->current[boxRow + i][boxCol + j];
                    if (num == 0 || seen[num]) {
                        return false;
                    }
                    seen[num] = true;
                }
            }
        }
    }
    
    return true;
}

// Load puzzle from file
void loadPuzzle(Sudoku* sudoku, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error opening puzzle file: %s\n", filename);
        exit(1);
    }
    
    int puzzle[9][9];
    char line[100];
    
    // Skip to the appropriate level
    for (int p = 0; p < sudoku->level - 1; p++) {
        for (int i = 0; i < 9; i++) {
            fgets(line, sizeof(line), file);
        }
        fgets(line, sizeof(line), file);  // Skip empty line
    }
    
    // Read the puzzle
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (fscanf(file, "%d", &puzzle[i][j]) != 1) {
                printf("Error reading puzzle from file\n");
                fclose(file);
                exit(1);
            }
        }
    }
    
    fclose(file);
    initializeSudoku(sudoku, puzzle);
}

// Display the Sudoku board
void displayBoard(Sudoku* sudoku, int timeLeft) {
    int minutes = timeLeft / 60;
    int seconds = timeLeft % 60;
    
    printf("\nYour Sudoku Board:\n\n");
    printf("     1  2  3   4  5  6   7  8  9       STATS\n");
    printf("   +---------+---------+---------+     ---------------------------------------\n");
    
    for (int i = 0; i < 9; i++) {
        printf("%d  |", i + 1);
        for (int j = 0; j < 9; j++) {
            if (sudoku->current[i][j] == 0) {
                printf(" . ");
            } else {
                printf(" %d ", sudoku->current[i][j]);
            }
            if ((j + 1) % 3 == 0 && j != 8) {
                printf("|");
            }
        }
        
        if ((i + 1) % 3 == 0 && i != 8) {
            if (i == 2) {
                printf("|     \n");
                printf("   +---------+---------+---------+");
                printf("     Mistakes : %d/5", sudoku->mistakeCount);
                printf("     Time Left : %02d:%02d\n", minutes, seconds);
            } else if (i == 5) {
                printf("|     \n");
                printf("   +---------+---------+---------+     ---------------------------------------\n");
            } else {
                printf("|     \n");
                printf("   +---------+---------+---------+     \n");
            }
        } else {
            printf("|");
            if (i == 0) {
                printf("     Difficulty : %s     Level : %d\n", sudoku->difficulty, sudoku->level);
            } else if (i == 1) {
                printf("     ---------------------------------------\n");
            } else if (i == 3) {
                printf("     Undo : -1 -1 -1\n");
            } else if (i == 4) {
                printf("     Exit : 0 0 0\n");
            } else {
                printf("\n");
            }
        }
    }
    printf("   +---------+---------+---------+\n");
}

// Display game rules
void displayRules() {
    printf("\n=========== SUDOKU RULES ===========\n");
    printf("1. Fill each row with numbers 1-9 without repetition\n");
    printf("2. Fill each column with numbers 1-9 without repetition\n");
    printf("3. Fill each 3x3 box with numbers 1-9 without repetition\n");
    printf("4. You cannot change the given numbers (clues)\n");
    printf("5. Limited time based on difficulty:\n");
    printf("   - Easy: 15 mins | Medium: 13 mins | Hard: 11 mins\n");
    printf("6. 5 mistakes allowed maximum\n\n");
}

// Main game logic
bool playGame(Sudoku* sudoku) {
    int totalMinutes;
    
    if (strcmp(sudoku->difficulty, "easy") == 0) {
        totalMinutes = 15;
    } else if (strcmp(sudoku->difficulty, "medium") == 0) {
        totalMinutes = 13;
    } else {
        totalMinutes = 11;
    }
    
    time_t startTime = time(NULL);
    
    printf("\nGAME STARTS!\nYou have limited time and 5 mistakes allowed.\n");
    displayBoard(sudoku, totalMinutes * 60);
    
    bool gameOver = false;
    
    while (!gameOver) {
        time_t now = time(NULL);
        int timePassed = (int)(now - startTime);
        int timeLeft = totalMinutes * 60 - timePassed;
        
        if (timeLeft <= 0) {
            printf("\nTime's up! Game Over.\n");
            return true;
        }
        
        int row, col, num;
        printf("\nEnter row, column, number (or -1 -1 -1 for undo or 0 0 0 to quit): ");
        scanf("%d %d %d", &row, &col, &num);
        
        if (row == 0 && col == 0 && num == 0) {
            printf("Exiting...\n");
            return true;
        }
        
        if (row == -1 && col == -1 && num == -1) {
            if (undoMove(sudoku)) {
                printf("Move undone successfully.\n");
            } else {
                printf("No moves to undo.\n");
            }
            displayBoard(sudoku, timeLeft);
            continue;
        }
        
        if (row < 1 || row > 9 || col < 1 || col > 9 || num < 1 || num > 9) {
            printf("Invalid input! Must be 1-9.\n");
            continue;
        }
        
        if (isOriginalCell(sudoku, row - 1, col - 1)) {
            printf("Cannot modify original clue!\n");
            continue;
        }
        
        if (makeMove(sudoku, row, col, num)) {
            printf("Move accepted!\n");
            displayBoard(sudoku, timeLeft);
            
            if (isSolved(sudoku)) {
                printf("\nCongratulations! You solved the Sudoku!\nWell played! ");
                printf("Time taken: %d minutes\n", (totalMinutes * 60 - timeLeft) / 60);
                gameOver = true;
            }
        } else {
            sudoku->mistakeCount++;
            printf("Invalid move! Mistakes: %d/5\n", sudoku->mistakeCount);
            if (sudoku->mistakeCount >= 5) {
                printf("Too many mistakes! Game Over.\nBetter luck next time!\n");
                return true;
            }
        }
    }
    return true;
}

// Main function
int main() {
    scrollSudoku(500, 1);
    
    printf("\n========== WELCOME TO SUDOKU ==========\n");
    displayRules();
    printf("Start Game? (yes/no): ");
    
    char choice[10];
    scanf("%s", choice);
    
    Sudoku sudoku;
    
    while (strcmp(choice, "yes") == 0 || strcmp(choice, "y") == 0 || strcmp(choice, "Y") == 0) {
        char diff[10];
        printf("\nChoose difficulty (easy/medium/hard): ");
        scanf("%s", diff);
        
        int level;
        printf("Choose level (1-10): ");
        scanf("%d", &level);
        
        // Added error handling for invalid level
        if (level < 1 || level > 10) {
            printf("Invalid level! Please choose a level between 1 and 10.\n");
            continue;
        }
        
        sudoku.level = level;
        
        if (strcmp(diff, "easy") == 0) {
            loadEasyPuzzle(&sudoku);
        } else if (strcmp(diff, "medium") == 0) {
            loadMediumPuzzle(&sudoku);
        } else if (strcmp(diff, "hard") == 0) {
            loadHardPuzzle(&sudoku);
        } else {
            printf("Invalid difficulty.\n");
            continue;
        }
        
        playGame(&sudoku);
        
        printf("\nPlay again? (yes/no): ");
        scanf("%s", choice);
    }
    
    printf("Thank you for playing Sudoku! 🎉\n");
    
    return 0;
}