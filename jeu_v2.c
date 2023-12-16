#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "programmes.h"
#include "ui.h"

#define UP 0    // y--
#define DOWN 2  // y++
#define LEFT 1  // x--
#define RIGHT 3 // x++

#define MULTIPLAYER 2
#define COMPUTER 1
#define AI 3
#define LOAD 5

#define ROUNDS 40 // how many rounds we want per game ?

int main(int argc, char **argv)
{
    init_nb_aleatoire();
    int taille_plateau = get_user_input("Donner la taille du tableau: ", " La taille doit etre au minimum 4. Redonner la taille: ", "Tu est sur pour une telle taille. Pour un jeu optimisé on ne recommend pas d'avoir une taille du tableau plus grand que 20. Redonner la taille: ", 4, 20);
    int number_of_navires = get_user_input("How many boats do you want to be included on the game ? ", "It has to be minimum 1 for an optimized game. Try again: ", "For a game that respects the rules, you can place up to 6 navires. Try again: ", 1, 6);

    // Allocation de la mémoire pour prop
    int **prop;
    prop = malloc(taille_plateau * sizeof(int *));
    if (prop == NULL)
    {
        allocation_error_print_general("prop");
    }

    for (int i = 0; i < taille_plateau; i++)
    {
        prop[i] = malloc(taille_plateau * sizeof(int));
        if (prop[i] == NULL)
        {
            allocation_error_print_with_id("prop row", i);
        }
    }

    initialize_plate(taille_plateau, prop);

    // Allocation de la mémoire pour prop2
    int **prop2;
    prop2 = malloc(taille_plateau * sizeof(int *));
    if (prop == NULL)
    {
        allocation_error_print_general("prop2");
    }

    for (int i = 0; i < taille_plateau; i++)
    {
        prop2[i] = malloc(taille_plateau * sizeof(int));
        if (prop2[i] == NULL)
        {
            allocation_error_print_with_id("prop2 row", i);
        }
    }

    initialize_plate(taille_plateau, prop2);

    // Allocation et initialisation des navires
    Liste_Navire liste;
    Liste_Navire liste2;

    int coulle = 0;
    int *NbNav = &coulle;

    int round = 1; // used to show the number of the round
    int *NbJoue = &round;

    int coulle2 = 0;
    int *NbNav2 = &coulle2;

    int round2 = 1; // used to show the number of the round
    int *NbJoue2 = &round2;

    bool repeat = true; // be used when positioning ships in the begining and to repeat the game procedure

    int mode = game_mode_menu();

    switch (mode)
    {
    case COMPUTER:
        clearScreen();
        liste = initialisation_plateau(prop, taille_plateau, number_of_navires);
        
        // game loop

        rules_interface(ROUNDS, taille_plateau);
        msleep(100);
        waitForKeypress();
        waitForKeypress();
        clearScreen();
        rules_reminder(ROUNDS, taille_plateau);

        while (repeat)
        {
            // loop's logic
            printf("\nRound No %d\n\n", *NbJoue);
            printing_the_grille_v3(prop, taille_plateau);
            if (waitForMenuKeypress())
            {
                if (midle_game_menu(ROUNDS, taille_plateau, 2, COMPUTER) == 1) // 1 is internal code foe saving the progress and continuing another time
                {
                    // TO BE ADDED THE SAVING FUNCTIONALITY
                    //function here
                    printf("\n\n\e[0;32mTHE GAME HAS BEEN SAVED SUCCESFULLY\e[0m\n");
                    exit(5);
                }
            }
            else
            {
                if (proposition_joueur(prop, NbJoue, liste, taille_plateau, NbNav)) // NbNav and NbJoue are updated on the function's core via pointers
                {
                    clearScreen();
                    printf("\033[0;36m\n=====================  Congratsulations, you found a navire. %d so far out of %d!!!  =====================\033[0m\n", *NbNav, number_of_navires);
                    printing_the_grille_v3(prop, taille_plateau);
                    msleep(3000);
                }
                clearScreen();

                // decision making if the user wins or loses the game
                if (round == ROUNDS && *NbNav < number_of_navires)
                {
                    repeat = false;
                    lost_graphics();
                    return 1; // returns 1 if the user ran out of rounds - it also works as the while(repeat) stopper
                }
                if (*NbNav == number_of_navires)
                {
                    win_graphics(taille_plateau, prop, *NbJoue - 1);
                    return 0; // returns 0 if the user found all the ships - it also works as the while(repeat) stopper
                }
            }
        }
        break;

    case MULTIPLAYER:
        clearScreen();
        // TO BE COMPLETED THE MULTIPLAYER FUNCTIONALITY - A NEW FUNCTION initialisation_plateau WILL BE CREATED FOR MANUAL ADDITION OF NAVIRES ON THE PLATEAU

        liste = initialisation_plateau(prop, taille_plateau, number_of_navires);
        liste2 = initialisation_plateau(prop2, taille_plateau, number_of_navires);

        rules_interface(ROUNDS, taille_plateau);
        msleep(100);
        waitForKeypress();
        waitForKeypress();
        clearScreen();
        rules_reminder(ROUNDS, taille_plateau);

        while (repeat)
        {
            // loop's logic
            printf("\nRound No %d\n\n", *NbJoue);
            printing_the_grille_v3(prop, taille_plateau);
            if (waitForMenuKeypress())
            {
                if (midle_game_menu(ROUNDS, taille_plateau, 2, MULTIPLAYER) == 1) // 1 is internal code foe saving the progress and continuing another time
                {
                    printf("\n\e[4;37mNo saving functionality for Multiplayer mode is available on that version. Sorry :/\e[0m\n");
                    continue;
                }
            }
            else
            {
                if (proposition_joueur(prop, NbJoue, liste, taille_plateau, NbNav)) // NbNav and NbJoue are updated on the function's core via pointers
                {
                    clearScreen();
                    printf("\033[0;36m\n=====================  Congratsulations, you found a navire. %d so far out of %d!!!  =====================\033[0m\n", *NbNav, number_of_navires);
                    printing_the_grille_v3(prop, taille_plateau);
                    msleep(3000);
                }
                clearScreen();

                // decision making if the user wins or loses the game
                if (round == ROUNDS && *NbNav < number_of_navires)
                {
                    repeat = false;
                    lost_graphics();
                    return 1; // returns 1 if the user ran out of rounds - it also works as the while(repeat) stopper
                }
                if (*NbNav == number_of_navires)
                {
                    win_graphics(taille_plateau, prop, *NbJoue - 1);
                    return 0; // returns 0 if the user found all the ships - it also works as the while(repeat) stopper
                }
            }
        }
        break;
    case AI:
        clearScreen();
        // TO BE ADDED THE AI FUNCTIONALITY
        break;
    case LOAD:
        clearScreen();
        // TO BE ADDED THE LOAD FUNCTIONALITY
        break;
    default:
        clearScreen();
        printf("\n\033[1;36mThe game has been terminated since no choice was given. See you next time!\033[0m\n");
        exit(4); // code on the log that determines that the game was exited without any saving action taking place
        break;
    }

    return 1;
}