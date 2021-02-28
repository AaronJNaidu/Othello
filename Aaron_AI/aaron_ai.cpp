#include <bits/stdc++.h>
using namespace std;

vector<vector<char>> board =    {{'.','.','.','.','.','.','.','.'},
                                {'.','.','.','.','.','.','.','.'},
                                {'.','.','.','.','.','.','.','.'},
                                {'.','.','.','W','B','.','.','.'},
                                {'.','.','.','B','W','.','.','.'},
                                {'.','.','.','.','.','.','.','.'},
                                {'.','.','.','.','.','.','.','.'},
                                {'.','.','.','.','.','.','.','.'},
                                };

struct position
{
    vector<vector<char>> currBoard;
    char toPlay;
    int row;
    int col;

    const vector<pair<int, int> > directions = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};
    int skippableMoves = 2;
    int coveredSquares = 4;

    position(vector<vector<char>> curBoard, char tPlay){
        currBoard = curBoard;
        toPlay = tPlay;
    }

    int numOutflanked(int rowDirection, int colDirection) { //assume board[row][col] is empty
        int ans = 0;
        int rowTemp, colTemp;
        rowTemp = row + rowDirection;
        colTemp = col + colDirection;
        while(0 <= colTemp and colTemp <= 7 and 0 <= rowTemp and rowTemp <= 7) {
            if (currBoard[rowTemp][colTemp] == '.') return 0;
            if (currBoard[rowTemp][colTemp] == toPlay) return ans;
            ans++;
            
            rowTemp += rowDirection;
            colTemp += colDirection;
        }
        return 0;
    }

    bool isValid() { //assume row/col are valid indices
        if(currBoard[row][col] != '.') return false;
        for (auto i : directions) {
            if(numOutflanked(i.first, i.second)) return true;
        }
        return false;
    }

    vector<pair<int, int>> legalMoves() {
        vector<pair<int, int>> ans;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                row = i;
                col = j;
                if(isValid()) ans.push_back({i, j});
            }
        }
        if(!ans.size()) {
            ans.push_back({-1, -1});
        }
        return ans;
    }

    void switchColours() {
        if(toPlay == 'B') toPlay = 'W';
        else if(toPlay == 'W') toPlay = 'B';
    }

    void makeMove(int r, int c) { //assumes move is legal
        row = r;
        col = c;
        if(row == -1) {
            switchColours();
            skippableMoves--;
            return;
        }
        currBoard[r][c] = toPlay;
        for (auto i : directions) {
            int tempRow = row;
            int tempCol = col;
            int count = numOutflanked(i.first, i.second);
            for (int j = 0; j < count; j++) {
                tempRow += i.first;
                tempCol += i.second;
                currBoard[tempRow][tempCol] = toPlay;
            }
        }
        switchColours();
        skippableMoves = 2;
        coveredSquares++;
    }

    void printBoard() {
        cout << "\n   ";
        for(int i = 0; i < 8; i++) {
            cout << i << " ";
        }
        cout << "\n";
        for (int i = 0; i < 8; i++) {
            cout << i << "  ";
            for (int j = 0; j < 8; j++) {
                cout << currBoard[i][j] << " ";
            }
            cout << "\n";
        }
        cout << "\n";
    }

};

struct engine
{
    position currPos = position(board, 'B');

    int recieveMove() { 
        int r, c;
        cin >> r >> c;
        currPos.makeMove(r, c);
        if (currPos.skippableMoves == 0 or currPos.coveredSquares == 64) {
            return 0;
        }
        return 1;
    }
        
    int randomMove() {
        vector<pair<int, int>> legalMoves = currPos.legalMoves();
        int randInd = rand() % legalMoves.size();
        currPos.makeMove(legalMoves[randInd].first, legalMoves[randInd].second);
        cout << legalMoves[randInd].first << " " << legalMoves[randInd].second << "\n";
        currPos.printBoard();
        if (currPos.skippableMoves == 0 or currPos.coveredSquares == 64) {
            return 0;
        }
        return 1;
    }

    void gameOver() {
        int whiteCount = 0;
        int blackCount = 0;

        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (currPos.currBoard[i][j] == 'W') {
                    whiteCount++;
                }
                else {
                    blackCount++;
                }
            }
        }

        cout << whiteCount << " white disks VS " << blackCount << " black disks\n";
        if (whiteCount > blackCount)
        {
            cout << "White Wins\n";
        }
        else if (blackCount > whiteCount)
        {
            cout << "Black Wins\n";
        }
        else
        {
            cout << "Draw\n";
        }
    }
};


int main(){
    position start = position(board, 'B');
    engine ai = engine();  
    int player;
    int keepGoing = 1;
    cin >> player;
    if (player == -1)
    {
        ai.recieveMove();
    }
    while (keepGoing)
    {
        keepGoing = ai.randomMove();
        if (keepGoing)
        {
            keepGoing = ai.recieveMove();
        }
    } 
    ai.gameOver();
    return 0;   
}