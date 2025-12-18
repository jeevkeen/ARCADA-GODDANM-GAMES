#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "games.h"
#include "utilits.h"

// ===========================
// |        LUCKY GAME       |
// ===========================

int start_lucky_game(int* points, int difficulty) {
    int user_number, computer_number;
    int min_val = 1, max_val = 10;

    // Game difficulty settings (numbers values)
    if (difficulty == 1)
        max_val = 5;
    else if (difficulty == 2)
        max_val = 10;
    else
        max_val = 20;

    computer_number = min_val + rand() % (max_val - min_val + 1);

//--------------------------------------------------------------------------------------
//debug mode (ON/OFF in utilits.h)
#ifdef DEBUG
    printf("\n[DEBUG] Computer number: %d\n", computer_number);
#endif
//--------------------------------------------------------------------------------------

    printf("Lucky Game! Guess number between %d and %d: ", min_val, max_val);


    scanf_s("%d", &user_number);

    if (computer_number == user_number) {
        printf("YOU WIN!\n+10 points\n");
        return 1;
    }
    else {
        printf("You lost. My number was %d.\n-5 points\n", computer_number);
        return 0;
    }
}


// =================================
// |   MEMO GAME (card game)       |
// =================================

void generate_game_field(char* game_field, int cards_amount) {
    char current_symbol = 'A';
    // Fill cards "A A B B..." in array
    for (int i = 0; i < cards_amount; i += 2) {
        game_field[i] = current_symbol;
        game_field[i + 1] = current_symbol;
        current_symbol++;
    }
}

void shuffle_game_field(char* game_field, int cards_amount) {
    // shuffle cards
    for (int i = cards_amount - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char temp = game_field[i];
        game_field[i] = game_field[j];
        game_field[j] = temp;
    }
}

void fill_user_field(char* user_field, int cards_amount) {
    for (int i = 0; i < cards_amount; i++) {
        user_field[i] = '*'; // back of a closed card (const)
    }
}

// show field as grid
void show_user_field(char* user_field, int cards_amount) {
    int cols = 4; // cards in a row for Easy and Medium mode (8 and 16 cards)

    if (cards_amount == 24) // cards in a row (6) for Hard mode (24 cards)
        cols = 6;

    printf("\n");
    for (int i = 0; i < cards_amount; i++) {
        printf("[%c]\t", user_field[i]);
        if ((i + 1) % cols == 0) {              // end row with 'cols' elements (there is 4 or 6 cards in line)
            printf("\n");
            for (int k = i - cols + 1; k <= i; k++) {  // sign number for every card
                printf(" %d\t", k + 1);
            }
            printf("\n\n");
        }
    }
}

void get_card_number(int* card_num, int cards_amount, char* user_field) {
    while (1) {
        printf("Input card number (1-%d): ", cards_amount);
        scanf_s("%d", card_num);

        // Is correct card number?
        if (*card_num < 1 || *card_num > cards_amount) {
            printf("Invalid number.\n");
            continue;
        }

        // Is card open?
        if (user_field[*card_num - 1] != '*') {
            printf("Card already open!\n");
            continue;
        }
        break;
    }
}

// open card in the user field
void open_card(int card_num, char* user_field, char* game_field) {
    user_field[card_num - 1] = game_field[card_num - 1];
}

// compare 2 cards (Is pair?)
int is_pair(int first_card, int second_card, char* game_field) {
    return game_field[first_card - 1] == game_field[second_card - 1];
}

// close card in the user field
void close_card(int card_num, char* user_field) {
    user_field[card_num - 1] = '*';
}

