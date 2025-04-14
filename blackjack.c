#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_CARDS 11

// Function prototypes
void print_help();
int draw_card();
void print_card(int card);
int calculate_score(int hand[], int count);

int main() {
  srand(time(NULL)); //random seed each run

  char input[10];
  int player_hand[MAX_CARDS], dealer_hand[MAX_CARDS];
  int player_count = 0, dealer_count = 0;
  int player_score = 0, dealer_score = 0;
  int playing = 1;

  printf("Welcome to Blackjack!\n");

  while (playing) {
    printf("Type 'h' for instructions, 's' to begin, or 'q' to quit.\n");
    printf("\n> ");
    scanf("%s", input);

    if (strcmp(input, "h")==0) {
      print_help();
    }
    else if (strcmp(input, "q")==0) {
      break;
    }
    else if (strcmp(input, "s") == 0) {
      // Reset hands
      player_count = dealer_count = 0;

      // Initial deal
      player_hand[player_count++] = draw_card();
      player_hand[player_count++] = draw_card();
      dealer_hand[dealer_count++] = draw_card();
      dealer_hand[dealer_count++] = draw_card();

      // Game loop
      while (1) {
        player_score = calculate_score(player_hand, player_count);
        dealer_score = calculate_score(dealer_hand, dealer_count);

        printf("\nYour hand: ");
        for (int i = 0; i < player_count; i++) print_card(player_hand[i]);
        printf(" (Total: %d)\n", player_score);

        printf("Dealer shows: ");
        print_card(dealer_hand[0]);
        printf(" [?]\n");

        if (player_score > 21) {
          printf("You busted! Dealer wins.\n");
          break;
        }

        printf("Type 'h' to draw a card or 's' to hold: ");
        scanf("%s", input);

        if (strcmp(input, "h") == 0) {
          player_hand[player_count++] = draw_card();
        } else if (strcmp(input, "s") == 0) {
          // Dealer's turn
          printf("\nDealer's hand: ");
          for (int i = 0; i < dealer_count; i++) print_card(dealer_hand[i]);
          printf(" (Total: %d)\n", dealer_score);

          // Dealer always hits if total is less than 17
          while (dealer_score < 17) {
            int new_card = draw_card();
            printf("Dealer draws: ");
            print_card(new_card);
            dealer_hand[dealer_count++] = new_card;
            dealer_score = calculate_score(dealer_hand, dealer_count);
            printf(" (Total: %d)\n", dealer_score);
          }

          // Winning or losing or tie
          if (dealer_score > 21 || player_score > dealer_score) {
            printf("You win!\n");
          } else if (dealer_score > player_score) {
            printf("Dealer wins.\n");
          } else {
            printf("It's a tie!\n");
          }
          break;
        } else {
          printf("Invalid input. Type 'h' or 's'.\n");
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
int calculate_score(int hand[], int count) {
  int score = 0;
  int ace_count = 0;

  for (int i = 0; i < count; i++) {
    score += hand[i];
    if (hand[i] == 1) ace_count++;
  }

  // Upgrade Aces from 1 to 11 if it helps and doesn't bust
  while (ace_count > 0 && score + 10 <= 21) {
    score += 10;
    ace_count--;
  }

  return score;
}

// Help message
void print_help() {
  printf("\n--- Blackjack Help ---\n");
  printf("Goal: Get as close to 21 as possible without going over.\n");
  printf("You vs the Dealer.\n");
  printf("Commands:\n");
  printf("  s - start (begin a new game)\n");
  printf("  h - hit (draw another card)\n");
  printf("  s - stand (end your turn)\n");
  printf("  h - help (show this help message)\n");
  printf("Cards: 2-10 as numbered, face cards are 10, Ace is 1 or 11.\n");
  printf("Dealer stands on 17 or higher.\n");
  printf("----------------------\n");
}
