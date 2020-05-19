/*-------------------------------------------------------------------------
    Include files:
-------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdbool.h>

/*-------------------------------------------------------------------------
    Constants and definitions:
-------------------------------------------------------------------------*/
#define N 11
#define NO_PLAYER_INDEX 0
#define FIRST_PLAYER_INDEX 1
#define SECOND_PLAYER_INDEX 2
#define PLAYER_INDEX_BASE 3
#define FIRST_PLAYER_CHAR 'X'
#define SECOND_PLAYER_CHAR 'O'
#define EMPTY_BOARD_CHAR '_'
#define HIST_ROWS 2
#define HIST_ROW_INDICATOR 0
#define HIST_COL_INDICATOR 1
#define EVEN_ODD_DIVISOR 2
#define SEC_DIAG_CHECK 2

/*-------------------------------------------------------------------------
    Function declaration
-------------------------------------------------------------------------*/
void print_welcome();

void print_enter_board_size();

void print_board(char board[N][N], int n);

void print_player_turn(int player_index);

void print_error();

void print_winner(int player_index);

void print_tie();

int play_game(int board_size);

int continue_game(char board[N][N], int board_size, int move_counter, int chosen_row, int curr_player, int move_history[HIST_ROWS][N * N]);

int player_turn(char board[N][N], int board_size, int move_counter, int chosen_row, int chosen_column, int curr_player,
                int move_history[HIST_ROWS][N * N]);

int perform_turn(char board[N][N], int board_size, int move_counter, int chosen_row, int chosen_column, int curr_player,
                 int move_history[HIST_ROWS][N * N]);

int perform_undo(char board[N][N], int board_size, int move_counter, int chosen_row, int curr_player, int move_history[HIST_ROWS][N * N]);

int check_winner(char board[N][N], int board_size);

int check_winner_by_row(char board[N][N], int board_size);

int check_winner_by_column(char board[N][N], int board_size);

int check_winner_by_diagonal(char board[N][N], int board_size);

int check_winner_by_secondary_diagonal(char board[N][N], int board_size);

bool check_legal_turn(char board[N][N], int board_size, int row, int column);

bool is_board_full(char board[N][N], int board_size);

/*-------------------------------------------------------------------------
    This is a game of AVOIDANCE TIC-TAC-TOE
-------------------------------------------------------------------------*/
/**
 * length: 11 lines.
 * Prints welcome text, will take as input the size of the
 * board and start the game.
 * @return The exit code of the program
 */
int main() {
    int board_size = 0, winner = 0;
    print_welcome();
    print_enter_board_size();
    scanf("%d", &board_size);
    winner = play_game(board_size);
    if (winner == 0) {
        print_tie();
    } else if (winner == FIRST_PLAYER_INDEX) {
        print_winner(FIRST_PLAYER_INDEX);
    } else {
        print_winner(SECOND_PLAYER_INDEX);
    }
    return 0;
}

/**
 * length: 8 lines.
 * Setup the board and ask the first player to perform his turn.
 * @param board_size The size of the game board
 * @return The index of the winning player
 */
int play_game(int board_size) {
    char board[N][N];
    // move_history saves every turn as a row and column pair in the 2 rows of the history array.
    int move_history[HIST_ROWS][N * N];
    for (int i = 0; i < board_size; i++) {
        for (int j = 0; j < board_size; j++) {
            board[i][j] = EMPTY_BOARD_CHAR;
        }
    }
    print_board(board, board_size);
    print_player_turn(FIRST_PLAYER_INDEX);

    return continue_game(board, board_size, 0, 0, FIRST_PLAYER_INDEX, move_history);
}

/**
 * length: 5 lines.
 * Continues the game loop according to input.
 * @param board The game board
 * @param board_size The board size
 * @param move_counter The counter for performed moves
 * @param chosen_row The selected row
 * @param curr_player The index of the current player
 * @param move_history The array storing the move history
 * @return Calls the next action in the game loop, will return the index of the winner up the function stack
 */
int continue_game(char board[N][N], int board_size, int move_counter, int chosen_row, int curr_player, int move_history[HIST_ROWS][N * N]) {
    int chosen_column = 0;
    scanf("%d", &chosen_row);
    while (!is_board_full(board, board_size)) {
        return player_turn(board, board_size, move_counter, chosen_row, chosen_column, curr_player, move_history);
    }

    return 0;
}

/**
 * length: 10 lines.
 * Checks the input from the player and performs or reverts turns accordingly.
 * @param board The game board
 * @param board_size The board size
 * @param move_counter The counter for performed moves
 * @param chosen_row The selected row
 * @param chosen_column The selected column
 * @param curr_player The index of the current player
 * @param move_history The array storing the move history
 * @return Calls the next action in the game loop, will return the index of the winner up the function stack
 */
