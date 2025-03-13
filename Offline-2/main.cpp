#include<bits/stdc++.h>
using namespace std;


int each_slot=6;
int boardSize= (2*each_slot)+2;
int stone_per_slot=4;

int storagePlayer1 = each_slot; // array indexing 6 ( 7th bin)
int storagePlayer2 = boardSize-1; // array indexing 13 (13th bin)

void initial_board(int board[])
{
    for(int i=0; i < boardSize; i++)
    {
        if(i==storagePlayer1 || i==storagePlayer2)
        {
            board[i]=0; // initial storage for player 1 and player 2 is 0
        }
        else
        {
           board[i]= stone_per_slot;
        }

    }

}

class Node
{
 public:
   int findValue;
   int bestMove;

   Node(int findValue,int bestMove)
   {
    this->findValue=findValue;
    this->bestMove= bestMove;
   }

   Node() { }

};

pair<bool,int> move(int board[],int pos,int turnPlayer)
{
    pair<bool,int> returnP;
    if(turnPlayer==1)
    {
        int captureStone=0;
        int moves=board[pos];
        board[pos]=0;
        int slot= pos+1;
        while(moves > 0)
        {
            if(slot==storagePlayer2)
            {
                slot=(slot+1)%boardSize;
                continue;
            }
            board[slot]+=1;
            moves=moves-1;
            if(moves==0)
            {
                int opposite_side= (2*each_slot) - slot;
                if((board[slot]==1 && slot >= 0 && slot <=5 && board[opposite_side]) > 0 )
                {
                    board[storagePlayer1]+=board[slot];
                    board[storagePlayer1]+=board[opposite_side];
                    captureStone+=board[slot];
                    captureStone+=board[opposite_side];
                    board[slot]=0;
                    board[opposite_side]=0;
                    returnP.first=false;
                    returnP.second=captureStone;
                    return returnP;
                }

                else if(slot == storagePlayer1)
                {
                    returnP.first=true;
                    returnP.second= captureStone;
                    return returnP;
                }
            }

            slot = (slot+1)%boardSize;
        }
        returnP.first=false;
        returnP.second=captureStone;
        return returnP;

    }

    else
    {
        int captureStone=0;
        int moves=board[pos];
        board[pos]=0;
        int slot= pos+1;
        while(moves > 0)
        {
            if(slot==storagePlayer1)
            {
                slot=(slot+1)%boardSize;
                continue;
            }
            board[slot]+=1;
            moves=moves-1;
            if(moves==0)
            {
                int opposite_side= (2*each_slot)-slot;
                if((board[slot]==1 && slot >= 7 && slot <=12 && board[opposite_side]>0) > 0)
                {
                    board[storagePlayer2]+=board[slot];
                    board[storagePlayer2]+=board[opposite_side];
                    captureStone+=board[slot];
                    captureStone+=board[opposite_side];
                    board[slot]=0;
                    board[opposite_side]=0;
                    returnP.first=false;
                    returnP.second=captureStone;
                    return returnP;
                }
                else if(slot == storagePlayer2)
                {
                    returnP.first=true;
                    returnP.second= captureStone;
                    return returnP;
                }
            }
            slot = (slot+1)%boardSize;
        }

        returnP.first=false;
        returnP.second=captureStone;
        return returnP;
    }
}




bool is_game_over(int board[]) {
    // Check if all stones are empty on Player 1's side
    int sumPlayer1 = 0, sumPlayer2 = 0;

    // Sum Player 1's side
    for (int i = 0; i < each_slot; i++) {
        sumPlayer1 += board[i];
    }

    // Sum Player 2's side
    for (int i = each_slot + 1; i < boardSize - 1; i++) {
        sumPlayer2 += board[i];
    }

    // If Player 1's side is empty, transfer stones from Player 2's side to their store
    if (sumPlayer1 == 0) {
        for (int i = each_slot + 1; i < boardSize - 1; i++) {
            board[storagePlayer2] += board[i];
            board[i] = 0;
        }
        return true;
    }

    // If Player 2's side is empty, transfer stones from Player 1's side to their store
    if (sumPlayer2 == 0) {
        for (int i = 0; i < each_slot; i++) {
            board[storagePlayer1] += board[i];
            board[i] = 0;
        }
        return true;
    }

    return false;
}


