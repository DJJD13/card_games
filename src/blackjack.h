#ifndef _blackjack_h
#define _blackjack_h

#include "dbg.h"
#include <stdlib.h>

#define BUFFER_SIZE 20
#define HAND_SIZE_MAX 15
#define SUITE_LENGTH 10

enum Suites { Spades, Hearts, Diamonds, Clubs };
enum Faces { None, Jack, Queen, King, Ace};

const char *suiteStrings[] = {
    "Spades",
    "Hearts",
    "Diamonds",
    "Clubs"
};

const char *faceStrings[] = {
    "",
    "Jack",
    "Queen",
    "King",
    "Ace"
};

typedef struct Card {
    enum Suites suite;
    int number;
    enum Faces faceOrAce;
} Card;

typedef struct Hand {
    int count;
    Card *cards[HAND_SIZE_MAX];
} Hand;

Hand *Hand_create();
Card *Hand_add_card(Hand *hand);
void Hand_reset(Hand *hand);

static inline int get_hand_value(Hand *hand) 
{
    int i = 0;
    int val = 0;

    for (i = 0; i < hand->count; i++) {
        val += hand->cards[i]->number;
    }
    return val;
}

static inline int random_value()
{
    return rand() % 10 + 2;
}


#endif