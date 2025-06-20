/*
    GROUP 4

    NAME:Piyush Desai
    ROLL NO: CE069
    ID: 24CEUOS026

    NAME:Mihir Bhanvadiya
    Roll NO: CE061
    ID: 24CEUOS011
*/

/*
    
    CONCEPTS USED IN THE PROJECT

1. Hierarchical Inheritance: 
    The Easy, Medium, and Hard classes inherit from the Sudoku class. 
    They implement the getSudoku() method to provide specific puzzles for each difficulty level.
        class Easy   : public Sudoku { … };
        class Medium : public Sudoku { … };
        class Hard   : public Sudoku { … };

2. Abstract Classes and Pure Virtual Functions: 
    The Sudoku class is an abstract class with a pure virtual function getSudoku(). 
    This enforces that derived classes (Easy, Medium, Hard) must implement this function.
        virtual vector<vector<int>> getSudoku() = 0;

3. Polymorphism (Dynamic Binding): 
    The playGame function uses a pointer of type Sudoku to call methods on derived classes. 
    This allows for dynamic binding, where the correct method is called based on the actual object type at runtime.
    The function playGame(Sudoku* game) takes a pointer to the base class and calls derived class methods.
        Sudoku* game = nullptr;
        … 
        if (diff=="easy")   game = new Easy(lvl);
        else if (diff=="medium") game = new Medium(lvl);
        else game = new Hard(lvl);

        playGame(game);

4. Constructor and Initialization List: 
    The constructors of the Sudoku class and its derived classes use initialization lists to initialize member variables.
    For example, the Sudoku constructor initializes level, difficulty, and mistakeCount using an initialization list:
        Sudoku(string diff, int lvl, int mistake = 0) : level(lvl), difficulty(diff), mistakeCount(mistake) {};

5. Encapsulation and data protection (use of protected and const function): 
    The member variables of the Sudoku class are protected, allowing derived classes to access them while
    
6. Making our own header file:
    #define	Defines a symbolic constant or macro — in this case, a unique name.
    #ifndef	“If not defined” — checks if the symbol hasn’t been defined yet.
    #endif	Ends the conditional started by #ifndef.

7. File Handling
    ifstream file("easy.txt"); --> Opens the file and prepares to read puzzles line by line.
    file.ignore(numeric_limits<streamsize>::max(), '\n'); --> Clears the remaining part of the current line after reading a board o skip blank lines or garbage after one puzzle, before reading the next.
    file.close() --> Releases the file stream once reading is done



8. Timer <chrono>
    The <chrono> library is used to implement a timer for the game. 
    It tracks the time taken by the player to solve the Sudoku puzzle.
        auto startTime = steady_clock::now();  --> now() returns time_point form monotic clock of chrono::steady_clock
        int timePassed = duration_cast<seconds>(now - startTime).count();  --> count() returns numeric value of time in seconds
        int timeLeft = totalMinutes * 60 - timePassed;


*/

#include<iostream>
#include<string>
#include<vector>
#include<fstream>                          
#include<limits>                           
#include<chrono>                  //Header in C++ is part of the Standard Library and provides facilities for manipulating date and time
#include "ScrollEffect.h"         //Include user defined header     

using namespace std;
using namespace std::chrono;

// -------------------- SUDOKU CLASS ---------------------
class Sudoku
{
protected:
    string difficulty;
    int level;
    vector<vector<int>> original;
    vector<vector<int>> current;
    vector<vector<int>> previous;
    int mistakeCount;
public:
    Sudoku() {};                                                                                                  // Default Constructor 
    Sudoku(string diff, int lvl, int mistake = 0) : level(lvl), difficulty(diff), mistakeCount(mistake) {};       // Constructor with initialisation list 

    virtual vector<vector<int>> getSudoku() = 0;                 // Pure Virtual Method

