#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<limits>
#include<chrono>  // For timer functionality
#include<thread>  // For sleep
#include<ctime>   // For time display
#include<iomanip> // For formatting time
using namespace std;

class Sudoku
{
    protected:
        string difficulty;
        int level;
        vector<vector<int>> original;  // To store original puzzle (clues)
        vector<vector<int>> current;   // Current state of the puzzle
        int mistakeCount;              // Count of mistakes
        int maxMistakes;               // Maximum allowed mistakes
        int timeLimit;                 // Time limit in seconds

    public:
        Sudoku() : mistakeCount(0), maxMistakes(5) {}
        Sudoku(string diff, int lvl) : difficulty(diff), level(lvl), mistakeCount(0), maxMistakes(5) {}
        
        virtual vector<vector<int>> getPuzzle() = 0;
        
        void setTimeLimit(int minutes) {
            timeLimit = minutes * 60;
        }
        
        int getMistakeCount() const {
            return mistakeCount;
        }
        
        int getMaxMistakes() const {
            return maxMistakes;
        }
        
        int getTimeLimit() const {
            return timeLimit;
        }
        
        string getDifficulty() const {
            return difficulty;
        }
        
        void incrementMistakeCount() {
            mistakeCount++;
        }
        
        // Save original and current puzzle state
        void initializeGame(const vector<vector<int>>& puzzle) {
            original = puzzle;
            current = puzzle;
        }
        
        // Check if a move is valid according to Sudoku rules
        bool isValidMove(int row, int col, int num) const {
            // Check if cell is editable (not part of original puzzle)
            if (original[row][col] != 0) {
                return false;
            }
            
            // Check row for duplicates
            for (int j = 0; j < 9; j++) {
                if (current[row][j] == num) return false;
            }
            
            // Check column for duplicates
            for (int i = 0; i < 9; i++) {
                if (current[i][col] == num) return false;
            }
            
            // Check 3x3 box for duplicates
            int boxRow = row - row % 3;
            int boxCol = col - col % 3;
            for (int i = boxRow; i < boxRow + 3; i++) 
            {
                for (int j = boxCol; j < boxCol + 3; j++) 
                {
                    if (current[i][j] == num) return false;
                }
            }
            
            return true;
        }
        
        // Make a move on the board
        bool makeMove(int row, int col, int num) {
            if (isValidMove(row-1, col-1, num)) {
                current[row-1][col-1] = num;
                return true;
            }
            return false;
        }
        
        // Check if the puzzle is fully solved
        bool isSolved() const {
            // Check for any unfilled cells
            for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 9; j++) {
                    if (current[i][j] == 0) return false;
                }
            }
            
            // Check all rows for validity
            for (int i = 0; i < 9; i++) {
                vector<bool> seen(10, false);
                for (int j = 0; j < 9; j++) {
                    int num = current[i][j];
                    if (num == 0 || seen[num]) return false;
                    seen[num] = true;
                }
            }
            
            // Check all columns for validity
            for (int j = 0; j < 9; j++) {
                vector<bool> seen(10, false);
                for (int i = 0; i < 9; i++) {
                    int num = current[i][j];
                    if (num == 0 || seen[num]) return false;
                    seen[num] = true;
                }
            }
            
            // Check all 3x3 boxes for validity
            for (int boxRow = 0; boxRow < 9; boxRow += 3) {
                for (int boxCol = 0; boxCol < 9; boxCol += 3) {
                    vector<bool> seen(10, false);
                    for (int i = 0; i < 3; i++) {
                        for (int j = 0; j < 3; j++) {
                            int num = current[boxRow + i][boxCol + j];
                            if (num == 0 || seen[num]) return false;
                            seen[num] = true;
                        }
                    }
                }
            }
            
            return true;  // Everything checks out, puzzle is solved
        }
        
        // Return current game board
        const vector<vector<int>>& getCurrentBoard() const {
            return current;
        }
        
        // Check if cell is part of original puzzle (a clue)
        bool isOriginalCell(int row, int col) const {
            return original[row][col] != 0;
        }
};

class Easy : public Sudoku
{
    private:
        vector<vector<vector<int>>> puzzles; 
    public:
        Easy(int lvl) : Sudoku("easy", lvl) 
        {
            loadPuzzles();
            setTimeLimit(10);  // 10 minutes for easy puzzles
        }
        
