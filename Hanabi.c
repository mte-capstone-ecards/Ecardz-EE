// plays Hanabi gameplay version 4 no rainbow for now
//currently just gives out X0s when out of cards, and breaks itself eventually
//does not acknowledge when a discard does not allow a full win
//cards played is not output correctly :(


/* Trying to check if discard does not allow a full win
printf("cardavail %d cardsplayed %d",cards_available[player_cards[player][card][0]][player_cards[player][card][1]],cards_played[player_cards[player][card][0]]);
    if ((cards_available[player_cards[player][card][0]][player_cards[player][card][1]]<1)&&(cards_played[player_cards[player][card][0]]>=player_cards[player][card][1]))
    {
        printf("You discarded all %c%ds! You lose :(\n",player_cards[player][card][0], player_cards[player][card][1]);
    }
*/
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

// TODO: Macro capitalized
#define NUM_PLAYERS 2
#define NUM_COLOURS 5
#define NUM_CARDS 5
#define MAX_NUMBER 5
#define MAX_HINTS 8

// TODO: Create enum of colors would make it more readable
//enum colours {green = *, red = +, $, #, @,X};
int hints = 8;
int bombs = 4;
char shapes[NUM_COLOURS+1] = {'*', '+', '$', '#', '@','X'};
int cards_available[NUM_COLOURS][MAX_NUMBER] = {{3,2,2,2,1},{3,2,2,2,1},{3,2,2,2,1},{3,2,2,2,1},{3,2,2,2,1}};
int player_cards[NUM_PLAYERS ][NUM_CARDS][2] = {0}; // TODO: cards should be represented by a struct with: {int number, Color color}, not a two item array
int cards_played[NUM_CARDS] = {0};
bool game_over = false;
int cards_left = 40; // TODO: Cards left should come from cards_available, not a seperate number


//draws new card into 5th card position of player
void draw_new(int player, int card)
{
    // TODO: This works for drawing but this is flawed as a deck because it is not correctly random.
    // You have an equal chance to get any card that is still in the deck BUT it is not weighted by the
    // number of cards there are left in the deck. So for e.g. if there were three 1s and one 5, you have a
    // 50% chance to get a 1 and 50% chance to get a 5.
    // TODO: Fix the randomness ( I can give suggestions if you like, but I'll leave it
    // to you to decide if you have a way you want to implement it )
    int colour = rand() % NUM_COLOURS;
    int number = rand() % MAX_NUMBER;
    while(cards_available[colour][number] == 0)
    {
        colour = rand() % NUM_COLOURS;
        number = rand() % MAX_NUMBER;
    }
    player_cards[player][card][0] = colour;
    player_cards[player][card][1] = number;
    cards_available[colour][number] --;
    cards_left --;
}

//randomly assign player cards
void assign_cards()
{
    for (int i = 0; i<NUM_PLAYERS ; i++)
    {
        for (int j = 0; j<NUM_CARDS; j++)
        {
            draw_new(i,j);
        }
    }
}

//print current player cards and which cards have been placed down
void print_cards()
{
    for (int i = 0; i < NUM_PLAYERS ; i++)
    {
        printf("Player %d's cards:\n", i + 1);
        for (int j = 0; j < NUM_CARDS; j++)
        {
            printf("\tCard %d: %c%d\n", j + 1, shapes[player_cards[i][j][0]], player_cards[i][j][1] + 1);
        }
        printf("\n");
    }
    printf("Cards played:\n");
    for (int i = 0; i < NUM_COLOURS; i++)
    {
        printf("\t%c: %d\n", shapes[i], cards_played[i]);
    }
}



//discards selected card of selected player
void discard (int player, int card)
{
    if(cards_left == 0)
    {
        printf("No more cards left...");
        player_cards[player][NUM_CARDS-1][0] = NUM_COLOURS;
        player_cards[player][NUM_CARDS-1][1] = -1;
    }
    else
        draw_new(player,card);
    if(!game_over)
        print_cards();
}

//plays card selected by player
void play(int player, int card)
{
    //is it a valid play?
    if (cards_played[player_cards[player][card][0]] == player_cards[player][card][1])
    {
        printf("Good play!\n");
        cards_played[player_cards[player][card][0]] ++;
        //did the players win?
        if(cards_played[0]==MAX_NUMBER && cards_played[1]==MAX_NUMBER && cards_played[2]==MAX_NUMBER && cards_played[3]==MAX_NUMBER && cards_played[4]==MAX_NUMBER)
        {
            printf("you win\n");
            game_over = true;
        }
    }
    else
    {
        printf("wrong card! BOMB!!\n");
        bombs --;
        // test if players lost all of their bombs
        if (bombs == 0)
        {
            printf("you lose\n");
            game_over = true;
        }
    }
    discard (player,card);
}

int main()
{
    printf("starting Hanabi...\n");
    // Seed the random number generator to ensure different results each time
    srand(time(NULL));

    assign_cards();
    print_cards();

    //start gameplay
    int current_player = 0;
    while(!game_over)
    {
        int selection = 0;
        printf("Player%d, would you like to 1: play, 2: discard or 3: hint?", current_player+1);
        scanf("%d", &selection);
        if (selection == 1)
        {
            int card_num = 0;
            printf("Which card?\n");
            scanf("%d", &card_num);
            play(current_player,card_num-1);
            current_player ++;
            current_player = current_player%NUM_PLAYERS ;
        }
        else if (selection == 2)
        {
            if(hints == MAX_HINTS)
            {
                printf("You cannot discard, try again\n");
            }
            else
            {
                int card_num = 0;
                printf("Which card?\n");
                scanf("%d", &card_num);
                discard (current_player, card_num-1);
                current_player ++;
                current_player = current_player%NUM_PLAYERS ;
                hints++;
            }
        }
        else if (selection == 3)
        {
            if(hints == 0)
            {
                printf("You have no hints, try again\n");
            }
            else
            {
                hints --;
                printf("Give ur hint bestie!\n");
                current_player ++;
                current_player = current_player%NUM_PLAYERS ;
            }
        }
        else
            printf("Incorrect response, try again\n");
    }
    return 0;
}