int start_find_game(int difficulty) {
    int cards_amount = 0;

    // card amount from the difficulty level
    if (difficulty == 1) cards_amount = 8;        // Easy
    else if (difficulty == 2) cards_amount = 16;  // Medium
    else cards_amount = 24;                       // Hard

    // Memory allocation
    char* game_field = (char*)malloc(sizeof(char) * cards_amount);
    char* user_field = (char*)malloc(sizeof(char) * cards_amount);


    int found_pairs = 0;

    generate_game_field(game_field, cards_amount);
    shuffle_game_field(game_field, cards_amount);
    fill_user_field(user_field, cards_amount);

    // Main game cycle
    while (found_pairs < cards_amount / 2) {
        int first_card, second_card;

        system("cls");
        printf("MEMO GAME | Found: %d/%d\n", found_pairs, cards_amount / 2);
        show_user_field(user_field, cards_amount);

// --- DEBUG MODE --------------------------------------------------------
#ifdef DEBUG
        printf("--- DEBUG ANSWERS ---\n");
        show_user_field(game_field, cards_amount);
        printf("---------------------\n");
#endif
//------------------------------------------------------------------------

        // ATTEMP 1
        get_card_number(&first_card, cards_amount, user_field);
        open_card(first_card, user_field, game_field);

        system("cls");
        printf("MEMO GAME | Card 1 selected.\n");
        show_user_field(user_field, cards_amount);

// --- DEBUG MODE --------------------------------------------------------
#ifdef DEBUG
        printf("--- DEBUG ANSWERS ---\n");
        show_user_field(game_field, cards_amount);
        printf("---------------------\n");
#endif
//------------------------------------------------------------------------

        // ATTEMP 2
        get_card_number(&second_card, cards_amount, user_field);
        open_card(second_card, user_field, game_field);

        system("cls");
        printf("MEMO GAME | Card 2 selected.\n");
        show_user_field(user_field, cards_amount);

        // --- CHECK FOR MATCHES ---
        if (is_pair(first_card, second_card, game_field)) {
            printf("MATCH! Great job.\n");
            found_pairs++;
            Sleep(1000);
        }
        else {
            printf("NO MATCH. Memorize positions...\n");
            Sleep(3000); // PAUSE TO MEMORIZE CARDS POSITION
            close_card(first_card, user_field);
            close_card(second_card, user_field);
        }
    }

    printf("CONGRATULATIONS! You completed the game.\n+20 points\n");

    // Clearing memory
    free(game_field);
    free(user_field);

    return 1;
}


// ==========================================
// |             MINESWEEPER                |
// ==========================================

// Function for moving the cursor (so that the screen does not flash)
void setCursor(int line, int column) {
    COORD coord;
    coord.X = column;
    coord.Y = line;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void free_field(char** field, int rows) {
    for (int i = 0; i < rows; i++) {
        free(field[i]);
    }
    free(field);
}

char** create_field(int rows, int cols) {
    char** field = (char**)malloc(sizeof(char*) * rows);
    for (int i = 0; i < rows; i++) {
        field[i] = (char*)malloc(sizeof(char) * cols);
    }
    return field;
}

void generate_mines_logic(char** hidden_field, int rows, int cols, int mines) {
    // 1. Clear
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) hidden_field[i][j] = '0';
    }

    // 2. Place mines
    int placed_mines = 0;
    while (placed_mines < mines) {
        int r = rand() % rows;
        int c = rand() % cols;

        if (hidden_field[r][c] != '*') {
            hidden_field[r][c] = '*';
            placed_mines++;

            // 3. Amount of mines around
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    int nx = r + dx;
                    int ny = c + dy;
                    if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && hidden_field[nx][ny] != '*') {
                        hidden_field[nx][ny]++;
                    }
                }
            }
        }
    }
}

