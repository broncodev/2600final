#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <time.h>
#include <string.h>

#define MAX_CARDS 11
#define MAX_HANDS 4

struct Hand {
    int cards[MAX_CARDS];
    int count;
    bool stand;
    bool bust;
};

// Function prototypes
int draw_card();
int calculate_score(struct Hand *hand);
bool full_bust(struct Hand player_hands[MAX_HANDS]);
bool ready_to_finish(struct Hand player_hands[MAX_HANDS]);

void print_help();
void print_card(int card);
void reset_hands(struct Hand player_hands[MAX_HANDS], struct Hand *dealer_hand);
void deal_or_hit(struct Hand *hand);
void next_hand_not_bust_or_stand(struct Hand player_hands[MAX_HANDS], int *current_hand);

int main() {
    srand(time(NULL)); //random seed each run

    char input[10];
    struct Hand player_hands[MAX_HANDS] = {{{0}, 0, false, false}};
    struct Hand dealer_hand = {{0}, 0, false, false};
    int player_score = 0, dealer_score = 0;
    int playing = 1;

    printf("Welcome to Blackjack!\n");

    while (playing) {
        printf("Type 'h' for instructions, 's' to begin, or 'q' to quit.\n");
        printf("\n> ");
        scanf("%s", input);

        if (strcmp(input, "h") == 0) {
            print_help();
        } else if (strcmp(input, "q") == 0) {
            break;
        } else if (strcmp(input, "s") == 0) {
            // Reset hands
            reset_hands(player_hands, &dealer_hand);

            // Initial deal
            deal_or_hit(&player_hands[0]);
            deal_or_hit(&dealer_hand);

            // Game loop
            int current_hand = -1;
            while (1) {
                printf("\nYour hands:\n");
                for (int i = 0; i < MAX_HANDS; i++) {
                    if (player_hands[i].count > 0) {
                        // Print each hand
                        printf("Hand %d: ", i + 1);
                        for (int j = 0; j < player_hands[i].count; j++) {
                            print_card(player_hands[i].cards[j]);
                        }
                        player_score = calculate_score(&player_hands[i]);
                        printf("Score: %d\t", player_score);

                        // Print hand status
                        if (player_hands[i].bust) {
                            printf("BUST!\n");
                        } else if (player_hands[i].stand) {
                            printf("STAND\n");
                        } else {
                            printf("\n");
                        }
                    }
                }

                // Get the next hand for the player to play.
                next_hand_not_bust_or_stand(player_hands, &current_hand);
                player_score = calculate_score(&player_hands[current_hand]);

                // Print dealer's hand (covered)
                printf("\nDealer's hand: ");
                for (int j = 0; j < dealer_hand.count; j++) {
                    if (j == 0) {
                        print_card(dealer_hand.cards[j]);
                    } else {
                        printf("[?] ");
                    }
                }
                printf("Score: %d+\n", dealer_hand.cards[0]);

                // Check if all hands are either bust or standing
                if (!ready_to_finish(player_hands)) {
                    // Player's turn
                    bool valid_input = false;
                    while (!valid_input) {
                        printf("\nYour turn (Hand %d): ", current_hand + 1);
                        printf("\nType 'h' to hit, 's' to stand, '|' to split.\n> ");
                        scanf("%s", input);

                        if (strcmp(input, "h") == 0) {
                            valid_input = true;

                            // Hit the next card to the current hand
                            deal_or_hit(&player_hands[current_hand]);

                            // Check if the hand is bust
                            player_score = calculate_score(&player_hands[current_hand]);
                            if (player_score > 21) {
                                player_hands[current_hand].bust = true;
                                printf("Hand %d bust!\n", current_hand + 1);
                            }
                        } else if (strcmp(input, "s") == 0) {
                            valid_input = true;

                            // Stand the current hand
                            player_hands[current_hand].stand = true;
                        } else if (strcmp(input, "|") == 0) {
                            // make sure the current hand has two pairs, also make sure the player
                            // hasn't reached the maximum number of hands
                            if (player_hands[current_hand].count == 2 && player_hands[current_hand].cards[0] == player_hands[current_hand].cards[1] && player_hands[MAX_HANDS-1].count == 0) {
                                valid_input = true;

                                // Split the hand
                                struct Hand new_hand = {{0}, 0, false, false};
                                new_hand.cards[new_hand.count++] = player_hands[current_hand].cards[1];

                                // Change the current hand
                                player_hands[current_hand].count = 1;
                                player_hands[current_hand].stand = false;
                                player_hands[current_hand].bust = false;

                                // Add the new hand to the list of hands
                                player_hands[current_hand + 1] = new_hand;
                                printf("Hand %d split into two hands!\n", current_hand + 1);
                            } else {
                                printf("Cannot split this hand. (Max Limit, or not a splittable hand)\n");
                            }
                        } else {
                            printf("Unknown command. Type 'h' for instructions.\n");
                        }
                    }
                } else {
                    // End game phase, dealer's turn?
                    printf("\nAll hands are either bust or standing.\n");

                    // Only play the dealer if not all player hands are bust (atleast one stand)
                    if (!full_bust(player_hands)) {
                        // Viciously take all my money at the MGM Grand in Las Vegas, yes i am talking from personal experience. Who gets five blackjacks in a row? 
                        // Apparently the house.
                        while (dealer_score < 17) {
                            deal_or_hit(&dealer_hand);
                            dealer_score = calculate_score(&dealer_hand);
                        }
                    }

                    // Reveal the dealer's hand
                    printf("\nDealer's final hand: ");
                    for (int j = 0; j < dealer_hand.count; j++) {
                        print_card(dealer_hand.cards[j]);
                    }
                    dealer_score = calculate_score(&dealer_hand);
                    printf("Score: %d\n", dealer_score);

                    // Check if the dealer busted
                    if (dealer_score > 21) {
                        dealer_hand.bust = true;
                        printf("Dealer bust!\n");
                    } else {
                        dealer_hand.stand = true;
                    }

                    // Determine winner
                    for (int i = 0; i < MAX_HANDS; i++) {
                        // Only check played hands
                        if (player_hands[i].count > 0) {
                            // Calculate hand score
                            player_score = calculate_score(&player_hands[i]);
                            if (player_hands[i].bust) { // check if the hand is bust
                                printf("Hand %d bust!\n", i + 1);
                            } else if (dealer_hand.bust || player_score > dealer_score) { // check if the dealer is bust or the player has a higher score
                                printf("Hand %d wins!\n", i + 1);
                            } else if (player_score == dealer_score) { // check if the scores are equal
                                printf("Hand %d ties with the dealer.\n", i + 1);
                            } else if (player_score < dealer_score && !dealer_hand.bust) { // check if the player has a lower score (and the dealer is not bust)
                                printf("Hand %d loses to the dealer.\n", i + 1);
                            }
                        }
                    }

                    break; // exit the game loop so we can start a new game
                }
            }
        } else {
            printf("Unknown command. Type 'h' for instructions.\n");
        }
    }

    printf("Thanks for playing!\n");
    return 0;
}