    void initializeSudoku(const vector<vector<int>>& puzzle)
    {
        original = puzzle;                                    //default copy constructor
        current = puzzle;
        previous = puzzle;
        mistakeCount = 0;
    }
    bool undoMove()                                          // true - prevoius to current
    {                                                        // flase - no moves to undo
        if (current == previous)
            return false;
        current = previous;
        return true;
    }

    bool isOriginalCell(int row, int col) const              // true - change in ogiginal
    {                                                        // false - no change in original
        return original[row][col] != 0;
    }

    bool isValidMove(int row, int col, int num) const        // true - all check pass
    {                                                        // false - any check fails
        if (original[row][col] != 0)                         // Check if cell is editable (not part of original puzzle)
            return false;

        for (int j = 0; j < 9; j++)                          // Check row for duplicates
            if (current[row][j] == num) return false;

        for (int i = 0; i < 9; i++)                          // Check column for duplicates
            if (current[i][col] == num) return false;

        int boxRow = row - row % 3;                          // Check 3x3 box for duplicates
        int boxCol = col - col % 3;
        for (int i = boxRow; i < boxRow + 3; i++)            
            for (int j = boxCol; j < boxCol + 3; j++)
                if (current[i][j] == num) return false;

        return true;
    }

    bool makeMove(int row, int col, int num)
    {
        if (isValidMove(row - 1, col - 1, num))             // true - stores current in prevoius and edits current
        {
            previous = current;
            current[row - 1][col - 1] = num;
            return true;
        }
        return false;
    }

    bool isSolved() const
    {
        for (int i = 0; i < 9; i++)
            for (int j = 0; j < 9; j++)
                if (current[i][j] == 0)
                    return false;

        for (int i = 0; i < 9; i++)
        {
            vector<bool> seen(10, false);
            for (int j = 0; j < 9; j++)
            {
                int num = current[i][j];
                if (num == 0 || seen[num]) return false;
                seen[num] = true;
            }
        }

        for (int j = 0; j < 9; j++)
        {
            vector<bool> seen(10, false);
            for (int i = 0; i < 9; i++)
            {
                int num = current[i][j];
                if (num == 0 || seen[num]) return false;
                seen[num] = true;
            }
        }

        for (int boxRow = 0; boxRow < 9; boxRow += 3)
            for (int boxCol = 0; boxCol < 9; boxCol += 3)
            {
                vector<bool> seen(10, false);
                for (int i = 0; i < 3; i++)
                    for (int j = 0; j < 3; j++)
                    {
                        int num = current[boxRow + i][boxCol + j];
                        if (num == 0 || seen[num]) return false;
                        seen[num] = true;
                    }
            }
        return true;
    }

    // Getter Methods 
    string getDifficulty() const { return difficulty; }
    int getLevel() const { return level; }
    int getMistakeCount() const { return mistakeCount; }
    int increaseMistakeCount() { return ++mistakeCount; }
    const vector<vector<int>>& getCurrentBoard() const { return current; }
};

