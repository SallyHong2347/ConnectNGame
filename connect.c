#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

const int INVALID_GAME = -1;
const int TIE = 1;
const int MAX_BOARD_SIZE = 1024;
char horizontal = '-';
char vertical = '|';
char corner = '+';

bool connected2(int board[], int width, int row,
                int col, int length, int chess);
int win_rd_lu(int board[], int width, int height, int length);
bool connected(int board[], int width, int row,
               int col, int length, int chess);
int win_ld_ru(int board[], int width, int height, int length);
int win_c(int board[], int width, int height, int length);
int win_r(int board[], int width, int height, int length);
bool full_col(int board[], int width, int height, int col);
int empty_index(int board[], int width, int height, int col);
void print_board(int board[], int width, int height);

// print_board(board, width, height) print out the chess board
void print_board(int board[], int width, int height)
{
  printf("%c", corner);
  for (int w = 0; w < width; w++)
  {
    printf("%c", horizontal);
  }
  printf("%c\n", corner);

  for (int h = height - 1; h >= 0; h--)
  {
    printf("%c", vertical);

    for (int w = 0; w < width; w++)
    {
      int b = board[h * width + w];
      if (b != 0)
      {
        printf("%d", b);
      }
      else
      {
        printf(" ");
      }
    }

    printf("%c\n", vertical);
  }

  printf("%c", corner);
  for (int w = 0; w < width; w++)
  {
    printf("%c", horizontal);
  }
  printf("%c\n", corner);
}

// connect_analysis(moves, width, height, length) analyzes a
// game of connect on a board of size (width x height) with the
// given [column] moves and the player must get length in-a-row
// to win
// require: moves != NULL
//          2 <= width
//          2 <= height
//          width * height < MAX_BOARD_SIZE
//          length > 0
int connect_analysis(const int moves[], int width,
                     int height, int length)
{
  assert(moves != NULL);
  assert(width * height <= MAX_BOARD_SIZE);
  assert(width >= 2);
  assert(height >= 2);
  assert(length > 0);
  int board[1024] = {0};
  int count = 0;
  int p_col = -1;
  int invalid = 0;
  int tie = 0;
  int win = 0;
  while (true)
  {
    // print the chess board
    print_board(board, width, height);
    //read from move[]
    p_col = moves[count];
    //p_col valid
    if ((p_col < 0) || (p_col >= width))
    {
      invalid = 1;
      break;
    }
    // check if column is full
    bool col_full = full_col(board, width, height, p_col);
    if (col_full)
    {
      invalid = 1;
      break;
    }

    //input is valid, add count
    count++;
    // assign the chess value
    int put_index = empty_index(board, width, height, p_col);
    if (count % 2 == 1)
    {
      board[put_index] = count % 2;
    }
    else
    {
      board[put_index] = 2;
    }

    //check winning
    int row_win = win_r(board, width, height, length);
    int col_win = win_c(board, width, height, length);
    int ld_ru = win_ld_ru(board, width, height, length);
    int rd_lu = win_rd_lu(board, width, height, length);
    if ((row_win == 1) || (col_win == 1) ||
        (ld_ru == 1) || (rd_lu == 1))
    {
      win = 1;
      break;
    }
    else if ((row_win == 2) || (col_win == 2) ||
             (ld_ru == 2) || (rd_lu == 2))
    {
      win = 2;
      break;
    }

    if ((win == 0) && count == width * height)
    {
      tie = 1;
      break;
    }
  }
  print_board(board, width, height);
  //result
  if (win == 1)
  {
    return count;
  }
  else if (win == 2)
  {
    return -1 * count;
  }
  else if (tie != 0)
  {
    return TIE;
  }
  else
  {
    return INVALID_GAME;
  }
}

// win_r(board, width, height, length) check if there is a
// winner in row
// require: board != NULL
//          2 <= width
//          2 <= height
//          width * height < MAX_BOARD_SIZE
//          length > 0
int win_r(int board[], int width, int height, int length)
{
  assert(board != NULL);
  assert(width * height <= MAX_BOARD_SIZE);
  assert(width >= 2);
  assert(height >= 2);
  assert(length > 0);

  int count_row_1 = 0;
  int count_row_2 = 0;
  int winner_r = 0;
  for (int row = 0; row < height; row++)
  {
    for (int col = 0; col < width; col++)
    {
      //count number of chess
      if (board[row * width + col] == 1)
      {
        count_row_2 = 0;
        count_row_1++;
      }
      else if (board[row * width + col] == 2)
      {
        count_row_1 = 0;
        count_row_2++;
      }
      else
      {
        count_row_1 = 0;
        count_row_2 = 0;
      }
      //check if win
      //p1 win, return 1, p2 win, return 2
      if (count_row_1 == length)
      {
        winner_r = 1;
        break;
      }
      if (count_row_2 == length)
      {
        winner_r = 2;
        break;
      }
    }
    // if there is a winner, break
    if (winner_r != 0)
    {
      break;
    }
    //reset count
    count_row_1 = 0;
    count_row_2 = 0;
  }
  return winner_r;
}

