#include <stdio.h>
#include <ctype.h>

char board[3][3];
const char PLAYER = 'X';
const char COMPUTER = 'O';

void computer_move();
void print_winner(char winner);

void init_board() {
    for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
    board[i][j] = '_';
}

void print_board() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++)
        printf("| %c ", board[i][j]);
        printf("|\n");
    }
}

void update_board(int pos, char player) {
    int i = (pos - 1) / 3;
    int j = (pos - 1) % 3;

    board[i][j] = player;
}

int is_empty(int pos) {
    int i = (pos - 1) / 3;
    int j = (pos - 1) % 3;
    
    return board[i][j] == '_';
}

int check_white_space() {
    for (int i = 1; i <= 9; i++)
    if (is_empty(i))
    return 1;
    
    return 0;
}

char check_winner() {
    int winning_conditions[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8},
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8},
        {0, 4, 8}, {2, 4, 6}
    };

    for (int i = 0; i < 8; i++) {
        int a = winning_conditions[i][0];
        int b = winning_conditions[i][1];
        int c = winning_conditions[i][2];

        for (int j = 0; j < 2; j++) {
            char player = (j == 0) ? PLAYER : COMPUTER;

            if (board[a / 3][a % 3] == player &&
                board[b / 3][b % 3] == player && 
                board[c / 3][c % 3] == player)
            return player;
        }
    }

    return ' ';
}

void player_move() {
    int pos = -1;
    
    while (pos < 1 || pos > 9 || !is_empty(pos)) {
        printf("Enter a position (1-9): ");
        scanf("%d", &pos);
        
        if (pos < 1 || pos > 9)
        printf("Invalid position.\n");
        else if (!is_empty(pos))
        printf("Position already taken\n");

        while(getchar() != '\n');
    }

    update_board(pos, PLAYER);
}

int minimax(char board[3][3], int is_maximizing) {
    char winner = check_winner();

    if (winner == COMPUTER) return 1;
    if (winner == PLAYER) return -1;
    if (!check_white_space()) return 0;

    if (is_maximizing) {
        int best_score = -1000;

        for (int i = 1; i <= 9; i++) {
            if (is_empty(i)) {
                update_board(i, COMPUTER);
                int score = minimax(board, 0);
                update_board(i, '_');

                if (score > best_score) 
                best_score = score;
            }
        }

        return best_score;
    } else {
        int best_score = 1000;

        for (int i = 1; i <= 9; i++) {
            if (is_empty(i)) {
                update_board(i, PLAYER);
                int score = minimax(board, 1);
                update_board(i, '_');

                if (score < best_score) 
                best_score = score;
            }
        }

        return best_score;
    }
}

void computer_move() {
    int best_score = -1000;
    int move = -1;

    for (int i = 1; i <= 9; i++) {
        if (is_empty(i)) {
            update_board(i, COMPUTER);
            int score = minimax(board, 0);
            update_board(i, '_');

            if (score > best_score) {
                best_score = score;
                move = i;
            }
        }
    }
    update_board(move, COMPUTER);
}

void print_winner(char winner) {
    if (winner == PLAYER)
    printf("Player wins!\n");
    else if (winner == COMPUTER)
    printf("Computer wins!\n");
    else
    printf("It's a draw!\n");
}

int main() {
    char winner = ' ';

    printf("\e[1;1H\e[2J");
    init_board();
    print_board();

    while(winner == ' ' && check_white_space()) {
        player_move();
        winner = check_winner();

        if (winner != ' ' || !check_white_space())
        break;
        printf("\e[1;1H\e[2J");

        computer_move();
        print_board();
        winner = check_winner();
    }

    printf("\e[1;1H\e[2J");
    print_board();
    print_winner(winner);

    return 0;
}