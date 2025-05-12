# **CS2600 Final Project: BlackJack Game!**

## **Overview**
This is a terminal-based BlackJack game written in C, which allows a user to play against a dealer. The implementation supports standard BlackJack actions like hitting, standing, splitting, and including realistic scoring of Aces as 1 or 11 with up to four player hands and an achivement system.

### **How To Play**:
- **START**: Type s to start game.
- **HELP**: Type h for instructions.
- **QUIT**: Type q to quit game.
- During each turn:
    Type h to hit (draw another card).
    Type s to stand (end your turn for that hand).
    Type | to split a pair into two hands (if allowed).
- Round Flow:
    Each hand and dealer and dealt two cards intially.
    Players play through all nonbusted,nonstand hands in order.
    Dealer reveals hidden card and draws until reaching at least 17.
    Results are compared to total points and determine game status.
    End of Round Options: Either keep your new total as starting points for next round or reset back to initial 500 points.

### **Game Mechanics** **
1. Multiple Hands & Splitting: Supports up to 4 active hands per round. 
2. Accurate Scoring: Aces are counted as 11 unless it causes a bust (counting as 1 then).
3. Dealer Rules: Dealer reveals one card then draws until reaching score of at least 17.
4. Point System: Base points, Bonus +500 per split, BlackJack hand x3 multipler, 5-Card Charlie Hands x2 multplier.
5. Achievements & High Score Tracking: Win, Lose, Tie

### **Contributors**
  * David Alvarez (Derylsharpe)
  * Landon Escorcio (broncodev)
  * Kenny Huynh (KennHuynh)
  * Joshua Owen (jaoCPP)
  * Jenni Puga-Raya (jenniraya)