// Draws a card between 1 (Ace) and 11 (Jack/Queen/King are 10)
int draw_card() {
    int card = rand() % 13 + 1;
    return (card > 10) ? 10 : card;
}

// Prints a card nicely
void print_card(int card) {
    if (card == 1) printf("[A] ");
    else if (card == 10) printf("[10] ");
    else printf("[%d] ", card);
}

// Calculates score, handling Aces (1 or 11)
int calculate_score(struct Hand *hand) {
    int score = 0;
    int ace_count = 0;

    for (int i = 0; i < hand->count; i++) {
        score += hand->cards[i];
        if (hand->cards[i] == 1) ace_count++;
    }

    // Upgrade Aces from 1 to 11 if it helps and doesn't bust
    while (ace_count > 0 && score + 10 <= 21) {
        score += 10;
        ace_count--;
    }

    return score;
}

// iterates through the hands and resets them
void reset_hands(struct Hand player_hands[MAX_HANDS], struct Hand *dealer_hand) {
    // reset player hands
    for (int i = 0; i < MAX_HANDS; i++) {
        player_hands[i].count = 0;
        player_hands[i].stand = false;
        player_hands[i].bust = false;
    }

    // reset dealer hand
    dealer_hand->count = 0;
    dealer_hand->stand = false;
    dealer_hand->bust = false;
}

// gives each hand a new set of cards
void deal_or_hit(struct Hand *hand) {
    // if the hand has more than 0 cards, it's a hit
    if (hand->count > 0) {
        hand->cards[hand->count++] = draw_card();
    } else {
        // otherwise we need to do the initial deal
        hand->cards[hand->count++] = draw_card();
        hand->cards[hand->count++] = draw_card();
    }
}

// checks if all player hands are bust, this is used to determine whether the dealer
// needs to play at all or not
bool full_bust(struct Hand player_hands[MAX_HANDS]) {
    for (int i = 0; i < MAX_HANDS; i++) {
        if (!player_hands[i].bust && player_hands[i].count > 0) {
            return false;
        }
    }
    return true;
}

// checks if all player hands are either bust or standing, this is used to determine
// whether to enter the end game phase or not
bool ready_to_finish(struct Hand player_hands[MAX_HANDS]) {
    bool all_bust = true;
    for (int i = 0; i < MAX_HANDS; i++) {
        if (player_hands[i].count > 0) {
            if (!player_hands[i].bust && !player_hands[i].stand) {
                all_bust = false;
                break;
            }
        }
    }
    return all_bust;
}

// find the next hand that is not bust or standing
// this is used to determine which hand to play next
// it will skip over any hands that are bust or standing
// and will return -1 if there are no hands left to play
void next_hand_not_bust_or_stand(struct Hand player_hands[MAX_HANDS], int *current_hand) {
    bool hands_before_not_bust = false;
    for (int i = *current_hand; i < MAX_HANDS; i++) {
        if (!player_hands[i].bust 
            && !player_hands[i].stand 
            && player_hands[i].count > 0
        ) {
            *current_hand = i;
            return;
        }
    }
    
    *current_hand = -1;
}

// Help message
void print_help() {
    printf("\n--- Blackjack Help ---\n");
    printf("Goal: Get as close to 21 as possible without going over.\n");
    printf("You vs the Dealer.\n");
    printf("Commands:\n");
    printf("  s - start (begin a new game)\n");
    printf("  h - help (show this help message)\n");
    printf("Commands in game:\n");
    printf("  h - hit (draw another card)\n");
    printf("  s - stand (end your turn)\n");
    printf("  | - split (split your hand into two)\n");
    printf("Cards: 2-10 as numbered, face cards are 10, Ace is 1 or 11.\n");
    printf("Dealer stands on 17 or higher.\n");
    printf("----------------------\n");
}
