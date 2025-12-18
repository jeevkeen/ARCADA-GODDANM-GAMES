#include <stdio.h>
#include <windows.h> 
#include <stdlib.h>  
#include <locale.h>  
#include <time.h>    
#include "utilits.h"
#include "games.h"
#include "menu.h"

int main() {

    setlocale(LC_ALL, "rus");
    srand(time(0)); // Initialization random numbers generator

    int points = 0;
    int difficulty = 1; // 1 - easy (default), 2 - medium, 3 - hard

    start_main_menu(&points, &difficulty);

    system("cls");
    printf("Your final score: %d\n", points);
    return 0;
}