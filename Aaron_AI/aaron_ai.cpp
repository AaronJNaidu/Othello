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

const vector<pair<int, int> > directions = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};
const vector<pair<int, int>> corners = {{0, 0}, {0, 7}, {7, 0}, {7, 7}};
const vector<pair<int, int>> xsquares = {{1, 1}, {1, 6}, {6, 1}, {6, 6}};
const vector<pair<int, int>> awayCorners = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
const vector<pair<int, int>> squareOrders = {{0, 0}, {7, 0}, {0, 7}, {7, 7}, {3, 3}, {3, 4}, {4, 4}, {4, 3}, 
                                             {3, 2}, {4, 2}, {3, 5}, {4, 5}, {2, 3}, {2, 4}, {5, 4}, {5, 3}, 
                                             {2, 2}, {2, 5}, {5, 2}, {5, 5}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, 
                                             {7, 2}, {7, 3}, {7, 4}, {7, 5}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, 
                                             {2, 7}, {3, 7}, {4, 7}, {5, 7}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, 
                                             {6, 2}, {6, 3}, {6, 4}, {6, 5}, {2, 1}, {3, 1}, {4, 1}, {5, 1}, 
                                             {2, 6}, {3, 6}, {4, 6}, {5, 6}, {0, 1}, {1, 0}, {0, 6}, {6, 0}, 
                                             {7, 1}, {1, 7}, {6, 7}, {7, 6}, {1, 1}, {1, 6}, {6, 1}, {6, 6}};

struct position
{
    vector<vector<char>> currBoard;
    char toPlay;
    int numToPlay = 0; //white = 1, black = 0
    int row;
    int col;
    int pieceCounts[2] = {2, 2};
    
    int skippableMoves = 2;

    position(vector<vector<char>> curBoard, char tPlay, int whitePieces, int blackPieces, int skips){
        currBoard = curBoard;
        toPlay = tPlay;
        if(toPlay == 'W') numToPlay = 1;
        pieceCounts[0] = blackPieces;
        pieceCounts[1] = whitePieces;
        skippableMoves = skips;
    }

    char boardAt(int r, int c) {
        if(r < 0 or r > 7 or c < 0 or c > 7) return 'X';
        return currBoard[r][c];
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
        for (auto j : squareOrders) {
            row = j.first;
            col = j.second;
            if(isValid()) ans.push_back({j.first, j.second});
        }
        if(!ans.size()) {
            ans.push_back({-1, -1});
        }
        return ans;
    }

    void switchColours() {
        if(toPlay == 'B') toPlay = 'W';
        else if(toPlay == 'W') toPlay = 'B';
        numToPlay = 1 - numToPlay;
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
        pieceCounts[numToPlay]++;
        for (auto i : directions) {
            int tempRow = row;
            int tempCol = col;
            int count = numOutflanked(i.first, i.second);
            for (int j = 0; j < count; j++) {
                tempRow += i.first;
                tempCol += i.second;
                currBoard[tempRow][tempCol] = toPlay;
                pieceCounts[numToPlay]++;
                pieceCounts[1-numToPlay]--;
            }
        }
        switchColours();
        skippableMoves = 2;
    }