        void loadPuzzles()
        {
            ifstream file("easy.txt");
            if (!file) 
            {
                cout << "Error opening easy.txt\n";
                return;
            }
            for (int p = 0; p < 10; ++p) 
            {
                vector<vector<int>> board(9, vector<int>(9));
                for (int i = 0; i < 9; ++i)
                {
                    for (int j = 0; j < 9; ++j)
                        file >> board[i][j];
                }
                puzzles.push_back(board);
                file.ignore(numeric_limits<streamsize>::max(), '\n');
                while (file.peek() == '\n' || file.peek() == '\r')
                    file.ignore();
            }
            file.close();
        }
        
        vector<vector<int>> getPuzzle()
        {
            vector<vector<int>> puzzle = puzzles[level - 1];
            initializeGame(puzzle);
            return puzzle;
        }
};

class Medium : public Sudoku
{
    private:
        vector<vector<vector<int>>> puzzles; 
    public:
        Medium(int lvl) : Sudoku("medium", lvl) 
        {
            loadPuzzles();
            setTimeLimit(7);  // 7 minutes for medium puzzles
        }
        
        void loadPuzzles()
        {
            ifstream file("medium.txt");
            if (!file) 
            {
                cout << "Error opening medium.txt\n";
                return;
            }
            for (int p = 0; p < 10; ++p) 
            {
                vector<vector<int>> board(9, vector<int>(9));
                for (int i = 0; i < 9; ++i)
                {
                    for (int j = 0; j < 9; ++j)
                        file >> board[i][j];
                }
                puzzles.push_back(board);
                string el;
                getline(file, el);
            }
            file.close();
        }
        
        vector<vector<int>> getPuzzle()
        {
            vector<vector<int>> puzzle = puzzles[level - 1];
            initializeGame(puzzle);
            return puzzle;
        }
};

class Hard : public Sudoku
{
    private:
        vector<vector<vector<int>>> puzzles; 
    public:
        Hard(int lvl) : Sudoku("hard", lvl) 
        {
            loadPuzzles();
            setTimeLimit(5);  // 5 minutes for hard puzzles
        }
        
        void loadPuzzles()
        {
            ifstream file("hard.txt");
            if (!file) 
            {
                cout << "Error opening hard.txt\n";
                return;
            }
            for (int p = 0; p < 10; ++p) 
            {
                vector<vector<int>> board(9, vector<int>(9));
                for (int i = 0; i < 9; ++i)
                {
                    for (int j = 0; j < 9; ++j)
                        file >> board[i][j];
                }
                puzzles.push_back(board);
                string el;
                getline(file, el);
            }
            file.close();
        }
        
        vector<vector<int>> getPuzzle()
        {
            vector<vector<int>> puzzle = puzzles[level - 1];
            initializeGame(puzzle);
            return puzzle;
        }
};

// Display the Sudoku board with current state
void displayBoard(const vector<vector<int>>& board)
{
    cout << "\nYour Sudoku Board:\n";
    cout << "    1 2 3   4 5 6   7 8 9  \n";
    cout << "  +---------+---------+---------+\n";
    for (int i = 0; i < 9; ++i) 
    {
        cout << i+1 << " |";
        for (int j = 0; j < 9; ++j) 
        {
            if (board[i][j] == 0)
                cout << " . ";
            else
                cout << " " << board[i][j] << " ";
            
            if((j+1)%3==0 && j!=8) 
                cout << "|"; 
        }
        if((i+1)%3==0 && i!=8) 
            cout << "|\n  +---------+---------+---------+\n"; 
        else 
            cout << "|\n";
    }
    cout << "  +---------+---------+---------+\n";
}

// Function to display timer
void displayTime(int seconds) {
    int minutes = seconds / 60;
    int secs = seconds % 60;
    cout << "Time: " << setw(2) << setfill('0') << minutes << ":" 
         << setw(2) << setfill('0') << secs << "\r";
    cout.flush();
}

// Function to display game rules
void displayRules() {
    cout << "\n========== SUDOKU RULES ==========\n";
    cout << "1. Fill each row with numbers 1-9 without repetition\n";
    cout << "2. Fill each column with numbers 1-9 without repetition\n";
    cout << "3. Fill each 3x3 box with numbers 1-9 without repetition\n";
    cout << "4. You cannot change the given numbers (clues)\n";
    cout << "5. You have a limited time based on difficulty:\n";
    cout << "   - Easy: 10 minutes\n";
    cout << "   - Medium: 7 minutes\n";
    cout << "   - Hard: 5 minutes\n";
    cout << "6. You are allowed 5 mistakes maximum\n\n";
}

