
#include <assert.h>
#include <stdio.h>
#include "connect.h"

int main(void)
{
  // player1_wins_move_7 is the movemoves (starting from player1)
  // that will let the first player win at the 7th move
  // the number in the array represent the columns that the player choose to place their chess
  // the chess will fall to the bottom of the board
  // the first move is done by player1 (1), the second move is done by player2(2)
  printf("the following is result for the first game:\n Player 1 wins\n");
  int player1_wins_move_7[42] = {1, 2, 1, 2, 1, 2, 1};
  assert(connect_analysis(player1_wins_move_7, 7, 6, 4) == 7);

  // the following moves (starting fro player1) will left the second player win at the 8th move
  printf("the following is result for the second game:\n Player 2 wins\n");
  int player2_wins_move_8[42] = {0, 1, 0, 1, 0, 1, 3, 1};
  assert(connect_analysis(player2_wins_move_8, 7, 6, 4) == -8);

  // in the following move, neither player one nor player 2 wins
  printf("the following is result for the third game:\n Neither player wins\n");
  int tie1[42] = {0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 6, 6,
                  2, 3, 2, 3, 3, 2, 3, 2, 2, 3, 2, 3, 6, 6,
                  4, 5, 4, 5, 5, 4, 5, 4, 4, 5, 4, 5, 6, 6};
  assert(connect_analysis(tie1, 7, 6, 4) == TIE);
}