    bool endOfGame() {
        for(int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                row = i;
                col = j;
                if (isValid()) return false;
            }
        }
        switchColours();
        for(int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                row = i;
                col = j;
                if (isValid()) {
                    switchColours();
                    return false;
                }
            }
        }
        switchColours();
        return true;
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

    int cornerMobility() {
        if (endOfGame()) {
            return 1000000 * (pieceCounts[1] - pieceCounts[0]);            
        }
        int eval = 0;
        char toMove = toPlay;
        int numToMove = numToPlay;

        toPlay = 'W';
        numToPlay = 1;
        double whiteMoves = legalMoves().size();

        toPlay = 'B';
        numToPlay = 0;
        double blackMoves = legalMoves().size();
        
        double mobility = 1000 * (whiteMoves - blackMoves) / (whiteMoves + blackMoves);

        double cornerControl = 0;
        for (int i = 0; i < 4; i++) {
            switch (currBoard[corners[i].first][corners[i].second]) {
            case 'W': cornerControl += 10000;
                break;
            case 'B': cornerControl -= 10000;
                break;
            default:
                switch (currBoard[xsquares[i].first][xsquares[i].second]) {
                case 'W': cornerControl -= 8000;
                    break;
                case 'B': cornerControl += 8000;
                    break;
                default:
                    switch (currBoard[corners[i].first][xsquares[i].second])
                    {
                    case 'W': cornerControl -= 4000;
                        break;
                    case 'B': cornerControl += 4000;
                        break;
                    default:
                        break;
                    }
                    break;
                    switch (currBoard[xsquares[i].first][corners[i].second])
                    {
                    case 'W': cornerControl -= 4000;
                        break;
                    case 'B': cornerControl += 4000;
                        break;
                    default:
                        break;
                    }
                    break;
                }
                break;
            }
        }
        return (int) (mobility + cornerControl);
    }

    int frontierStable() {
        int stableDisks = 0;

        for (int i = 0; i < corners.size(); i++) {
            int cutoffColumn = 7 - corners[i].second;
            for (int j = corners[i].first; j != 7-corners[i].first; j+=awayCorners[i].first) {
                for (int k = corners[i].second; k != cutoffColumn; k+=awayCorners[i].second)
                {
                    if (currBoard[j][k] == 'W') stableDisks += 1000;
                    else {
                        cutoffColumn = k;
                        break;
                    }   
                }
            }
        }

        for (int i = 0; i < corners.size(); i++) {
            int cutoffColumn = 7 - corners[i].second;
            for (int j = corners[i].first; j != 7-corners[i].first; j+=awayCorners[i].first) {
                for (int k = corners[i].second; k != cutoffColumn; k+=awayCorners[i].second)
                {
                    if (currBoard[j][k] == 'B') stableDisks -= 1000;
                    else {
                        cutoffColumn = k;
                        break;
                    }   
                }
            }
        }

        int frontierDisks = 0;

        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (currBoard[i][j] == 'W') {
                    for (auto k : directions) {
                        if(boardAt(i + k.first, j + k.second) == '.') frontierDisks -= 100;
                    }
                }
                else if (currBoard[i][j] == 'B') {
                    for (auto k : directions) {
                        if(boardAt(i + k.first, j + k.second) == '.') frontierDisks += 100;
                    }
                }
            }
        }

        return frontierDisks + stableDisks;
    }

    int examined[8][8];

    int compSize(int r, int c) { //assumes curr square not examined
        int toRet = 0;
        queue<pair<int, int>> q;
        examined[r][c] = true;
        q.push({r, c});
        while (!q.empty()) {
            pair<int, int> curr = q.front();
            q.pop();
            toRet++;
            for (auto i : directions) {
                if (boardAt(r+i.first, c+i.second) == '.' and !examined[r+i.first][c+i.second]) {
                    q.push({r+i.first, c+i.second});
                    examined[r+i.first][c+i.second] = true;
                } 
            }
        }
        return toRet;
    }

    int parity() {
        int ans = 0;
        vector<int> parities;
        int paritySum = 0;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                examined[i][j] = false;
            }
        }
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if(currBoard[i][j] == '.' and !examined[i][j]) {
                    parities.push_back(compSize(i, j) % 2);
                    paritySum += parities[parities.size()-1];
                }
            }
        }
        if (paritySum % 2) ans = 400 * parities.size();
        else ans = -400 * parities.size();
        
        if (toPlay == 'B') ans *= -1;
        return ans;        
    }

    int evaluate1() {
        return frontierStable() + cornerMobility();
    }

    int evaluate2() {
        return frontierStable() + cornerMobility() + parity();
    }

};