int player_turn(char board[N][N], int board_size, int move_counter, int chosen_row, int chosen_column, int curr_player,
                int move_history[HIST_ROWS][N * N]) {
    if (chosen_row < 0) {
        if (-1 * chosen_row % EVEN_ODD_DIVISOR == 1 && -1 * chosen_row <= move_counter) {
            return perform_undo(board, board_size, move_counter, chosen_row, curr_player, move_history);
        }
        print_error();
        return continue_game(board, board_size, move_counter, chosen_row, curr_player, move_history);
    } else {
        scanf("%d", &chosen_column);
    }
    if (check_legal_turn(board, board_size, chosen_row, chosen_column)) {
        return perform_turn(board, board_size, move_counter, chosen_row, chosen_column, curr_player, move_history);
    } else {
        print_error();
    }
    return continue_game(board, board_size, move_counter, chosen_row, curr_player, move_history);
}

/**
 * length: 12 lines.
 * Modifies the game board according to the turn specified by the player.
 * @param board The game board
 * @param board_size The board size
 * @param move_counter The number of moves performed until now
 * @param chosen_row The target row for the turn
 * @param chosen_column The target column for the turn
 * @param curr_player The index of the current player
 * @param move_history The array storing the move history
 * @return Calls the next action in the game loop, will return the index of the winner up the function stack
 */
int perform_turn(char board[N][N], int board_size, int move_counter, int chosen_row, int chosen_column, int curr_player,
                 int move_history[HIST_ROWS][N * N]) {
    board[chosen_row - 1][chosen_column - 1] = (curr_player == FIRST_PLAYER_INDEX) ? FIRST_PLAYER_CHAR : SECOND_PLAYER_CHAR;
    // Saving the move in following format: the row is saved in the first row, the column is saved in the second row.
    move_history[HIST_ROW_INDICATOR][move_counter] = chosen_row - 1;
    move_history[HIST_COL_INDICATOR][move_counter] = chosen_column - 1;
    move_counter++;
    print_board(board, board_size);
    curr_player = PLAYER_INDEX_BASE - curr_player;
    if (check_winner(board, board_size) != 0) {
        return check_winner(board, board_size);
    }
    if (is_board_full(board, board_size)) {
        return 0;
    }
    print_player_turn(curr_player);
    return continue_game(board, board_size, move_counter, chosen_row, curr_player, move_history);
}

/**
 * length: 8 lines.
 * Reverts the board to the correct older state.
 * @param board The game board
 * @param board_size The board size
 * @param move_counter The number of moves performed until now
 * @param chosen_row The row to undo turns from
 * @param curr_player The index of the current player
 * @param move_history The array storing the move history
 * @return Calls the next action in the game loop, will return the index of the winner up the function stack
 */
int perform_undo(char board[N][N], int board_size, int move_counter, int chosen_row, int curr_player, int move_history[HIST_ROWS][N * N]) {
    int moves_before_undo = move_counter - 1;
    for (int i = moves_before_undo; i > moves_before_undo + chosen_row; i--) {
        move_counter--;
        board[move_history[HIST_ROW_INDICATOR][i]][move_history[HIST_COL_INDICATOR][i]] = EMPTY_BOARD_CHAR;
    }
    print_board(board, board_size);
    curr_player = PLAYER_INDEX_BASE - curr_player;
    print_player_turn(curr_player);
    return continue_game(board, board_size, move_counter, chosen_row, curr_player, move_history);
}

/**
 * length: 13 lines.
 * Checks the board for a winner.
 * @param board The game board
 * @param board_size The board size
 * @return the index of the winning player, or 0 if no player has won yet.
 */
int check_winner(char board[N][N], int board_size) {
    int diag_win = check_winner_by_diagonal(board, board_size);
    int sec_diag_win = check_winner_by_secondary_diagonal(board, board_size);
    int row_win = check_winner_by_row(board, board_size);
    int column_win = check_winner_by_column(board, board_size);

    if (diag_win != 0) {
        return diag_win;
    } else if (sec_diag_win != 0) {
        return sec_diag_win;
    } else if (row_win != 0) {
        return row_win;
    } else if (column_win != 0) {
        return column_win;
    }
    return NO_PLAYER_INDEX;
}

/**
 * length: 11 lines.
 * Checks the rows of the board for a winner.
 * @param board The game board
 * @param board_size The board size
 * @return the index of the winning player, or 0 if no player has won yet.
 */
int check_winner_by_row(char board[N][N], int board_size) {
    int streak_length = 0;
    for (int i = 0; i < board_size; i++) {
        for (int j = 0; j < board_size - 1; j++) {
            if (board[i][j] == board[i][j + 1] && board[i][j] != EMPTY_BOARD_CHAR) {
                streak_length++;
            }
        }
        if (streak_length == board_size - 1) {
            if (board[i][0] == FIRST_PLAYER_CHAR) {
                return SECOND_PLAYER_INDEX;
            } else {
                return FIRST_PLAYER_INDEX;
            }
        } else {
            streak_length = 0;
        }
    }
    return NO_PLAYER_INDEX;
}

