#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<limits>
#include <chrono>
#include "ScrollEffect.h"

using namespace std;

class Sudoku
{
    protected:
        string difficulty;
        int level;
        vector<vector<int>> original;  // To store original puzzle (clues)
        vector<vector<int>> current;   // Current state of the puzzle
        vector<vector<int>> previous;  // New board to store last move
        int mistakeCount;              // Count of mistakes
    public:
        Sudoku(){};
        Sudoku(string diff,int lvl,int mistake=0):level(lvl),difficulty(diff),mistakeCount(mistake){};

        // Save original and current puzzle state
        void initializeSudoku(const vector<vector<int>>& puzzle) 
        {
            original = puzzle;
            current = puzzle;
            previous = puzzle; 
            mistakeCount=0;
        }
        bool undoMove() 
        {
            if(current==previous)
            {
                return false;
            }
            current = previous;  // Restore previous board
            return true;
            
        }

        virtual vector<vector<int>> getSudoku() = 0;

        // Check if cell is part of original puzzle (a clue) : 
        bool isOriginalCell(int row, int col) const 
        {
            return original[row][col] != 0;
        }

        // Check if a move is valid according to Sudoku rules
        bool isValidMove(int row, int col, int num) const 
        {
            // Check if cell is editable (not part of original puzzle)
            if (original[row][col] != 0) 
            {
                return false;
            }
            
            // Check row for duplicates
            for (int j = 0; j < 9; j++) 
            {
                if (current[row][j] == num) return false;
            }
            
            // Check column for duplicates
            for (int i = 0; i < 9; i++) 
            {
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
        bool makeMove(int row, int col, int num) 
        {
            if (isValidMove(row-1, col-1, num)) 
            {
                previous=current;
                current[row-1][col-1] = num;
                return true;
            }
            return false;
        }
        
        // Check if the puzzle is fully solved
        bool isSolved() const 
        {
            // Check for any unfilled cells
            for (int i = 0; i < 9; i++) 
            {
                for (int j = 0; j < 9; j++) 
                {
                    if (current[i][j] == 0) 
                        return false;
                }
            }
            
            // Check all rows for validity
            for (int i = 0; i < 9; i++) 
            {
                vector<bool> seen(10, false);
                for (int j = 0; j < 9; j++) 
                {
                    int num = current[i][j];
                    if (num == 0 || seen[num]) 
                        return false;
                    seen[num] = true;
                }
            }
            
            // Check all columns for validity
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
            
            // Check all 3x3 boxes for validity
            for (int boxRow = 0; boxRow < 9; boxRow += 3)
            {
                for (int boxCol = 0; boxCol < 9; boxCol += 3) 
                {
                    vector<bool> seen(10, false);
                    for (int i = 0; i < 3; i++) 
                    {
                        for (int j = 0; j < 3; j++)
                        {
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
        const vector<vector<int>>& getCurrentBoard() const 
        {
            return current;
        }
        int getMistakeCount() const 
        {
            return mistakeCount;
        }
        int increaseMistakeCount() 
        {
            return ++mistakeCount;
        }
        string getDifficulty() const {
            return difficulty;
        }
        int getLevel() const 
        {
            return level;
        }
        
         
        
};
class Easy:public Sudoku
{
    private:
        vector<vector<vector<int>>> puzzles; 
    public:
        Easy(int lvl) : Sudoku("easy",lvl) 
        {
            loadPuzzles();
        }
        void loadPuzzles()        //LOADS ALL 10 PROBLEMS IN EASY LEVEL AND STORES IN 3D VECTOR puzzles
        {
            ifstream file("easy.txt");
            if (!file) 
            {
                std::cout << "Error opening easy.txt\n";
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
                // after reading one puzzle, skip the rest of the current line
                file.ignore(numeric_limits<streamsize>::max(), '\n');
                // and skip any blank lines between puzzles
                while (file.peek() == '\n' || file.peek() == '\r')
                file.ignore();
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
class Medium:public Sudoku
{
    private:
        vector<vector<vector<int>>> puzzles; 
    public:
        Medium(int lvl) : Sudoku("medium",lvl) 
        {
            loadPuzzles();
        }
        void loadPuzzles()        //LOADS ALL 10 PROBLEMS IN EASY LEVEL AND STORES IN 3D VECTOR puzzles
        {
            ifstream file("medium.txt");
            if (!file) 
            {
                std::cout << "Error opening medium.txt\n";
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
        vector<vector<int>> getSudoku()   //RETURNS THE PUZZLE OF REQUIRED LEVEL TO board
        {
            return puzzles[level - 1];
        }
};
class Hard:public Sudoku
{
    private:
        vector<vector<vector<int>>> puzzles; 
    public:
        Hard(int lvl) : Sudoku("Hard",lvl) 
        {
            loadPuzzles();
        }
        void loadPuzzles()        //LOADS ALL 10 PROBLEMS IN EASY LEVEL AND STORES IN 3D VECTOR puzzles
        {
            ifstream file("hard.txt");
            if (!file) 
            {
                std::cout << "Error opening hard.txt\n";
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
        vector<vector<int>> getSudoku()   //RETURNS THE PUZZLE OF REQUIRED LEVEL TO board
        {
            return puzzles[level - 1];
        }
};


void displayBoard(const vector<vector<int>>& board,Sudoku* game) //DISPLAY THE BOARD
{
    std::cout << "\nYour Sudoku Board:\n\n";
    std::cout << "     1  2  3   4  5  6   7  8  9       STATS\n";
    std::cout << "   +---------+---------+---------+     ---------------------------------------\n";
    for (int i = 0; i < 9; ++i) 
    {
        std::cout << i+1 << "  |";
        for (int j = 0; j < 9; ++j) 
        {
            if (board[i][j] == 0)
                std::cout << " . ";
            else
                std::cout << " " << board[i][j] << " ";
            if((j+1)%3==0 && j!=8) 
                std::cout << "|"; 
            
        }
        if((i+1)%3==0 && i!=8) 
        {
            if(i==2)
            {
                std::cout << "|     "<<endl;
                std::cout <<"   +---------+---------+---------+"; 
                std::cout << "     Mistakes : " << game->getMistakeCount() << "/5"; 
                std::cout << "     Time Left : " << game->getLevel() * 2 << " minutes" << std::endl;
            }
            else if (i==5)
            {
                std::cout << "|     "<<endl;
                std::cout <<"   +---------+---------+---------+"; 
                std::cout<<"     ---------------------------------------"<<endl;
                
            }
            else
            {
                std::cout << "|     "<<endl;
                std::cout <<"   +---------+---------+---------+     "<<endl; 
            }
        }
        else 
        {
            std::cout<<"|";
            if (i == 0) 
            {
                std::cout << "     Difficulty : " <<game->getDifficulty()<<"     Level : "<<game->getLevel()<<std::endl;
            } 
            else if (i == 1) 
            {
                std::cout << "     ---------------------------------------\n";
            } 
            else if (i == 3) 
            {
                std::cout << "     Undo : -1 -1 -1"<<std::endl;
            } 
            else if (i == 4) 
            {
                std::cout << "     Exit : 0 0 0"<<std::endl;
            }
            
            
            else
            {
                std::cout<<endl;
            }
            
        }
    }
    std::cout<<"   +---------+---------+---------+"<<endl;
}

// FUNCTION TO DISPLAY GAME RULES
void displayRules() {
    std::cout << "\n=========== SUDOKU RULES ===========\n";
    std::cout << "1. Fill each row with numbers 1-9 without repetition\n";
    std::cout << "2. Fill each column with numbers 1-9 without repetition\n";
    std::cout << "3. Fill each 3x3 box with numbers 1-9 without repetition\n";
    std::cout << "4. You cannot change the given numbers (clues)\n";
    std::cout << "5. You have a limited time based on difficulty:\n";
    std::cout << "   - Easy: 15 minutes\n";
    std::cout << "   - Medium: 13 minutes\n";
    std::cout << "   - Hard: 11 minutes\n";
    std::cout << "6. You are allowed 5 mistakes maximum\n\n";
}

bool playGame(Sudoku *game)
{
    
    vector<vector<int>> board=game->getSudoku();
    std::cout<<"\n\nGAME STARTS! You have limited time to solve the sudoku puzzle.\n";
    std::cout<<"You are allowed 5 mistakes maximum.\n";
    displayBoard(board,game);

    using namespace std::chrono;
    auto startTime = steady_clock::now();
    int totalMinutes = 0;
    if (game->getDifficulty() == "easy") 
        totalMinutes = 15;
    else if (game->getDifficulty() == "medium")
        totalMinutes = 13;
    else 
        totalMinutes = 11;

    bool gameOver=false;
    bool solved=false;

    while(gameOver!=true) //GAME CONTINUES UNTIL 5 MISTAKES OR SOLVED
    {
        auto now = steady_clock::now();
        int secondsPassed = duration_cast<seconds>(now - startTime).count();
        int timeLeft = totalMinutes * 60 - secondsPassed;

        if (timeLeft <= 0)
        {
            cout << "\nTime's up! Game Over.\n";
            return true;
        }

        if(solved) 
            break; // If solved, exit the loop
    
        int row,col,num;
        std::cout<<"\nEnter row (1-9), column (1-9), and number (1-9) (or 0 0 0 to quite the game) : ";
        cin>>row>>col>>num;

        if(row==-1 && col==-1 && num==-1)
        {
            std::cout<<"Undoing the last move..."<<endl;
            if(game->undoMove())
            {
                std::cout<<"Undo successfully.\n";
                displayBoard(game->getCurrentBoard(), game);
                continue;
            }
            else
            {
                std::cout<<"No moves to undo.\n";
                continue;
            } 
        }

        if(row==0 && col==0 && num==0) //EXIT OPTION
        {
            std::cout<<"Exiting game...\n";
            return true;
        }
        // Validate input range
        if (row < 1 || row > 9 || col < 1 || col > 9 || num < 1 || num > 9) 
        {
            std::cout << "Invalid input! Row, column, and number must be between 1 and 9.\n";
            continue;
        }
        if(game->isOriginalCell(row-1,col-1))
        {
            std::cout<<"Cannot modify the original sudoku!\n";
            continue;
        }
        if(game->isValidMove(row-1,col-1,num))
        {
            
            std::cout<<"Move accepted.\n";
            game->makeMove(row-1, col-1, num);
            displayBoard(game->getCurrentBoard(), game);

            if(game->isSolved())
            {
                std::cout<<"\nCongratulations! You solved the puzzle!\n";
                solved=true;
                gameOver=true;
            }
        }
        else
        {
            std::cout<<"Invalid move! The number violates Sudoku rules.  Mistakes :"<<game->getMistakeCount() +1<<"/5"<<endl;
            game->increaseMistakeCount(); // Increment mistake count
            if(game->getMistakeCount()>=5)
            {
                std::cout<<"You have exceded mistake limits! Game Over.\n";
                gameOver=true;
            }
            else
            {
                continue;
            }
            
        }
    }

}

int main()
{
    scrollSudoku(50,15);
    std::cout << "========== WELCOME TO SUDOKU ==========\n";  // WELCOME MESSAGE
    displayRules() ;                                      // FUNCTION TO DISPLAY GAME RULES
    std::cout<<"Want to start the game? (yes/no) : ";             // ASK FOR STARTING THE GAME
    string start;
    cin>> start;

    Sudoku* game=nullptr;                                 //POINTER game OF TYPE CLASS SUDOKU : STATIC TYPE OF game - SUDOKU
    
    while (start == "y" || start == "Y" || start == "yes" || start == "Yes" || start == "YES") 
    {
        std::cout << "\nChoose difficulty (easy/medium/hard) : ";//ASK FOR DIFFICULTY LEVEL
        string difficulty;
        cin >> difficulty;
        if(difficulty != "easy" && difficulty != "medium" && difficulty != "hard") 
        {
            std::cout << "Invalid difficulty level. Try again\n";
            continue; 
        }

        std::cout << "Select level (1-10) : ";                //ASK FOR LEVEL
        int level;
        cin >> level;
        if(level < 1 || level > 10) 
        {
            std::cout << "Invalid level. Please select a level between 1 and 10.\n";
            continue; 
        }
        
        if (difficulty == "easy") 
        {
            game = new Easy(level);                      //DYNAMIC TYPE OF game - EASY
        } 
        else if (difficulty == "medium") 
        {
            game = new Medium(level);                    //DYNAMIC TYPE OF game - MEDIUM
        }
        else if (difficulty == "hard") 
        {
            game = new Hard(level);                      //DYNAMIC TYPE OF game - HARD
        }

        playGame(game);

        std::cout << "\nDo you want to play again? (yes/no) : "; //ASK FOR REPLAY OR QUIT
        cin >> start;
    }
    std::cout << "Thank you for playing Sudoku! 🎉\n";
    if (game != nullptr)
        delete game;                                    //FREE THE MEMORY ALLOCATED FOR game
}           