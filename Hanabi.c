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

#define num_players 2
#define num_colours 5
#define num_cards 5
#define max_number 5
#define max_hints 8

int hints = 8;
int bombs = 4;
char shapes[num_colours+1] = {'*', '+', '$', '#', '@','X'};
int cards_available[num_colours][max_number] = {{3,2,2,2,1},{3,2,2,2,1},{3,2,2,2,1},{3,2,2,2,1},{3,2,2,2,1}};
int player_cards[num_players][num_cards][2] = {0};
int cards_played[num_cards] = {0};
bool game_over = false;
int cards_left = 40;

//randomly assign player cards
void assign_cards()
{
    for (int i = 0; i<num_players; i++) 
    {
        for (int j = 0; j<num_cards; j++)
        {
            int colour = rand() % num_colours;
            int number = rand() % max_number;
            while(cards_available[colour][number] == 0)
            {
                colour = rand() % num_colours;
                number = rand() % max_number;
            }
            player_cards[i][j][0] = colour;
            player_cards[i][j][1] = number;
            cards_available[colour][number] --;
        }
    }
}

//print current player cards and which cards have been placed down
void print_cards()
{
    for (int i = 0; i < num_players; i++) 
    {
        printf("Player %d's cards:\n", i + 1);
        for (int j = 0; j < num_cards; j++)
        {
            printf("  Card %d: %c%d\n", j + 1, shapes[player_cards[i][j][0]], player_cards[i][j][1] + 1);
        }
        printf("\n");
    }
    printf("Cards played:\n");
    for (int i = 0; i < num_colours; i++)
    {
        printf("%c: %d\n", shapes[i], cards_played[i]);
    }
}

//draws new card into 5th card position of player
void draw_new(int player)
{
    int colour = rand() % num_colours;
    int number = rand() % max_number;
    while(cards_available[colour][number] == 0)
    {
        colour = rand() % num_colours;
        number = rand() % max_number;
    }
    player_cards[player][num_cards-1][0] = colour;
    player_cards[player][num_cards-1][1] = number;
    cards_available[colour][number] --;
    cards_left --;

}

//discards selected card of selected player
void discard (int player, int card)
{
    for(int i = card; i<num_cards; i++)
    {
        player_cards[player][i][0] = player_cards[player][i+1][0];
        player_cards[player][i][1] = player_cards[player][i+1][1];
    }
    if(cards_left ==0)
    {
        printf("No more cards left...");
        player_cards[player][num_cards-1][0] = num_colours;
        player_cards[player][num_cards-1][1] = -1;
    }
    else
        draw_new(player);
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
        if(cards_played[0]==max_number && cards_played[1]==max_number && cards_played[2]==max_number && cards_played[3]==max_number && cards_played[4]==max_number)
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
            current_player = current_player%num_players;
        }
        else if (selection == 2)
        {
            if(hints == max_hints)
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
                current_player = current_player%num_players;
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
                current_player = current_player%num_players;
            }
        }
        else
            printf("Incorrect response, try again\n");
    }
    return 0;
}