struct engine
{
    //unordered_map<vector<vector<char>>, vector<pair<int, pair<int, int>>>> posEval;
    position currPos = position(board, 'B', 2, 2, 2);

    bool gameOver() {
        if (currPos.skippableMoves <= 0 or currPos.pieceCounts[0] + currPos.pieceCounts[1] == 64) {
            return 0;
        }
        return 1;
    }

    bool recieveMove() { 
        int r, c;
        cin >> r >> c;
        currPos.makeMove(r, c);
        return gameOver();
    }
        
    bool randomMove() {
        vector<pair<int, int>> legalMoves = currPos.legalMoves();
        int randInd = rand() % legalMoves.size();
        currPos.makeMove(legalMoves[randInd].first, legalMoves[randInd].second);
        cout << legalMoves[randInd].first << " " << legalMoves[randInd].second << "\n";
        currPos.printBoard();
        return gameOver();
    }

    bool bestEvalMove1() {
        vector<pair<int, int>> legalMoves = currPos.legalMoves();
        int bestInd = -1;
        if (legalMoves.size() == 1) {
            currPos.makeMove(legalMoves[0].first, legalMoves[0].second);
            cout << legalMoves[0].first << " " << legalMoves[0].second << "\n";
            currPos.printBoard();
            return gameOver();
        }
        if (currPos.toPlay == 'W') {
            int bestEval = -1000000000;
            for (int i = 0; i < legalMoves.size(); i++) {
                position tempPos = position(currPos.currBoard, currPos.toPlay, currPos.pieceCounts[1], currPos.pieceCounts[0], currPos.skippableMoves);
                tempPos.makeMove(legalMoves[i].first, legalMoves[i].second);
                int eval = tempPos.evaluate1();
                if (eval > bestEval) {
                    bestEval = eval;
                    bestInd = i; 
                }                
            }
        }
        else {
            int bestEval = 1000000000;
            for (int i = 0; i < legalMoves.size(); i++) {
                position tempPos = position(currPos.currBoard, currPos.toPlay, currPos.pieceCounts[1], currPos.pieceCounts[0], currPos.skippableMoves);
                tempPos.makeMove(legalMoves[i].first, legalMoves[i].second);
                int eval = tempPos.evaluate1();
                if (eval < bestEval) {
                    bestEval = eval;
                    bestInd = i; 
                }                
            }
        }
        currPos.makeMove(legalMoves[bestInd].first, legalMoves[bestInd].second);
        cout << legalMoves[bestInd].first << " " << legalMoves[bestInd].second << "\n";
        currPos.printBoard();
        return gameOver();
    }

    bool bestEvalMove2() {
        vector<pair<int, int>> legalMoves = currPos.legalMoves();
        int bestInd = -1;
        if (legalMoves.size() == 1) {
            currPos.makeMove(legalMoves[0].first, legalMoves[0].second);
            cout << legalMoves[0].first << " " << legalMoves[0].second << "\n";
            currPos.printBoard();
            return gameOver();
        }
        if (currPos.toPlay == 'W') {
            int bestEval = -1000000000;
            for (int i = 0; i < legalMoves.size(); i++) {
                position tempPos = position(currPos.currBoard, currPos.toPlay, currPos.pieceCounts[1], currPos.pieceCounts[0], currPos.skippableMoves);
                tempPos.makeMove(legalMoves[i].first, legalMoves[i].second);
                int eval = tempPos.evaluate2();
                if (eval > bestEval) {
                    bestEval = eval;
                    bestInd = i; 
                }                
            }
        }
        else {
            int bestEval = 1000000000;
            for (int i = 0; i < legalMoves.size(); i++) {
                position tempPos = position(currPos.currBoard, currPos.toPlay, currPos.pieceCounts[1], currPos.pieceCounts[0], currPos.skippableMoves);
                tempPos.makeMove(legalMoves[i].first, legalMoves[i].second);
                int eval = tempPos.evaluate2();
                if (eval < bestEval) {
                    bestEval = eval;
                    bestInd = i; 
                }                
            }
        }
        currPos.makeMove(legalMoves[bestInd].first, legalMoves[bestInd].second);
        cout << legalMoves[bestInd].first << " " << legalMoves[bestInd].second << "\n";
        currPos.printBoard();
        return gameOver();
    }

