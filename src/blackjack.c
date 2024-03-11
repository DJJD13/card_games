#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include "dbg.h"
#include "blackjack.h"

Hand *Hand_create()
{
    Hand *new_hand = malloc(sizeof(Hand));
    check_mem(new_hand);

    return new_hand;
error:
    return NULL;
}

Card *Hand_add_card(Hand *hand)
{
    Card *newCard = malloc(sizeof(Card));
    check_mem(newCard);

    newCard->number = random_value();
    if(newCard->number == 11) {
        newCard->faceOrAce = (enum Faces)(rand() % 4) + 1;
    }
    newCard->suite = (enum Suites)(rand() % 4);
    hand->cards[hand->count++] = newCard;
    return newCard;
error:
    return NULL;
}

void Hand_reset(Hand *hand)
{
    int i = 0;
    if (hand->cards)
    {
        for (i = 0; i < hand->count; i++)
        {
            free(hand->cards[i]);
        }
    }
    hand->count = 0;
}

int setup_game_card(Hand *playerHand, Hand *dealerHand)
{

    printf("Dealing...\n");
    Card *playerCard = Hand_add_card(playerHand);
    check_mem(playerCard);
    printf("Player dealt %d of %s\n", playerCard->number, suiteStrings[playerCard->suite]);


    Card *dealerCard = Hand_add_card(dealerHand);
    check_mem(dealerCard);
    printf("Dealer dealt first card\n");

    playerCard = Hand_add_card(playerHand);
    check_mem(playerCard);
    printf("Player dealt %d of %s\n", playerCard->number, suiteStrings[playerCard->suite]);

    dealerCard = Hand_add_card(dealerHand);
    check_mem(dealerCard);
    printf("Dealer showing %d of %s\n", dealerCard->number, suiteStrings[dealerCard->suite]);
    debug("Hand Count: %d", playerHand->count);
    printf("--------------\n");
    printf("Player has: %d\n", get_hand_value(playerHand));
    return 0;
error:
    return -1;
}

void setup_game(int *playerHand, int *dealerHand)
{
    printf("Dealing...\n");
    *playerHand += random_value();
    printf("Player dealt: %d\n", *playerHand);
    *dealerHand += random_value();
    printf("Dealer dealt card\n");
    *playerHand += random_value();
    printf("Player dealt: %d\n", *playerHand);
    int cardToShow = random_value();
    *dealerHand += cardToShow;
    printf("Dealer showing: %d\n", cardToShow);
    
}

void player_call_card(Hand *playerHand, Hand *dealerHand) 
{
    printf("Dealer has: %d\n", get_hand_value(dealerHand));
    while (get_hand_value(dealerHand) < 16)
    {
        Hand_add_card(dealerHand);      // Ignore return value
        printf("Dealer has: %d\n", get_hand_value(dealerHand));
    }
    if (get_hand_value(dealerHand) > 21)
    {
        printf("Dealer busted! You win!\n");
    }
    else if (get_hand_value(dealerHand) > get_hand_value(playerHand))
    {
        printf("Dealer wins!\n");
    }
    else if (get_hand_value(dealerHand) < get_hand_value(playerHand))
    {
        printf("Player wins!\n");
    }
    else
    {
        printf("Dealer and Player push\n");
    }
    Hand_reset(playerHand);
    Hand_reset(dealerHand);
    
}

void player_hit(int *playerHand)
{
    *playerHand += random_value();
    printf("Player amount: %d\n", *playerHand);
}

void player_call(int *playerHand, int *dealerHand)
{
    printf("Dealer has: %d\n", *dealerHand);
    while (*dealerHand < 16)
    {
        *dealerHand += random_value();
        printf("Dealer has: %d\n", *dealerHand);
    }
    if (*dealerHand > 21)
    {
        printf("Dealer busted! You win!\n");
        reset_hands(playerHand, dealerHand);
    }
    else if (*dealerHand > *playerHand)
    {
        printf("Dealer wins!\n");
    }
    else if (*dealerHand < *playerHand)
    {
        printf("Player wins!\n");
    }
    else
    {
        printf("Dealer and Player push\n");
    }
    reset_hands(playerHand, dealerHand);
}

int main(int argc, char *argv[])
{
    // Initialize random seed for number generator
    srand(time(NULL));
    char buffer[BUFFER_SIZE];
    char command = '\0';

    Hand *player_hand = Hand_create();
    check(player_hand != NULL, "Player Hand was NULL on creation");
    Hand *dealer_hand = Hand_create();
    check(dealer_hand != NULL, "Dealer hand was NULL on creation");

    printf("--- Welcome to JD's BlackJack Game! ---\n");

    while(command != 'Q') {
        if (player_hand ->count > 0 || dealer_hand->count > 0) {
            printf("Type [H]it or [C]all\n");
        } else {
            printf("Type [D]eal to start, or type [Q]uit to close the program\n");
        }
        fgets(buffer, BUFFER_SIZE - 1, stdin);
        sscanf(buffer, "%c", &command);        
        command = toupper(command);
        switch (command)
        {
        case 'D':
            if (player_hand->count > 0 || dealer_hand->count > 0)
            {
                printf("Cards have already been dealt\n");
                break;
            }
            setup_game_card(player_hand, dealer_hand);
            break;
        case 'H':
            Card *dealtCard = Hand_add_card(player_hand);
            check_mem(dealtCard);
            printf("Player has %d\n", get_hand_value(player_hand));
            if (get_hand_value(player_hand) == 21)
            {
                printf("Player wins!\n");
                Hand_reset(player_hand);
                Hand_reset(dealer_hand);
                break;
            } else if (get_hand_value(player_hand) > 21) {
                printf("Player has busted!\n");
                Hand_reset(player_hand);
                Hand_reset(dealer_hand);
                break;
            } 
            break;
        case 'C':
            player_call_card(player_hand, dealer_hand);
            break;
        case 'Q':
            printf("Thank you for playing JD's Blackjack Game!\n");
            break;
        default:
            printf("Invalid command: Please [D]eal, [H]it, or [C]all\n");
            break;
        }
    }


    return 0;
error:
    return 1;
}