int Winner(int board[]) {
    // Compare the stone counts in the storage areas of both players
    if (board[storagePlayer1] == board[storagePlayer2]) {
        return 3; // Game is a draw
    }
    return (board[storagePlayer1] > board[storagePlayer2]) ? 1 : 2;
}



bool isMoveValid(int board[], int playerTurn, int pos) {
    // Define the valid move range based on the player's turn
    int minPos = (playerTurn == 1) ? 0 : each_slot + 1;
    int maxPos = (playerTurn == 1) ? each_slot : boardSize - 1;

    // Check if the position is within range and has stones
    return (pos >= minPos && pos < maxPos && board[pos] > 0);
}



void showBoard(int board[])
{
    // Player 2's side (opposite row)
    cout << "---------------------------------------" << endl;
    cout << "|   |";
    for(int i = (2 * each_slot); i >= (each_slot + 1); i--)
    {
        cout << " " << board[i] << "   |";
    }
    cout << endl;
    cout << "---------------------------------------" << endl;

    // Player storage areas
    cout << "| " << board[boardSize - 1] << " |";
    for (int i = 0; i < each_slot; i++)
    {
        cout << "    ";
    }
    cout << "         |  " << board[each_slot] << " |" << endl;

    // Player 1's side (bottom row)
    cout << "---------------------------------------" << endl;
    cout << "|   |";
    for(int i = 0; i < each_slot; i++)
    {
        cout << " " << board[i] << "   |";
    }
    cout << endl;
    cout << "---------------------------------------" << endl;
}

int stone_in_side(int playerTurn, int board[]) {
    int sum = 0;
    int startPos = (playerTurn == 1) ? 0 : each_slot + 1;
    int endPos = (playerTurn == 1) ? each_slot : boardSize - 1;

    // Loop through the relevant range and calculate the sum of stones
    for (int i = startPos; i < endPos; ++i) {
        sum += board[i];
    }

    return sum;
}


int heuristic_1(int board[])
{
    int val_1;
    val_1 = board[storagePlayer1] - board[storagePlayer2];
    return val_1;
}

int heuristic_2(int board[])
{
     srand(time(0));
     int W1= rand()%5 + 1;
     int W2 = rand()%5 + 1;
     int val_2;
     val_2 = W1 * (board[storagePlayer1] - board[storagePlayer2]) + W2 * (stone_in_side(1,board) - stone_in_side(2,board));
    return val_2;

}

int heuristic_3(int board[],int extraMoveP1,int extraMoveP2)
{
    srand(time(0));
    int W1= rand()%5 + 1;
    int W2= rand()%5 + 1;
    int W3= rand()%5 + 1;
    int val_3;
    val_3 = W1*(board[storagePlayer1] - board[storagePlayer2]) + W2 * (stone_in_side(1,board) - stone_in_side(2,board)) + W3 * (extraMoveP1 - extraMoveP2);

   return val_3;
}

int heuristic_4(int board[],int extraMoveP1,int extraMoveP2,int stonesCapturedP1,int stonesCapturedP2)
{
    int W1= rand()%5 + 1;
    int W2= rand()%5 + 1;
    int W3= rand()%5 + 1;
    int W4 = rand()%5 + 1;
    int val_4;
    val_4 = W1*(board[storagePlayer1] - board[storagePlayer2]) + W2 * (stone_in_side(1,board) - stone_in_side(2,board)) + W3 * (extraMoveP1 - extraMoveP2) + W4 * (stonesCapturedP1 - stonesCapturedP2);
    return val_4;
}