// ---------------- EASY, MEDIUM, HARD CLASSES --------------
class Easy : public Sudoku
{
    vector<vector<vector<int>>> puzzles;          
public:
    Easy(int lvl) : Sudoku("easy", lvl)           // Base class costructor in initialization list
    { 
        loadPuzzles(); 
    }      
    void loadPuzzles()                            // Loads 10 puzzles from file
    {
        ifstream file("easy.txt");
        for (int p = 0; p < 10; ++p)
        {
            vector<vector<int>> board(9, vector<int>(9));
            for (int i = 0; i < 9; ++i)
                for (int j = 0; j < 9; ++j)
                    file >> board[i][j];
            puzzles.push_back(board);
            file.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        file.close();
    }
    vector<vector<int>> getSudoku()                // Returns the puzzle of required level 
    {
        vector<vector<int>> puzzle = puzzles[level - 1];
        initializeSudoku(puzzle);
        return puzzle;
    }
};

class Medium : public Sudoku
{
    vector<vector<vector<int>>> puzzles;
public:
    Medium(int lvl) : Sudoku("medium", lvl) { loadPuzzles(); }
    void loadPuzzles()
    {
        ifstream file("medium.txt");
        for (int p = 0; p < 10; ++p)
        {
            vector<vector<int>> board(9, vector<int>(9));
            for (int i = 0; i < 9; ++i)
                for (int j = 0; j < 9; ++j)
                    file >> board[i][j];
            puzzles.push_back(board);
            string dummy;
            getline(file, dummy);
        }
        file.close();
    }
    vector<vector<int>> getSudoku()
    {
        vector<vector<int>> puzzle = puzzles[level - 1];
        initializeSudoku(puzzle);
        return puzzle;
    }
};

class Hard : public Sudoku
{
    vector<vector<vector<int>>> puzzles;
public:
    Hard(int lvl) : Sudoku("hard", lvl) { loadPuzzles(); }
    void loadPuzzles()
    {
        ifstream file("hard.txt");
        for (int p = 0; p < 10; ++p)
        {
            vector<vector<int>> board(9, vector<int>(9));
            for (int i = 0; i < 9; ++i)
                for (int j = 0; j < 9; ++j)
                    file >> board[i][j];
            puzzles.push_back(board);
            string dummy;
            getline(file, dummy);
        }
        file.close();
    }
    vector<vector<int>> getSudoku()
    {
        vector<vector<int>> puzzle = puzzles[level - 1];
        initializeSudoku(puzzle);
        return puzzle;
    }
};

// ---------------- DISPLAY FUNCTIONS ------------------------
void displayBoard(const vector<vector<int>>& board, Sudoku* game, int timeLeft)
{
    int minutes = timeLeft / 60;
    int seconds = timeLeft % 60;

    cout << "\nYour Sudoku Board:\n\n";
    cout << "     1  2  3   4  5  6   7  8  9       STATS\n";
    cout << "   +---------+---------+---------+     ---------------------------------------\n";

    for (int i = 0; i < 9; ++i)
    {
        cout << i + 1 << "  |";
        for (int j = 0; j < 9; ++j)
        {
            if (board[i][j] == 0)
                cout << " . ";
            else
                cout << " " << board[i][j] << " ";
            if ((j + 1) % 3 == 0 && j != 8)
                cout << "|";
        }
        if ((i + 1) % 3 == 0 && i != 8)
        {
            if (i == 2)
            {
                cout << "|     \n";
                cout << "   +---------+---------+---------+";
                cout << "     Mistakes : " << game->getMistakeCount() << "/5";
                printf("     Time Left : %02d:%02d\n", minutes, seconds);
            }
            else if (i == 5)
            {
                cout << "|     \n";
                cout << "   +---------+---------+---------+     ---------------------------------------\n";
            }
            else
            {
                cout << "|     \n";
                cout << "   +---------+---------+---------+     \n";
            }
        }
        else
        {
            cout << "|";
            if (i == 0) cout << "     Difficulty : " << game->getDifficulty() << "     Level : " << game->getLevel() << "\n";
            else if (i == 1) cout << "     ---------------------------------------\n";
            else if (i == 3) cout << "     Undo : -1 -1 -1\n";
            else if (i == 4) cout << "     Exit : 0 0 0\n";
            else cout << "\n";
        }
    }
    cout << "   +---------+---------+---------+\n";
}

void displayRules()
{
    cout << "\n=========== SUDOKU RULES ===========\n";
    cout << "1. Fill each row with numbers 1-9 without repetition\n";
    cout << "2. Fill each column with numbers 1-9 without repetition\n";
    cout << "3. Fill each 3x3 box with numbers 1-9 without repetition\n";
    cout << "4. You cannot change the given numbers (clues)\n";
    cout << "5. Limited time based on difficulty:\n";
    cout << "   - Easy: 15 mins | Medium: 13 mins | Hard: 11 mins\n";
    cout << "6. 5 mistakes allowed maximum\n\n";
}

// ---------------- GAME LOGIC --------------------------
bool playGame(Sudoku* game)                                  // Passing pointer of type Sudoku
{
    vector<vector<int>> board = game->getSudoku();
    int totalMinutes = (game->getDifficulty() == "easy") ? 15 : (game->getDifficulty() == "medium") ? 13 : 11;       // Use of ternary operator to set time limit

    auto startTime = steady_clock::now();                   // now() returns time_point form monotic clock of chrono::steady_clock

    cout << "\nGAME STARTS!\nYou have limited time and 5 mistakes allowed.\n";
    displayBoard(board, game, totalMinutes * 60);

    bool gameOver = false;

    while (!gameOver)
    {
        auto now = steady_clock::now();
        int timePassed = duration_cast<seconds>(now - startTime).count();
        int timeLeft = totalMinutes * 60 - timePassed;

        if (timeLeft <= 0)
        {
            cout << "\nTime's up! Game Over.\n";
            return true;
        }

        int row, col, num;
        cout << "\nEnter row, column, number (or -1 -1 -1 for undo or 0 0 0 to quit): ";
        cin >> row >> col >> num;

        if (row == 0 && col == 0 && num == 0)
        {
            cout << "Exiting...\n";
            return true;
        }

        if (row == -1 && col == -1 && num == -1)
        {
            if (game->undoMove())
                cout << "Move undone successfully.\n";
            else
                cout << "No moves to undo.\n";
            displayBoard(game->getCurrentBoard(), game, timeLeft);
            continue;
        }

        if (row < 1 || row > 9 || col < 1 || col > 9 || num < 1 || num > 9)
        {
            cout << "Invalid input! Must be 1-9.\n";
            continue;
        }

        if (game->isOriginalCell(row - 1, col - 1))
        {
            cout << "Cannot modify original clue!\n";
            continue;
        }

        if (game->makeMove(row, col, num))
        {
            cout << "Move accepted!\n";
            displayBoard(game->getCurrentBoard(), game, timeLeft);

            if (game->isSolved())
            {
                cout << "\nCongratulations! You solved the Sudoku!\nWell played! ";
                cout << "Time taken: " << timePassed / 60 << " minutes\n";
                gameOver = true;
            }
        }
        else
        {
            cout << "Invalid move! Mistakes: " << game->increaseMistakeCount() << "/5\n";
            if (game->getMistakeCount() >= 5)
            {
                cout << "Too many mistakes! Game Over.\nBetter luck next time!\n";
                return true;
            }
        }
    }
    return true;
}

// ---------------- MAIN FUNCTION --------------------------
int main()
{
    scrollSudoku(50, 10);

    cout << "\n========== WELCOME TO SUDOKU ==========\n";
    displayRules();
    cout << "Start Game? (yes/no): ";
    string choice;
    cin >> choice;

    // Dynamically allocated object of type Sudoku
    Sudoku* game = nullptr;                                            // Static Binding of game : Sudoku           
                                                                    
    while (choice == "yes" || choice == "y" || choice == "Y")
    {
        cout << "\nChoose difficulty (easy/medium/hard): ";
        string diff;
        cin >> diff;
        if(diff != "easy" && diff != "medium" && diff != "hard") 
        {
            cout << "Invalid difficulty level. Try again\n";
            continue; 
        }


        cout << "Choose level (1-10): ";
        int lvl;
        cin >> lvl;
        if(lvl < 1 || lvl > 10) 
        {
            cout << "Invalid level. Please select a level between 1 and 10.\n";
            continue; 
        }

        if (diff == "easy")                       
            game = new Easy(lvl);                        // Dynamic Binding of game : Easy
        else if (diff == "medium")
            game = new Medium(lvl);                      // Dynamic Binding of game : Medium
        else if (diff == "hard")
            game = new Hard(lvl);                        // Dynamic Binding of game : Hard
        else
        {
            cout << "Invalid difficulty.\n";
            continue;
        }

        playGame(game);

        delete game;                                     // Freeing memory allocated for game
        cout << "\nWant to play again? (yes/no): ";
        cin >> choice;
    }

    cout << "Thank you for playing Sudoku! 🎉\n";
}