// Show field
void render_full_field(char** display_field, char** hidden_field, int rows, int cols, int top_margin) {
    setCursor(top_margin, 0);

    printf("   ");
    for (int j = 0; j < cols; j++) printf(" %2d ", j + 1);
    printf("\n");

    printf("   ");
    for (int j = 0; j < cols; j++) printf("----");
    printf("\n");

    for (int i = 0; i < rows; i++) {
        printf("%2d |", i + 1);
        for (int j = 0; j < cols; j++) {
            char symbol = display_field[i][j];
//==========================================================================================
#ifdef DEBUG
            if (hidden_field[i][j] == '*') symbol = '*';
#endif
//==========================================================================================
            printf(" %c |", symbol);
        }
        printf("\n");
        printf("   ");
        for (int j = 0; j < cols; j++) printf("----");
        printf("\n");
    }
}

// Update one cell in the field
void update_cell_on_screen(int row, int col, char new_val, int top_margin) {
    int cursor_y = top_margin + 2 + (row * 2);
    int cursor_x = 5 + (col * 4);
    setCursor(cursor_y, cursor_x);
    printf("%c", new_val);
}

// Recurssion (open)
void open_cells_recursive(int r, int c, char** hidden_field, char** display_field, int rows, int cols, int* opened_counter, int top_margin) {
    if (r < 0 || r >= rows || c < 0 || c >= cols) return;
    if (display_field[r][c] != '#') return; // If it's already open or have a flag

    display_field[r][c] = hidden_field[r][c];
    if (display_field[r][c] == '0') display_field[r][c] = ' ';

    update_cell_on_screen(r, c, display_field[r][c], top_margin);
    (*opened_counter)++;

    if (hidden_field[r][c] == '0') {
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx != 0 || dy != 0) {
                    open_cells_recursive(r + dx, c + dy, hidden_field, display_field, rows, cols, opened_counter, top_margin);
                }
            }
        }
    }
}

int start_minesweeper(int difficulty) {
    int rows, cols, mines;

    if (difficulty == 1) { rows = 6; cols = 6; mines = 5; }
    else if (difficulty == 2) { rows = 10; cols = 10; mines = 15; }
    else { rows = 12; cols = 15; mines = 30; }

    char** hidden_field = create_field(rows, cols);
    char** display_field = create_field(rows, cols);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) display_field[i][j] = '#';
    }
    generate_mines_logic(hidden_field, rows, cols, mines);

    int game_over = 0, win = 0;
    int safe_cells_total = (rows * cols) - mines;
    int safe_cells_opened = 0;
    int flags_placed = 0, correct_flags = 0;
    int top_margin = 2;

    system("cls");
    printf("MINESWEEPER | Mines: %d\n", mines);
    render_full_field(display_field, hidden_field, rows, cols, top_margin);

    while (!game_over) {
        // Setting cursor under field
        setCursor(top_margin + (rows * 2) + 2, 0);
        printf("                                                ");
        setCursor(top_margin + (rows * 2) + 2, 0);
        printf("Action (Row Col o/f): "); // Enter info tip

        int r, c;
        char action;

        // ENTER THE COORDS
        if (scanf_s("%d %d %c", &r, &c, &action, 1) != 3) {
            while (getchar() != '\n'); // clear if in input were trash
            continue;
        }

        r--; c--; // Человек вводит 1, а в массиве это 0

        if (r < 0 || r >= rows || c < 0 || c >= cols) continue;

        // Flag action
        if (action == 'f' || action == 'F') {
            if (display_field[r][c] == '#') {
                display_field[r][c] = 'F';
                flags_placed++;
                if (hidden_field[r][c] == '*') correct_flags++;
                update_cell_on_screen(r, c, 'F', top_margin);
            }
            else if (display_field[r][c] == 'F') {
                display_field[r][c] = '#';
                flags_placed--;
                if (hidden_field[r][c] == '*') correct_flags--;
                update_cell_on_screen(r, c, '#', top_margin);
            }
        }
        // Open action
        else if (action == 'o' || action == 'O') {
            if (display_field[r][c] == 'F') {
                continue; // Flag is already open
            }
            if (hidden_field[r][c] == '*') {
                display_field[r][c] = '*';
                update_cell_on_screen(r, c, '*', top_margin);
                game_over = 1; win = 0;
            }
            else {
                open_cells_recursive(r, c, hidden_field, display_field, rows, cols, &safe_cells_opened, top_margin);
            }
        }

        // Is win?
        if (safe_cells_opened == safe_cells_total || (correct_flags == mines && flags_placed == mines)) {
            game_over = 1; win = 1;
        }
    }

    setCursor(top_margin + (rows * 2) + 4, 0);
    if (win) printf("CONGRATULATIONS! YOU WON!\n");
    else {
        printf("GAME OVER!\n");
        // Show mines
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (hidden_field[i][j] == '*' && display_field[i][j] != 'F') {
                    update_cell_on_screen(i, j, '*', top_margin);
                }
            }
        }
    }

    free_field(hidden_field, rows);
    free_field(display_field, rows);
    return win;
}