Node min_max_with_pruning(int board[],int depth,int alpha,int beta,int isMax,int heuristic,int extraMoveP1,int extraMoveP2,int captureStoneP1,int captureStoneP2)
{
    if(is_game_over(board) || depth==0)
    {
        if(heuristic==1)
        {
            Node node(heuristic_1(board),-1);
            return node;
        }
        else if(heuristic==2)
        {
            Node node(heuristic_2(board),-1);
            return node;
        }
        else if(heuristic==3)
        {
            Node node(heuristic_3(board,extraMoveP1,extraMoveP2),-1);
            return node;
        }
        else if(heuristic==4)
        {
            Node node(heuristic_4(board,extraMoveP1,extraMoveP2,captureStoneP1,captureStoneP2),-1);
            return node;
        }

    }

    if(isMax)
    {
        Node tempNode(-INT_MAX,-1);
        for(int i=0; i < each_slot; i++)
        {
            int curr_move = i;
            if(board[curr_move]==0)
            {
                continue;
            }

             int tempBoard[boardSize];
             for (int i = 0; i < boardSize; i++) {
               tempBoard[i] = 0;
             }

            for(int i=0; i < boardSize;i++)
            {
                tempBoard[i]=board[i];
            }

            pair<bool,int>returnPair= move(tempBoard,i,2-isMax);
            bool isFreeTurn = returnPair.first;
            int cap = returnPair.second;
            Node resultNode;

            if(isFreeTurn)
            {
                resultNode=min_max_with_pruning(tempBoard,depth-1,alpha,beta,isFreeTurn,heuristic,extraMoveP1+1,extraMoveP2,captureStoneP1+cap,captureStoneP2);

            }
            else
            {
                resultNode=min_max_with_pruning(tempBoard,depth-1,alpha,beta,isFreeTurn,heuristic,extraMoveP1,extraMoveP2,captureStoneP1+cap,captureStoneP2);
            }

            if(resultNode.findValue >= tempNode.findValue)
            {
                tempNode.findValue= resultNode.findValue;
                tempNode.bestMove = i;
            }
            alpha = max(alpha,resultNode.findValue);

            if(beta <= alpha)
            {
                break;
            }
        }
        return tempNode;
    }

    else
    {
        Node tempNode(INT_MAX,-1);
        for(int i= (each_slot+1); i < (boardSize-1);i++)
        {
            int curr_move = i;
            if(board[curr_move]==0)
            {
                continue;
            }
            int tempBoard[boardSize]={0};
            for(int i=0; i < boardSize; i++)
            {
                tempBoard[i]=board[i];
            }
            pair<bool,int>returnPair= move(tempBoard,i,2-isMax);
            bool isFreeTurn = returnPair.first;
            int cap = returnPair.second;
            Node resultNode;

            if(isFreeTurn)
            {
                resultNode =min_max_with_pruning(tempBoard,depth-1,alpha,beta,!isFreeTurn,heuristic,extraMoveP1,extraMoveP2+1,captureStoneP1,captureStoneP2+cap);
            }
            else
            {
                resultNode =min_max_with_pruning(tempBoard,depth-1,alpha,beta,!isFreeTurn,heuristic,extraMoveP1,extraMoveP2,captureStoneP1,captureStoneP2+cap);
            }

            if(resultNode.findValue <= tempNode.findValue)
            {
                tempNode.findValue=resultNode.findValue;
                tempNode.bestMove=i;
            }

            beta = min(beta,resultNode.findValue);
            if(beta <= alpha)
            {
                break;
            }
        }
        return tempNode;
    }

}

