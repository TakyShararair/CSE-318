
#include<bits/stdc++.h>
#include<vector>
#include"node.h"
using namespace std;

bool valid(int zr, int zc, int move, int row, int col) {
    switch (move) {
        case 0: // left
            return zc > 0;
        case 1: // right
            return zc < col - 1;
        case 2: // up
            return zr > 0;
        case 3: // down
            return zr < row - 1;
        default:
            return false;
    }
}


int  solvable(const vector<vector<int>> &board, int zr) {
  int countInversion = 0;

  vector<int> forInversion;
  for(int i=0; i < board.size();i++)
  {
    for(int j=0; j < board[i].size(); j++)
    {
      forInversion.push_back(board[i][j]);
    }
  }

  for (int i = 0; i < forInversion.size(); i++) {
    if(forInversion[i]==0)
    {
      continue;
    }
    for (int j = i + 1; j < forInversion.size(); j++) {
      if(forInversion[j]==0)
      {
        continue;
      }
      if ( forInversion[i] > forInversion[j]) 
      {
        countInversion++;
      }
  }
  }
  int k = board.size();

  if (k % 2) {
    // if odd, then number of inversions must be even
    int i = (countInversion % 2 == 0);
    return i;
  }

  else {
   int diff = k - zr;
   bool isOddDiff = (diff % 2 != 0);
   bool isOddInversionCount = (countInversion % 2 != 0);

   int j;
   if (isOddDiff != isOddInversionCount) {
      j = 1;
   } else {
    j = 0;
   }
   return j;

  }
}


class Compare {
public:
    enum DistanceType { Hamming, Manhattan };

private:
    DistanceType type;
    
    // Private helper functions for comparison
    bool compareHamming(Node* a, Node* b) const {
        assert(a->hamming_dist != -1 && b->hamming_dist != -1);
        return (a->hamming_dist + a->moves) > (b->hamming_dist + b->moves);
    }

    bool compareManhattan(Node* a, Node* b) const {
        assert(a->manhattan_dist != -1 && b->manhattan_dist != -1);
        return (a->manhattan_dist + a->moves) > (b->manhattan_dist + b->moves);
    }
  
public:
    // Constructor
    Compare(DistanceType type = Manhattan) : type(type) {}
    // Overload operator() for comparison
    bool operator()(Node* a, Node* b) const {
        if (type == Hamming) {
            return compareHamming(a, b);
        } else { // Manhattan
            return compareManhattan(a, b);
        }
    }
};


void A_starSearch(Node *start, bool useManhattan) {
 
  Compare::DistanceType type = useManhattan ? Compare::Manhattan : Compare::Hamming;
   Compare comp(type);
  priority_queue<Node *, vector<Node *>, Compare> openList(comp);
  set<vector<vector<int>>> visited;
  vector<Node *> pointer_to_be_deleted;
  openList.push(start);
  int expanded = 0;
  int explored = 1;
  int moveCount = 0;
  bool flag = true;
  

  while (flag) {
    Node *cur = openList.top();
    openList.pop();
    expanded++;
    visited.insert(cur->board);
    pointer_to_be_deleted.push_back(cur);

    for (int move = 0; move < 4; move++) {

      bool isMoveValid = valid(cur->zr, cur->zc, move, cur->board.size(), cur->board[0].size());
      if (!isMoveValid) {
          continue;
      }

      Node *new_node = cur->make_move(move);
      bool isVisited = visited.count(new_node->board) > 0;
       if (isVisited) {
        delete new_node;
        continue;
     }

      bool isGoal = new_node->hamming_dist == 0 && new_node->manhattan_dist == 0;

      if (isGoal) {      
        moveCount = new_node->moves;
        stack<Node *> st;
        Node *goal = new_node;

        while (new_node->parent != nullptr) {
          st.push(new_node);
          new_node = new_node->parent;
        }

        st.push(new_node);
        int i = 0;
        while (!st.empty()) {
          int sizeOfStack;
          sizeOfStack = st.size();      
          Node node ;
          node = *(st.top());
          cout << "Move No : " << i  << endl;
          i++;
          node.print();
          cout << endl;
          st.pop();
        }

        delete goal;

        flag = false;
        break;
      }

      else {
        openList.push(new_node);
        explored++;
      }
    }
  }

  cout << " Number Of explored nodes : " << explored << endl;
  cout << " Number of expanded nodes : " << expanded << endl;
  cout << "Total Number of moves : " << moveCount << endl;

   vector<Node*> nodes;
   while (!openList.empty()) {
      nodes.push_back(openList.top());
      openList.pop();
   }

  for (Node* node : nodes) {
     delete node;
   }

   for (size_t i = 0; i < pointer_to_be_deleted.size(); ++i) {
    delete pointer_to_be_deleted[i];
}


}

int main() {
  cout << "Enter the board size ( value of k) : " << endl;
  int k;
  cin >> k;
  vector<vector<string>> boardForString(k, vector<string>(k,"0"));
  vector<vector<int>> board(k, vector<int>(k, 0));
  int zr = 0;
  string asterik = "*";

  for (int i = 0; i < k; i++) {
    for (int j = 0; j < k; j++) {
      cin >> boardForString[i][j];
      if(boardForString[i][j] == asterik)
      {
             board[i][j] = 0;
             zr = i;
      }
      else{
        string str = boardForString[i][j];
        int num = stoi(str);
        board[i][j] = num;
      }
    }
  }

  int checkSolvability = solvable(board,zr);

  if (checkSolvability == 0) {
    cout << "This is not solvable for the initial board" << endl;
    return 0;
  }
  else
  {
    cout << " This puzzle is solvable " << endl;
  }

  Node *initial = new Node(board, nullptr);
  cout << "Using Manhattan distance :" << endl;
  A_starSearch(initial, true);

  Node   *initial2 = new Node(board, nullptr);
  cout << endl << "Using Hamming distance : " << endl;;
   A_starSearch(initial2, false);
   return 0;
}