// ==========================================
// |             TIC-TAC-TOE                |
// ==========================================

// Show field TTT
void render_ttt_board(char* board, int top_margin) {
    setCursor(top_margin, 0);

    // Create field 3x3
    // Index:   0 1 2
    //          3 4 5
    //          6 7 8

    printf("     1   2   3\n");   // Column Numbers
    printf("   -------------\n");

    for (int i = 0; i < 3; i++) {
        printf(" %d |", i + 1); // Row Numbers
        for (int j = 0; j < 3; j++) {
            // The formula for converting 2D coordinates to 1D index is : (row * 3) + column
            int index = i * 3 + j;
            printf(" %c |", board[index]);
        }
        printf("\n   -------------\n");
    }
}

// Is win? Returns 0 if game not finished, 1 if user won
int check_ttt_win(char* board) {
    // Winner combos (index)
    int wins[8][3] = {
        {0,1,2}, {3,4,5}, {6,7,8}, // Row
        {0,3,6}, {1,4,7}, {2,5,8}, // Columns
        {0,4,8}, {2,4,6}           // Diagonal
    };

    for (int i = 0; i < 8; i++) {
        // If 1-st cell not a space, and it matches with other two
        if (board[wins[i][0]] != ' ' &&
            board[wins[i][0]] == board[wins[i][1]] &&
            board[wins[i][0]] == board[wins[i][2]]) {
            return 1; // Win
        }
    }
    return 0;
}

// Is draw? (If there is any spaces)
int check_draw(char* board) {
    for (int i = 0; i < 9; i++) {
        if (board[i] == ' ') return 0; // There are free space
    }
    return 1; // There are any free space
}

// AI Logic 
int get_bot_move(char* board, int difficulty, char bot_sym, char player_sym) {
    // 1. EASY: Random
    // 2. MEDIUM, 50% chance to place randomly
    if (difficulty == 1 || (difficulty == 2 && rand() % 2 == 0)) {
        int move;
        do {
            move = rand() % 9;
        } while (board[move] != ' ');
        return move;
    }

    // 2. HARD (half MEDIUM): Smart algorithm

    // A) Win: If AI could win by one step it done that
    for (int i = 0; i < 9; i++) {
        if (board[i] == ' ') {
            board[i] = bot_sym; // Try to place here
            if (check_ttt_win(board)) {
                board[i] = ' '; // Returns to previous
                return i;       // Win step
            }
            board[i] = ' '; // Return hot it was before
        }
    }

    // B) Defence: If player could win by his one step AI block his win
    for (int i = 0; i < 9; i++) {
        if (board[i] == ' ') {
            board[i] = player_sym; // If player want to place there
            if (check_ttt_win(board)) {
                board[i] = ' ';
                return i; // AI block player
            }
            board[i] = ' ';
        }
    }

    // C) Strategy: Take mid
    if (board[4] == ' ') return 4;

    // D) Strategy: Take corners (0, 2, 6, 8)
    int corners[4] = { 0, 2, 6, 8 };
    for (int k = 0; k < 4; k++) {
        // Not only one corner AI take always
        int idx = corners[k];
        if (board[idx] == ' ') return idx;
    }

    // E) If there is any space to go
    int move;
    do {
        move = rand() % 9;
    } while (board[move] != ' ');
    return move;
}

