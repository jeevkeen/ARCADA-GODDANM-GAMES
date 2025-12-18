#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
#include "games.h"
#include "utilits.h"

//open games menu
void start_games_menu(int* is_exit, int* points, int* difficulty) {
    int user_input;
    int is_back = 0;

    while (1) {
        system("cls");
        printf("----- GAME MENU -----\n\n\
1. Lucky Game\n\
2. Memo (Card Game)\n\
3. Minesweeper\n\
4. Tic Tac Toe\n\
5. Back\n\
0. Exit\n\
================\n\
Choose item: ");

        scanf_s("%d", &user_input);

        while (user_input < 0 || user_input > 5) {
            printf("Wrong input. Try again: ");
            scanf_s("%d", &user_input);
        }

        switch (user_input) {
        case 1:
            system("cls");
            if (start_lucky_game(points, *difficulty)) { //start lucky game. if it return 1, you win
                *points += 10; // win
            }
            else {
                *points -= 5;  // lose
            }
            system("pause");
            break;
        case 2:
            system("cls");
            if (start_find_game(*difficulty)) { //start memo (card game). if it return 1, you win. you cant lose in this game
                *points += 20;
            }
            system("pause");
            break;
        case 3:
            system("cls");
            if (start_minesweeper(*difficulty)) { //start minesweeper.
                *points += 50;
            }
            system("pause");
            break;
        case 4:
            system("cls");
            if (start_tictactoe(*difficulty))
                *points += 30;
            system("pause");
            break;
        case 5:
            is_back = 1;
            break;
        case 0:
            *is_exit = 1;
            break;
        }

        if (is_back) break;
        if (*is_exit) break;
    }
}

//open setting menu
void start_settings_menu(int* is_exit, int* difficulty) {
    int user_input;
    int is_back = 0;

    while (1) {
        system("cls");
        printf("----- SETTINGS -----\n\n1. Easy\n2. Medium\n3. Hard\n4. Back\n0. Exit\n====================\n[Current difficulty: %d]\nChoose difficulty: ", *difficulty);

        scanf_s("%d", &user_input);

        while (user_input < 0 || user_input > 4) {
            printf("Wrong input. Try again: ");
            scanf_s("%d", &user_input);
        }

        switch (user_input) {
        case 1:
            *difficulty = 1;
            printf("Difficulty set to EASY.\n");
            system("pause");
            break;
        case 2:
            *difficulty = 2;
            printf("Difficulty set to MEDIUM.\n");
            system("pause");
            break;
        case 3:
            *difficulty = 3;
            printf("Difficulty set to HARD.\n");
            system("pause");
            break;
        case 4:
            is_back = 1;
            break;
        case 0:
            *is_exit = 1;
            break;
        }

        if (is_back) break;
        if (*is_exit) break;
    }
}

void start_main_menu(int *points, int *difficulty) {
	int user_input;
	int is_exit = 0;

	while (1) {
		system("cls");
		printf("----- MAIN MENU -----\n\n\
1. Play\n\
2. Settings\n\
0. Exit\n\
=====================\n\
Choose item: ");

		scanf_s("%d", &user_input);

		while (user_input < 0 || user_input > 2) {
			printf("Wrong input. Try again: ");
			scanf_s("%d", &user_input);
		}

		switch (user_input) {
		case 1:
			system("cls");
			start_games_menu(&is_exit, points, difficulty);
			break;
		case 2:
			system("cls");
			start_settings_menu(&is_exit, difficulty);
			break;
		case 0:
			is_exit = 1;
			break;
		}

		if (is_exit) break;
	}
}