// Function to play the game
bool playGame(Sudoku* game) {
    vector<vector<int>> board = game->getPuzzle();
    
    cout << "\nGame started! You have ";
    string difficulty = game->getDifficulty();
    if (difficulty == "easy") cout << "10 minutes";
    else if (difficulty == "medium") cout << "7 minutes";
    else cout << "5 minutes";
    cout << " to solve the puzzle.\n";
    
    cout << "You're allowed " << game->getMaxMistakes() << " mistakes maximum.\n\n";
    
    // Display initial board
    displayBoard(board);
    
    // Start the timer
    auto startTime = chrono::steady_clock::now();
    int timeLimit = game->getTimeLimit();
    
    bool gameOver = false;
    bool solved = false;
    bool timeUp = false;
    
    while (!gameOver) {
        // Calculate elapsed time
        auto currentTime = chrono::steady_clock::now();
        int elapsedSeconds = chrono::duration_cast<chrono::seconds>(currentTime - startTime).count();
        int remainingSeconds = timeLimit - elapsedSeconds;
        
        if (remainingSeconds <= 0) {
            cout << "\nTime's up! Game over.\n";
            timeUp = true;
            gameOver = true;
            continue;
        }
        
        // Display time and mistake count
        displayTime(remainingSeconds);
        cout << "   Mistakes: " << game->getMistakeCount() << "/" << game->getMaxMistakes() << "\n";
        
        // Get user input
        int row, col, num;
        cout << "\nEnter row (1-9), column (1-9), and number (1-9) (or 0 0 0 to exit): ";
        cin >> row >> col >> num;
        
        // Exit option
        if (row == 0 && col == 0 && num == 0) {
            cout << "Exiting game...\n";
            return false;
        }
        
        // Validate input range
        if (row < 1 || row > 9 || col < 1 || col > 9 || num < 1 || num > 9) {
            cout << "Invalid input! Row, column, and number must be between 1 and 9.\n";
            continue;
        }
        
        // Check if trying to modify original clue
        if (game->isOriginalCell(row-1, col-1)) {
            cout << "Cannot modify original puzzle clues!\n";
            continue;
        }
        
        // Make the move
        if (game->makeMove(row, col, num)) {
            cout << "Move accepted.\n";
            
            // Get updated board and display it
            board = game->getCurrentBoard();
            displayBoard(board);
            
            // Check if puzzle is solved
            if (game->isSolved()) {
                cout << "\nCongratulations! You solved the puzzle!\n";
                solved = true;
                gameOver = true;
            }
        } else {
            cout << "Invalid move! The number violates Sudoku rules.\n";
            game->incrementMistakeCount();
            
            if (game->getMistakeCount() >= game->getMaxMistakes()) {
                cout << "\nYou've made " << game->getMistakeCount() << " mistakes. Game over!\n";
                gameOver = true;
            }
        }
    }
    
    if (solved) {
        cout << "You completed the puzzle successfully!\n";
        return true;
    } else if (timeUp) {
        cout << "You ran out of time. Better luck next time!\n";
        return false;
    } else {
        cout << "You made too many mistakes. Better luck next time!\n";
        return false;
    }
}

int main()
{
    cout << "\n========== Welcome to Sudoku ==========\n";
    displayRules();
    
    cout << "Want to start the game? (y/n): ";
    string start;
    cin >> start;
    
    Sudoku* game = nullptr;
    
    while (start == "y" || start == "Y" || start == "yes" || start == "Yes" || start == "YES") 
    {
        cout << "\nChoose difficulty (easy/medium/hard): ";
        string difficulty;
        cin >> difficulty;
        
        // Convert input to lowercase for easier comparison
        for (char& c : difficulty) c = tolower(c);
        
        if (difficulty != "easy" && difficulty != "medium" && difficulty != "hard") 
        {
            cout << "Invalid difficulty level.\n";
            continue;
        }
        
        cout << "Select level (1-10): ";
        int level;
        cin >> level;
        if (level < 1 || level > 10) 
        {
            cout << "Invalid level. Please select a level between 1 and 10.\n";
            continue;
        }
        
        // Create the appropriate game object
        if (difficulty == "easy") 
        {
            game = new Easy(level);
        } 
        else if (difficulty == "medium") 
        {
            game = new Medium(level);
        }
        else 
        {
            game = new Hard(level);
        }
        
        // Play the game
        bool wonGame = playGame(game);
        
        // Clean up
        delete game;
        game = nullptr;
        
        // Ask to play again
        cout << "\nDo you want to play again? (y/n): ";
        cin >> start;
    }
    
    cout << "\nThank you for playing Sudoku! 🎉\n";
    return 0;
}