void report(int h1,int h2)
{
        int winCount_for1 = 0;
        int winCount_for2 = 0;
        int tieCount = 0;
        for(int i=0; i < 100 ; i++)
        {
             int board[boardSize]={0};
             initial_board(board);

             while (true)
           {
              Node node = min_max_with_pruning(board,10,-INT_MAX,INT_MAX,true,h1,0,0,0,0);

              pair<bool,int> returnPair=move(board,node.bestMove,1);
              bool val= returnPair.first;

              if(is_game_over(board))
             {
                 break;
             }
             if(val)
             {
                 continue;
             }

              node = min_max_with_pruning(board,10,-INT_MAX,INT_MAX,false,h2,0,0,0,0);
              returnPair= move(board,node.bestMove,2);
              val = returnPair.first;

             if(is_game_over(board))
             {
                  break;
             }

              while (val)
             {

                node = min_max_with_pruning(board,10,-INT_MAX,INT_MAX,false,h2,0,0,0,0);
                returnPair= move(board,node.bestMove,2);
                val = returnPair.first;

                if(is_game_over(board))
                {
                   break;
                }

            }
            if(is_game_over(board))
            {
                break;
            }

          }

          int winner=Winner(board);

          if(winner == 1)
          {
             cout << "In game No. " << i+1 << ": ";
             cout << "Winner Player : " << winner << endl;
             winCount_for1++;
          }
          else if(winner == 2)
           {
             cout << "In game No. " << i+1 << ": ";
             cout << "Winner Player : " << winner << endl;
             winCount_for2++;
           }
          else
          {
            cout << "In game No. " << i+1 << ": ";
            cout << "Result is : DRAW " << endl;
            tieCount++;
          }

         }

        cout << " Player 1 Win : " << winCount_for1 << endl;
        cout << " Player 2 Win : " << winCount_for2 << endl;
        cout << " Tie match : " << tieCount << endl;
        float win1 = winCount_for1;
        float win2 = winCount_for2;
        float ratio = 0.0;
        ratio = (win1/win2);
        cout << "The win loss ratio is : (player 1 Win / player 2 Win ) : ";
        cout << ratio << endl;

}


