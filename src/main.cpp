#include "raylib.h"
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cstdlib>

using namespace std;


struct Card {
    string rank;
    string suit;
    int value;

    Card(string r, string s, int v) : rank(r), suit(s), value(v) {}
};


class Deck {
private:
    vector<Card> cards;

public:
    Deck() {
        string suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
        string ranks[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King", "Ace"};
        int values[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11};

        for (const auto& suit : suits) {
            for (int i = 0; i < 13; ++i) {
                cards.emplace_back(ranks[i], suit, values[i]);
            }
        }
        shuffleDeck();
    }

    void shuffleDeck() {
        srand(static_cast<unsigned>(time(0)));
        random_shuffle(cards.begin(), cards.end());
    }

    Card drawCard() {
        Card card = cards.back();
        cards.pop_back();
        return card;
    }
};


class Player {
private:
    vector<Card> hand;
    bool isDealer;

public:
    Player(bool dealer = false) : isDealer(dealer) {}

    void addCard(const Card& card) {
        hand.push_back(card);
    }

    int calculateScore() {
        int score = 0;
        int aces = 0;

        for (const auto& card : hand) {
            score += card.value;
            if (card.rank == "Ace") ++aces;
        }

        while (score > 21 && aces > 0) {
            score -= 10;
            --aces;
        }

        return score;
    }

    void resetHand() {
        hand.clear();
    }

    vector<Card> getHand() {
        return hand;
    }

    bool isBusted() {
        return calculateScore() > 21;
    }

    bool hasBlackjack() {
        return hand.size() == 2 && calculateScore() == 21;
    }
};


class BlackjackGame {
private:
    Deck deck;
    Player player;
    Player dealer;
    bool playerTurn;
    bool gameOver;
    string message;

public:
    BlackjackGame() : player(false), dealer(true), playerTurn(true), gameOver(false), message("") {}

    void startGame() {
        player.resetHand();
        dealer.resetHand();
        deck.shuffleDeck();
        playerTurn = true;
        gameOver = false;
        message = "";


        player.addCard(deck.drawCard());
        dealer.addCard(deck.drawCard());
        player.addCard(deck.drawCard());
        dealer.addCard(deck.drawCard());

        checkBlackjack();
    }

    void checkBlackjack() {
        if (player.hasBlackjack()) {
            if (dealer.hasBlackjack()) {
                message = "Both have Blackjack! It's a tie!";
            } else {
                message = "Player has Blackjack! Player wins!";
            }
            gameOver = true;
        } else if (dealer.hasBlackjack()) {
            message = "Dealer has Blackjack! Dealer wins!";
            gameOver = true;
        }
    }

    void playerHit() {
        if (!gameOver && playerTurn) {
            player.addCard(deck.drawCard());
            if (player.isBusted()) {
                message = "Player busted! Dealer wins!";
                gameOver = true;
            }
        }
    }

    void playerStand() {
        if (!gameOver && playerTurn) {
            playerTurn = false;
            dealerTurn();
        }
    }

    void dealerTurn() {
        while (dealer.calculateScore() < 17) {
            dealer.addCard(deck.drawCard());
        }
        if (dealer.isBusted()) {
            message = "Dealer busted! Player wins!";
        } else {
            evaluateWinner();
        }
        gameOver = true;
    }

    void evaluateWinner() {
        int playerScore = player.calculateScore();
        int dealerScore = dealer.calculateScore();

        if (playerScore > dealerScore) {
            message = "Player wins!";
        } else if (playerScore < dealerScore) {
            message = "Dealer wins!";
        } else {
            message = "It's a tie!";
        }
    }

    void render() {

        ClearBackground(DARKGREEN);


        DrawText("Player's Hand:", 20, 400, 20, RAYWHITE);
        auto playerHand = player.getHand();
        float playerStartX = 400 - (playerHand.size() * 60);
        for (size_t i = 0; i < playerHand.size(); i++) {
            DrawRectangle(playerStartX + i * 120, 450, 100, 140, LIGHTGRAY);
            DrawText(playerHand[i].rank.c_str(), playerStartX + 10 + i * 120, 460, 20, BLACK);
            DrawText(playerHand[i].suit.c_str(), playerStartX + 10 + i * 120, 490, 20, BLACK);
        }




        DrawText("Dealer's Hand:", 20, 100, 20, RAYWHITE);
        auto dealerHand = dealer.getHand();
        float dealerStartX = 400 - (dealerHand.size() * 60);
        for (size_t i = 0; i < dealerHand.size(); i++) {
            if (i == 0 && playerTurn) {
                DrawRectangle(dealerStartX, 150, 100, 140, DARKGRAY);
                DrawText("Hidden", dealerStartX + 10, 180, 20, BLACK);
            } else {
                DrawRectangle(dealerStartX + i * 120, 150, 100, 140, LIGHTGRAY);
                DrawText(dealerHand[i].rank.c_str(), dealerStartX + 10 + i * 120, 160, 20, BLACK);
                DrawText(dealerHand[i].suit.c_str(), dealerStartX + 10 + i * 120, 190, 20, BLACK);
            }
        }


        if (!playerTurn || gameOver) {
            DrawText(TextFormat("Dealer's Score: %d", dealer.calculateScore()), 20, 250, 20, RAYWHITE);
        }
        DrawText(TextFormat("Player's Score: %d", player.calculateScore()), 20, 550, 20, RAYWHITE);


        if (gameOver) {
            DrawText(message.c_str(), 400, 300, 30, YELLOW);
            DrawText("Press R to Restart", 400, 350, 20, YELLOW);
        }
    }

    void handleInput() {
        if (IsKeyPressed(KEY_H) && !gameOver && playerTurn) playerHit();
        if (IsKeyPressed(KEY_S) && !gameOver && playerTurn) playerStand();
        if (IsKeyPressed(KEY_R) && gameOver) startGame();
    }
};


int main() {
    InitWindow(800, 600, "Blackjack with raylib");
    SetTargetFPS(60);

    BlackjackGame game;
    game.startGame();

    while (!WindowShouldClose()) {
        BeginDrawing();
        game.render();
        EndDrawing();

        game.handleInput();
    }

    CloseWindow();
    return 0;
}