/**
 * length: 11 lines.
 * Checks the columns of the board for a winner.
 * @param board The game board
 * @param board_size The board size
 * @return the index of the winning player, or 0 if no player has won yet.
 */
int check_winner_by_column(char board[N][N], int board_size) {
    int streak_length = 0;
    for (int i = 0; i < board_size; i++) {
        for (int j = 0; j < board_size - 1; j++) {
            if (board[j][i] == board[j + 1][i] && board[j][i] != EMPTY_BOARD_CHAR) {
                streak_length++;
            }
        }
        if (streak_length == board_size - 1) {
            if (board[0][i] == FIRST_PLAYER_CHAR) {
                return SECOND_PLAYER_INDEX;
            } else {
                return FIRST_PLAYER_INDEX;
            }
        } else {
            streak_length = 0;
        }
    }
    return NO_PLAYER_INDEX;
}

/**
 * length: 9 lines.
 * Checks the primary diagonal of the board for a winner
 * @param board The game board
 * @param board_size The board size
 * @return the index of the winning player, or 0 if no player has won yet.
 */
int check_winner_by_diagonal(char board[N][N], int board_size) {
    int streak_length = 0;
    for (int i = 0; i < board_size - 1; i++) {
        if (board[i][i] == board[i + 1][i + 1] && board[i][i] != EMPTY_BOARD_CHAR) {
            streak_length++;
        }
    }
    if (streak_length == board_size - 1) {
        if (board[0][0] == FIRST_PLAYER_CHAR) {
            return SECOND_PLAYER_INDEX;
        } else {
            return FIRST_PLAYER_INDEX;
        }
    }
    return NO_PLAYER_INDEX;
}

/**
 * length: 9 lines.
 * Checks the secondary diagonal of the board for a winner
 * @param board The game board
 * @param board_size The board size
 * @return the index of the winning player, or 0 if no player has won yet.
 */
int check_winner_by_secondary_diagonal(char board[N][N], int board_size) {
    int streak_length = 0;
    for (int i = 0; i < board_size - 1; i++) {
        if (board[i][board_size - i - 1] == board[i + 1][board_size - i - SEC_DIAG_CHECK] && board[i][board_size - i - 1] != EMPTY_BOARD_CHAR) {
            streak_length++;
        }
    }
    if (streak_length == board_size - 1) {
        if (board[board_size - 1][0] == FIRST_PLAYER_CHAR) {
            return SECOND_PLAYER_INDEX;
        } else {
            return FIRST_PLAYER_INDEX;
        }
    }
    return NO_PLAYER_INDEX;
}

/**
 * length: 5 lines.
 * Checks if the requested move is legal
 * @param board The game board
 * @param board_size The board size
 * @param row The row of the target move
 * @param column The column of the target move
 * @return true if the move requested is legal, false otherwise.
 */
bool check_legal_turn(char board[N][N], int board_size, int row, int column) {
    // Checking that the row and column specified are within the legal ranges of the board.
    if (row > board_size || column > board_size || row < 1 || column < 1) {
        return false;
    } else if (board[row - 1][column - 1] != EMPTY_BOARD_CHAR) {
        return false;
    }
    return true;
}

/**
 * length: 5 lines.
 * Checks if the board is full
 * @param board The game board
 * @param board_size The size of the game board
 * @return true of the board is full, false otherwise.
 */
bool is_board_full(char board[N][N], int board_size) {
    for (int i = 0; i < board_size; i++) {
        for (int j = 0; j < board_size; j++) {
            if (board[i][j] == EMPTY_BOARD_CHAR) {
                return false;
            }
        }
    }
    return true;
}

//print welcome message
//1 lines
void print_welcome() {
    printf("*** Welcome to AVOIDANCE TIC-TAC-TOE game ***\n\n");
}

//print message to enter board size
//1 lines
void print_enter_board_size() {
    printf("Please enter board size (1 to %d):\n", N);
}

//print the board
//7 lines
void print_board(char board[N][N], int n) {
    printf("\nCurrent board:\n");
    for (int i = 0; i < n; i++) {
        printf("|");
        for (int j = 0; j < n; j++) {
            printf("%c|", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

//print a request for player with index "player_index" to move
//1 lines
void print_player_turn(int player_index) {
    printf("\nPlayer ** %d **, enter next move:\n", player_index);
}

//print error message
//1 lines
void print_error() {
    printf("Illegal move!!!, please try again:\n");
}

//print the winner
//1 lines
void print_winner(int player_index) {
    printf("Player %d Wins! Hooray!\n", player_index);
}

//print message to announce there is a tie (no one wins)
//1 lines
void print_tie() {
    printf("It's a tie!\n");
}