    int bestEvalDepth(int depth, int layers, int alpha, int beta, position thisPos) {
        if (depth == 0) return thisPos.evaluate2();

        if (thisPos.endOfGame()) return thisPos.evaluate2();
        
        vector<pair<int, int>> legalMoves = thisPos.legalMoves();
        int bestInd = -1;
        if (legalMoves.size() == 1 and layers == 0) {
            currPos.makeMove(legalMoves[0].first, legalMoves[0].second);
            cout << legalMoves[0].first << " " << legalMoves[0].second << "\n";
            currPos.printBoard();
            return gameOver();
        }
        if (thisPos.toPlay == 'W') {
            int bestEval = -1000000000;
            for (int i = 0; i < legalMoves.size(); i++) {
                position tempPos = position(thisPos.currBoard, thisPos.toPlay, thisPos.pieceCounts[1], thisPos.pieceCounts[0], thisPos.skippableMoves);
                tempPos.makeMove(legalMoves[i].first, legalMoves[i].second);
                int tempEval = bestEvalDepth(depth-1, layers+1, alpha, beta, tempPos);
                if (tempEval > bestEval) {
                   bestEval = tempEval;
                   bestInd = i;
                }
                alpha = max(alpha, bestEval);
                if(alpha >= beta) return bestEval;             
            }   
            if (layers == 0) {
                currPos.makeMove(legalMoves[bestInd].first, legalMoves[bestInd].second);
                cout << legalMoves[bestInd].first << " " << legalMoves[bestInd].second << "\n";
                currPos.printBoard();
                return gameOver();
            }
            return bestEval;
        }
        else {
            int bestEval = 1000000000;
            for (int i = 0; i < legalMoves.size(); i++) {
                position tempPos = position(thisPos.currBoard, thisPos.toPlay, thisPos.pieceCounts[1], thisPos.pieceCounts[0], thisPos.skippableMoves);
                tempPos.makeMove(legalMoves[i].first, legalMoves[i].second);
                int tempEval = bestEvalDepth(depth-1, layers+1, alpha, beta, tempPos);
                if (tempEval < bestEval) {
                   bestEval = tempEval;
                   bestInd = i;
                }
                beta = min(beta, bestEval);
                if(alpha >= beta) return bestEval;             
            }  
            if (layers == 0) {
                currPos.makeMove(legalMoves[bestInd].first, legalMoves[bestInd].second);
                cout << legalMoves[bestInd].first << " " << legalMoves[bestInd].second << "\n";
                currPos.printBoard();
                return gameOver();
            }
            return bestEval;
        }
        
    }

    void printWinner() {
        
        cout << currPos.pieceCounts[1] << " white disks VS " << currPos.pieceCounts[0] << " black disks\n";
        if (currPos.pieceCounts[1] > currPos.pieceCounts[0]) {
            cout << "White Wins\n";
        }
        else if (currPos.pieceCounts[0] > currPos.pieceCounts[1]) {
            cout << "Black Wins\n";
        }
        else {
            cout << "Draw\n";
        }  
    }

};


int main(){
    srand((int) time(0));
    engine ai = engine();  
    int player;
    int keepGoing = 1;
    cin >> player;
    if (player == -1) {
        ai.bestEvalMove2();
    }
    while (keepGoing) {
        keepGoing = ai.bestEvalDepth(5, 0, -1000000000, 1000000000, ai.currPos);
        if (keepGoing) keepGoing = ai.bestEvalMove2();
    } 
    ai.printWinner();
    
    return 0;   
}