// Main Func
int start_tictactoe(int difficulty) {
    // One-dimensional dynamic array
    char* board = (char*)malloc(9 * sizeof(char));
    if (board == NULL) return 0;

    // Fill spaces
    for (int i = 0; i < 9; i++) board[i] = ' ';

    int top_margin = 4;
    int coin = rand() % 2; // 0 or 1
    char player_sym, bot_sym;
    int current_turn = 0; // 0 - Cross, 1 - zeros

    // Players side
    if (coin == 0) {
        printf("Coin toss: HEADS. You play as X (First turn).\n");
        player_sym = 'X';
        bot_sym = 'O';
        current_turn = 0; // Player (X) move first
    }
    else {
        printf("Coin toss: TAILS. Bot plays as X (First turn).\n");
        player_sym = 'O';
        bot_sym = 'X';
        current_turn = 1; // AI (X) move first
    }
    system("pause");
    system("cls");

    render_ttt_board(board, top_margin);
    printf("\nYou are: %c | Bot is: %c\n", player_sym, bot_sym);
    printf("Input format: Row Col (e.g. 1 1)\n");

    int game_over = 0;
    int winner = 0; // 0 - draw, 1 - player, 2 - AI

    while (!game_over) {
        // --- PLAYERS TURN ---
        if ((current_turn == 0 && player_sym == 'X') || (current_turn == 1 && player_sym == 'O')) {
            int r, c;
            setCursor(top_margin + 8, 0);
            printf("Your turn (%c):       ", player_sym); // Free spaces to clean
            setCursor(top_margin + 8, 13);

            if (scanf_s("%d %d", &r, &c) != 2) {
                while (getchar() != '\n');
                continue;
            }
            r--; c--; // Into indexes 0-2

            // Checks
            if (r < 0 || r > 2 || c < 0 || c > 2) continue;
            int index = r * 3 + c;
            if (board[index] != ' ') {
                printf("Occupied!");
                Sleep(500);
                continue;
            }

            // Place char
            board[index] = player_sym;

            // Show one cell (do not redraw all)
            // Y = top_margin + 2 + row*2
            // X = 5 + col*4
            setCursor(top_margin + 2 + r * 2, 5 + c * 4);
            printf("%c", player_sym);

            // Is win?
            if (check_ttt_win(board)) {
                winner = 1;
                game_over = 1;
            }
            current_turn = !current_turn; // Change turn (0->1 or 1->0)
        }
        // --- AI's TURN ---
        else {
            setCursor(top_margin + 8, 0);
            printf("Bot (%c) is thinking...", bot_sym);
            Sleep(500 + rand() % 500); // AI thinks

            int bot_move_index = get_bot_move(board, difficulty, bot_sym, player_sym);
            board[bot_move_index] = bot_sym;

            // Calculate the coordinates for rendering from the index
            int r = bot_move_index / 3;
            int c = bot_move_index % 3;

            setCursor(top_margin + 2 + r * 2, 5 + c * 4);
            printf("%c", bot_sym);

            if (check_ttt_win(board)) {
                winner = 2;
                game_over = 1;
            }
            current_turn = !current_turn;
        }

        // Is draw? (if no one wins but there are no more places)
        if (!game_over && check_draw(board)) {
            winner = 0;
            game_over = 1;
        }
    }

    setCursor(top_margin + 10, 0);
    if (winner == 1) printf("CONGRATULATIONS! You won!\n");
    else if (winner == 2) printf("Bot won! AI is supreme.\n");
    else printf("It's a DRAW!\n");

    free(board);
    return (winner == 1); // return 1 if the player wins.
}