// win_c(board, width, height, length) check if there is a
// winner from column
// require: board != NULL
//          2 <= width
//          2 <= height
//          width * height < MAX_BOARD_SIZE
//          length > 0
int win_c(int board[], int width, int height, int length)
{
  assert(board != NULL);
  assert(width * height <= MAX_BOARD_SIZE);
  assert(width >= 2);
  assert(height >= 2);
  assert(length > 0);

  int count_col_1 = 0;
  int count_col_2 = 0;
  int winner_c = 0;
  for (int col = 0; col < width; col++)
  {
    for (int row = 0; row < height; row++)
    {
      // count number of chess
      if (board[row * width + col] == 1)
      {
        count_col_2 = 0;
        count_col_1++;
      }
      else if (board[row * width + col] == 2)
      {
        count_col_1 = 0;
        count_col_2++;
      }
      else
      {
        count_col_1 = 0;
        count_col_2 = 0;
      }
      //check if win
      //p1 win, return 1, p2 win, return 2
      if (count_col_1 == length)
      {
        winner_c = 1;
        break;
      }
      if (count_col_2 == length)
      {
        winner_c = 2;
        break;
      }
    }
    //if winner exist, break
    if (winner_c != 0)
    {
      break;
    }
    //rest counts
    count_col_1 = 0;
    count_col_2 = 0;
  }
  return winner_c;
}

// win_ld_ru(board, width, height, length) return the winner
// that reach length in diagnose from left bottom to up right
// in the board
// require: board != NULL
//          2 <= width
//          2 <= height
//          width * height < MAX_BOARD_SIZE
//          length > 0
int win_ld_ru(int board[], int width, int height, int length)
{
  assert(board != NULL);
  assert(width * height <= MAX_BOARD_SIZE);
  assert(width >= 2);
  assert(height >= 2);
  assert(length > 0);

  bool p1 = false;
  bool p2 = false;
  int winner = 0;
  int max_col = width - length;  // max beginning column
  int max_row = height - length; // max beginning row
  for (int row = 0; row <= max_row; row++)
  {
    for (int col = 0; col <= max_col; col++)
    {
      p1 = connected(board, width, row, col, length, 1);
      p2 = connected(board, width, row, col, length, 2);
      if (p1)
      {
        winner = 1;
        break;
      }
      else if (p2)
      {
        winner = 2;
        break;
      }
    }
    if (winner != 0)
    {
      break;
    }
  }
  return winner;
}

// connected(board, width, row, col, length, chess) check
// if one dignose, from left bottom to top right, start from
// (row, col) has the number of chess equals to length
// require: board != NULL
//          2 <= width
//          length > 0
//          chess equals to 1 or 2
bool connected(int board[], int width, int row,
               int col, int length, int chess)
{
  assert(board);
  assert(width > 0);
  assert((chess == 1) || (chess == 2));

  bool connect = true;
  int shift = 0;
  while (length != 0)
  {
    if (board[(row + shift) * width + col + shift] == chess)
    {
      length--;
      shift++;
    }
    else
    {
      connect = false;
      break;
    }
  }
  return connect;
}

// win_rd_lu(board, width, height, length) return the winner
// that reach length in diagnose from top left to right bottom
// in the board
// require: board != NULL
//          2 <= width
//          2 <= height
//          width * height < MAX_BOARD_SIZE
//          length > 0
int win_rd_lu(int board[], int width, int height, int length)
{
  assert(board != NULL);
  assert(width * height <= MAX_BOARD_SIZE);
  assert(width >= 2);
  assert(height >= 2);
  assert(length > 0);

  bool p1 = false;
  bool p2 = false;
  int winner = 0;
  int min_col = length - 1;      //min starting col
  int max_row = height - length; //max starting row
  for (int row = 0; row <= max_row; row++)
  {
    for (int col = min_col; col < width; col++)
    {
      p1 = connected2(board, width, row, col, length, 1);
      p2 = connected2(board, width, row, col, length, 2);
      if (p1)
      {
        winner = 1;
        break;
      }
      else if (p2)
      {
        winner = 2;
        break;
      }
    }
    if (winner != 0)
    {
      break;
    }
  }
  return winner;
}

// connected2(board, width, row, col, length, chess) check
// if one dignose, from top left to down right, start from
// (row, col) has the number of chess equals to length
// require: board != NULL
//          2 <= width
//          length > 0
//          chess equals to 1 or 2
bool connected2(int board[], int width, int row,
                int col, int length, int chess)
{
  assert(board);
  assert(width > 0);
  assert((chess == 1) || (chess == 2));

  bool connect = true;
  int shift = 0;
  while (length != 0)
  {
    if (board[(row + shift) * width + col - shift] == chess)
    {
      length--;
      shift++;
    }
    else
    {
      connect = false;
      break;
    }
  }
  return connect;
}

// full_col(board, width, height, col) check if the col is
// full in board
// require: board != NULL
//          2 <= width
//          2 <= height
//          width * height < MAX_BOARD_SIZE
//          0 <= col < width
bool full_col(int board[], int width, int height, int col)
{
  assert(board != NULL);
  assert(width * height <= MAX_BOARD_SIZE);
  assert(width >= 2);
  assert(height >= 2);
  assert(col >= 0);
  assert(col < width);
  bool full = true;
  for (int row = 0; row < height; row++)
  {
    if (board[row * width + col] == 0)
    {
      full = false;
      break;
    }
  }
  return full;
}

// empty_index(board, width, height, col) return the next
// empty index in the col in the board
// require: board != NULL
//          2 <= width
//          2 <= height
//          width * height < MAX_BOARD_SIZE
//          0 <= col < width
int empty_index(int board[], int width,
                int height, int col)
{
  assert(board != NULL);
  assert(width * height <= MAX_BOARD_SIZE);
  assert(width >= 2);
  assert(height >= 2);
  assert(col >= 0);
  assert(col < width);
  int row = 0;
  for (row = 0; row < height; row++)
  {
    if (board[row * width + col] == 0)
    {
      break;
    }
  }
  return row * width + col;
}
