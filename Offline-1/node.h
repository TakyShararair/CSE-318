#include<bits/stdc++.h>
using namespace std;

struct Node {
 public:
  vector<vector<int>> board;
  Node *parent = nullptr;
  int hamming_dist = -1;
  int manhattan_dist = -1;
  int moves = 0;
  int zr = 0;
  int zc = 0;

 
  Node(int k=0) : board(k,vector<int>(k,0))
  {

  }

  Node(const vector<vector<int>> &board, Node* par) {
    this->board = board;

    for (int i = 0; i < board.size(); i++) {
      for (int j = 0; j < board[i].size(); j++) {
        if (board[i][j] == 0) {
          zr = i;
          zc = j;
          break;
        }
      }
    }

    hamming_dist = this->hamming_distance();
    manhattan_dist = this->manhattan_distance();

    this->parent = par;
    if(par)
    {
      moves = par->moves + 1;
    }
    else
    {
      moves = 0;
    }
  }

  int hamming_distance() {
    int distance = 0;
 
   int rowIndex = 0;
   for(int i=0; i < board.size();i++)
   {
    for(int j=0; j < board.size(); j++)
    {
      if(board[i][j] && board[i][j] != rowIndex+1)
      {
        distance++;
      }
      rowIndex++;
    }
   }

    return distance;
  }
  
 int manhattan_distance()
 {
  int distance = 0;
  for(int i=0; i < board.size(); i++)
  {
    for(int j=0; j < board.size(); j++)
    {
      int val = board[i][j];
      if(val == 0)
      {
        continue;
      }
      else
      {
        int row = (val - 1)/ board.size();
        int col = (val - 1) % board.size();
        distance += abs(row - i) + abs(col - j);
      }
    }
  }
  return distance ;
 }

void print(ostream &os = cout) const
{
  if(board.size()==3)
  {
     os << "-----------" << endl;
  }
  if(board.size()==4)
  {
    os << "------------------" << endl;
  }
  for(int i=0; i < board.size(); i ++)
  {
    
    os << " |";
    for(int j=0; j < board[i].size(); j++)
    {
      
      os << board[i][j] << " |";
    }
    os << endl;
    if(board.size()==3)
  {
     os << "-----------" << endl;
  }
  if(board.size()==4)
  {
    os << "------------------" << endl;
  }
  }
}

  Node *make_move(int move) {
    vector<vector<int>> boardAfterMove = board;

    if (move == 0) {
      assert(zc > 0);
     int temp = boardAfterMove[zr][zc];
     boardAfterMove[zr][zc] = boardAfterMove[zr][zc - 1];
     boardAfterMove[zr][zc - 1] = temp; // left
    } 
    else if (move == 1) {
      assert(zc < board[0].size() - 1);
     int temp = boardAfterMove[zr][zc];
     boardAfterMove[zr][zc] = boardAfterMove[zr][zc + 1];
     boardAfterMove[zr][zc + 1] = temp; // right

    } 
    else if (move == 2) {
      assert(zr > 0);
     int temp = boardAfterMove[zr][zc];
     boardAfterMove[zr][zc] = boardAfterMove[zr-1][zc];
     boardAfterMove[zr-1][zc] = temp; // up

    }
     else if (move == 3) {
      assert(zr < board.size() - 1);
      int temp = boardAfterMove[zr][zc];
      boardAfterMove[zr][zc] = boardAfterMove[zr+1][zc];
      boardAfterMove[zr+1][zc] = temp; // down

    }

    Node *new_board = new Node(boardAfterMove, this);
    return new_board;
  }
};