int main()
{
    srand(time(0));
    int type;
    int board[boardSize];
    for (int i=0; i < boardSize; i++)
      {
         board[i]=0;
      }

    initial_board(board);
    int max_heu;
    int min_heu;
    int max_depth;
    int min_depth;
    cout << "1. AI Vs AI" << endl;
    cout << "2. Human Vs AI" << endl;
    cout << "3. Human Vs Human" << endl;

    cout << "Enter the type " << endl;
    cin >> type;



    if(type==1)
    {
        cout << "Enter the heuristic for maximizing player" << endl;
        cin >> max_heu;
        cout << "Enter the heuristic for minimizing player" << endl;
        cin >> min_heu;
        cout << "Enter the depth for maximizing player" << endl;
        cin >> max_depth;
        cout << "Enter the depth for minimizing player" << endl;
        cin >> min_depth;
        cout << "Intial Board is : " << endl;
        showBoard(board);
        int winCount_for1 = 0;
        int winCount_for2 = 0;
        int tieCount = 0;

             while (true)
          {
            Node node = min_max_with_pruning(board,max_depth,-INT_MAX,INT_MAX,true,max_heu,0,0,0,0);
            cout << "Player 1 : " << node.bestMove << endl;
            pair<bool,int> returnPair=move(board,node.bestMove,1);
            bool val= returnPair.first;
            showBoard(board);
            if(is_game_over(board))
            {
                break;
            }
            if(val)
            {
                continue;
            }

            node = min_max_with_pruning(board,min_depth,-INT_MAX,INT_MAX,false,min_heu,0,0,0,0);
            cout << "Player  2 : " << node.bestMove << endl;
            cout << endl;
            returnPair= move(board,node.bestMove,2);
            val = returnPair.first;
            showBoard(board);

            if(is_game_over(board))
            {
                break;
            }

            while (val)
            {

                node = min_max_with_pruning(board,min_depth,-INT_MAX,INT_MAX,false,min_heu,0,0,0,0);
                cout << "Player  2 : " << node.bestMove << endl;
                cout << endl;
                returnPair= move(board,node.bestMove,2);
                val = returnPair.first;
                showBoard(board);

                if(is_game_over(board))
                {
                   break;
                }

            }
            if(is_game_over(board))
            {
                break;
            }

         }
          cout << "Final Board : " << endl;
          showBoard(board);
          int winner=Winner(board);

          if(winner == 1)
          {
             cout << "Winner is : Player " << winner << endl;
             winCount_for1++;
          }
          else if(winner == 2)
           {
             cout << "Winner is : Player " << winner << endl;
             winCount_for2++;
          }
          else
          {
             cout << " Result is : DRAW " << endl;
             tieCount++;
          }

          cout << "Do you want to see the report :[yes/no] " << endl;
          int h1;
          int h2;

          while(true)
          {

              string comment;
             cin >> comment;
             if(comment=="yes")
             {
               cout << "Enter heuristic number for maximizing player : " << endl;
               cin >>  h1 ;
               cout << "Enter heuristic number for the minimizing player : " << endl;
               cin >> h2;
               report(h1,h2);
               break;
             }
              else if(comment == "no")
             {
                cout << "Thank you " << endl;
                break;

             }
              else{
               cout << "You Entered : " << comment << endl;
               cout << "Please,enter yes or no  " << endl;
             }
          }


    }

    if (type == 2) {
           cout << "Initial Board: " << endl;
           showBoard(board);

            while (true) {
               // Player 1's Move
               int mov;
               bool isValidMove = false;
               do {
                    cout << "Enter your move: ";
                    cin >> mov;
                    isValidMove = isMoveValid(board, 1, mov);
                    if (!isValidMove) {
                  cout << "Invalid Move... Try again..." << endl;
                  }
              } while (!isValidMove);

               // Execute Player 1's Move
               pair<bool, int> moveResult = move(board, mov, 1);
               cout << endl;
               showBoard(board);

              // Check if the game is over
              if (is_game_over(board)) break;

              // Handle Player 1's free turn
              if (moveResult.first) {
                   cout << "Player 1 gets a free turn." << endl;
                  continue;
               }

               // Player 2 (AI) Move
              while (true) {
                  Node node = min_max_with_pruning(board, 10, -INT_MAX, INT_MAX, false, 4, 0, 0, 0, 0);
                  cout << "Player 2 (AI) chooses move: " << node.bestMove << endl << endl;

                   pair<bool, int> aiMoveResult = move(board, node.bestMove, 2);
                   bool aiGetsFreeTurn = aiMoveResult.first;
                   showBoard(board);

                   // Check if the game is over after AI's move
                   if (is_game_over(board)) break;

                   // Handle AI's free turn
                  if (!aiGetsFreeTurn) break;
                  cout << "Player 2 (AI) gets a free turn." << endl;
                 }

                 // Check again if the game is over after AI's move
                 if (is_game_over(board)) break;
            }


          cout << "Final Board :" << endl;
          showBoard(board);
          int winner = Winner(board);
         if (winner == 3) {
            cout << "Result: DRAW" << endl;
         } else {
           cout << "Winner: Player " << winner << endl;
         }
   }


    if(type==3)
    {
        cout << "Initial Board : " << endl;
        showBoard(board);
        bool maxPlay=1;
        while (true)
        {
            cout << "Player  " << 2-maxPlay ;
            cout << "   Enter your move " << endl;
            int mov;
            cin >> mov;
            bool checkValidity = isMoveValid(board,2-maxPlay,mov);
            if(!checkValidity)
            {
                cout << "Invalid Move ...." << endl;
                cout << "Try Again......" << endl;
            }
            else
            {
                pair<bool,int>returnPair = move(board,mov,2-maxPlay);
                bool val= returnPair.first;
                showBoard(board);
                if(val)
                {
                    cout << "Free Turn : " << endl;
                }
                if(is_game_over(board)) break;
                if(!val)
                {
                    maxPlay= !maxPlay;
                }
            }
        }
        cout << "Final Board :" << endl;
        showBoard(board);
        int win = Winner(board);
        if(win != 3)
        {
            cout << "Winner : Player " << win << endl;
        }
        else
        {
            cout << "Result : DRAW " << endl;
        }

    }


     return 0;

}
