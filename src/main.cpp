#include "raylib.h"
#include <memory>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <random>

using namespace std;


class Button {
private:
    Rectangle bounds;      
    Color color;           
    Color hoverColor;      
    Color textColor;       
    string text;           
    bool isHovered;        

public:
    
    Button(float x, float y, float width, float height, const string& text, Color color = GRAY, Color hoverColor = DARKGRAY, Color textColor = WHITE)
        : bounds({x, y, width, height}), color(color), hoverColor(hoverColor), textColor(textColor), text(text), isHovered(false) {}

    
    void render() {
        DrawRectangleRec(bounds, isHovered ? hoverColor : color); 
        float textWidth = MeasureText(text.c_str(), 20);
        DrawText(text.c_str(), bounds.x + (bounds.width - textWidth) / 2, bounds.y + (bounds.height - 20) / 2, 20, textColor);
    }

    
    void update(Vector2 mousePos) {
        isHovered = CheckCollisionPointRec(mousePos, bounds);
    }

    
    bool isClicked() const {
        return isHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    }
};


class ICard {
public:
    virtual string getRank() const = 0;
    virtual string getSuit() const = 0;
    virtual int getValue() const = 0;
    virtual ~ICard() = default;
};


class Card : public ICard {
private:
    string rank;
    string suit;
    int value;

public:
    Card(string r, string s, int v) : rank(r), suit(s), value(v) {}
    
    string getRank() const override { return rank; }
    string getSuit() const override { return suit; }
    int getValue() const override { return value; }
};


class IDeck {
public:
    virtual shared_ptr<ICard> drawCard() = 0;
    virtual void shuffleDeck() = 0;
    virtual void refillAndShuffleDeck() = 0;
    virtual ~IDeck() = default;
};


class Deck : public IDeck {
private:
    vector<shared_ptr<ICard>> cards;

public:
    Deck() {
        string suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
        string ranks[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King", "Ace"};
        int values[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11};

        for (const auto& suit : suits) {
            for (int i = 0; i < 13; i++) {
                cards.push_back(make_unique<Card>(ranks[i], suit, values[i]));
            }
        }
        shuffleDeck();
    }

    void shuffleDeck() override {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(cards.begin(), cards.end(), g);
    }

    void refillAndShuffleDeck() override {
        cards.clear();
        string suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
        string ranks[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King", "Ace"};
        int values[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11};

        for (const auto& suit : suits) {
            for (int i = 0; i < 13; i++) {
                cards.push_back(make_unique<Card>(ranks[i], suit, values[i]));
            }
        }
        shuffleDeck();
    }

    shared_ptr<ICard> drawCard() override {
        if (cards.empty()) {
            refillAndShuffleDeck();
        }
        shared_ptr<ICard> card = cards.back(); 
        cards.pop_back();
        return card;
    }



};


class IPlayer {
public:
    virtual void addCard(shared_ptr<ICard>  card) = 0;
    virtual int calculateScore() = 0;
    virtual void resetHand() = 0;
    virtual bool isBusted() = 0;
    virtual bool hasBlackjack() = 0;
    virtual vector<shared_ptr<ICard> > getHand() const = 0;
    virtual ~IPlayer() = default;
};


class Player : public IPlayer {
private:
    vector<shared_ptr<ICard> > hand;
    bool isDealer;

public:
    
    Player(bool dealer = false) : isDealer(dealer) {}

    void addCard(shared_ptr<ICard>  card) override {
    if (!card) {
        return;
    }
    hand.push_back(card);
}


    int calculateScore() override {
        int score = 0;
        int aces = 0;

        for (const auto& card : hand) {
            if (card != nullptr) { 
                score += card->getValue();
                if (card->getRank() == "Ace") aces++;
            }
        }

        while (score > 21 && aces > 0) {
            score -= 10;
            aces--;
        }

        return score;
    }


    void resetHand() override {
        hand.clear();
    }

    vector<shared_ptr<ICard> > getHand() const override {
        return hand;
    }

    bool isBusted() override {
        return calculateScore() > 21;
    }

    bool hasBlackjack() override {
        return hand.size() == 2 && calculateScore() == 21;
    }

    bool isDealerPlayer() const {
        return isDealer;
    }
};



class IGame {
public:
    virtual void startGame() = 0;
    virtual void playerHit() = 0;
    virtual void playerStand() = 0;
    virtual void render() = 0;
    virtual void handleInput() = 0;
    virtual ~IGame() = default;
};


class BlackjackGame : public IGame {
private:
    Button hitButton;
    Button standButton;
    Button continueButton;
    unique_ptr<IDeck> deck;
    unique_ptr<IPlayer> player;
    unique_ptr<IPlayer> dealer;
    bool playerTurn;
    bool gameOver;
    string message;

public:
     BlackjackGame()
        : hitButton(200, 520, 100, 40, "Hit"),
          standButton(500, 520, 100, 40, "Stand"),
          continueButton(350, 520, 100, 40, "Continue"),
          deck(make_unique<Deck>()),
          player(make_unique<Player>()),
          dealer(make_unique<Player>(true)),
          playerTurn(true),
          gameOver(false),
          message("") {}
          
    void startGame() override {
        player->resetHand();
        dealer->resetHand();
        deck->shuffleDeck();
        playerTurn = true;
        gameOver = false;
        message = "";

        player->addCard(deck->drawCard());
        dealer->addCard(deck->drawCard());
        player->addCard(deck->drawCard());
        dealer->addCard(deck->drawCard());

        checkBlackjack();
    }

    void checkBlackjack() {
        if (player->hasBlackjack()) {
            if (dealer->hasBlackjack()) {
                message = "Tie!";
            } else {
                message = "Player Blackjack!";
            }
            gameOver = true;
        } else if (dealer->hasBlackjack()) {
            message = "Dealer Blackjack!";
            gameOver = true;
        }
    }

    void playerHit() override {
        if (!gameOver && playerTurn) {
            player->addCard(deck->drawCard());
            if (player->isBusted()) {
                message = "Dealer wins!";
                gameOver = true;
            }
        }
    }

    void playerStand() override {
        if (!gameOver && playerTurn) {
            playerTurn = false;
            dealerTurn();
        }
    }

    void dealerTurn() {
        while (dealer->calculateScore() < 17) {
            dealer->addCard(deck->drawCard());
        }
        if (dealer->isBusted()) {
            message = "Player wins!";
        } else {
            evaluateWinner();
        }
        gameOver = true;
    }

    void evaluateWinner() {
        int playerScore = player->calculateScore();
        int dealerScore = dealer->calculateScore();

        if (playerScore > dealerScore) {
            message = "Player wins!";
        } else if (playerScore < dealerScore) {
            message = "Dealer wins!";
        } else {
            message = "It's a tie!";
        }
    }

    void render() override {
        ClearBackground(DARKGREEN);

        DrawText("Player's Hand:", 20, 330, 20, RAYWHITE); 
        auto playerHand = player->getHand();
        float playerStartX = 400 - (playerHand.size() * 60);
        for (size_t i = 0; i < playerHand.size(); i++) {
            DrawRectangle(playerStartX + i * 120, 350, 100, 140, LIGHTGRAY); 
            DrawText(playerHand[i]->getRank().c_str(), playerStartX + 10 + i * 120, 360, 20, BLACK); 
            DrawText(playerHand[i]->getSuit().c_str(), playerStartX + 10 + i * 120, 390, 20, BLACK); 
        }

        DrawText("Dealer's Hand:", 20, 30, 20, RAYWHITE); 
        auto dealerHand = dealer->getHand();
        float dealerStartX = 400 - (dealerHand.size() * 60);

        for (size_t i = 0; i < dealerHand.size(); i++) {
            if (i == 0 && (gameOver || !playerTurn)) {
                DrawRectangle(dealerStartX + i * 120, 50, 100, 140, LIGHTGRAY); 
                DrawText(dealerHand[i]->getRank().c_str(), dealerStartX + 10 + i * 120, 60, 20, BLACK); 
                DrawText(dealerHand[i]->getSuit().c_str(), dealerStartX + 10 + i * 120, 90, 20, BLACK); 
            }
            else if (i > 0 || !playerTurn) {
                DrawRectangle(dealerStartX + i * 120, 50, 100, 140, LIGHTGRAY); 
                DrawText(dealerHand[i]->getRank().c_str(), dealerStartX + 10 + i * 120, 60, 20, BLACK); 
                DrawText(dealerHand[i]->getSuit().c_str(), dealerStartX + 10 + i * 120, 90, 20, BLACK); 
            } else {
                DrawRectangle(dealerStartX + i * 120, 50, 100, 140, DARKGRAY); 
                DrawText("Hidden", dealerStartX + 10, 80, 20, BLACK); 
            }
        }

        if (!playerTurn || gameOver) {
            DrawText(TextFormat("Dealer's Score: %d", dealer->calculateScore()), 20, 195, 20, RAYWHITE); 
        }
        DrawText(TextFormat("Player's Score: %d", player->calculateScore()), 20, 495, 20, RAYWHITE); 

        if (gameOver) {
            DrawText(message.c_str(), 300, 250, 30, YELLOW); 
        }
        hitButton.render();
        standButton.render();
        if (gameOver) {
            continueButton.render();
        } else {
            hitButton.render();
            standButton.render();
        }
    }


    void handleInput() override {
        Vector2 mousePos = GetMousePosition();

        if (gameOver) {
            continueButton.update(mousePos);
            if (continueButton.isClicked()) {
                startGame(); 
            }
        } else {
            hitButton.update(mousePos);
            standButton.update(mousePos);

            if (hitButton.isClicked() && playerTurn) {
                playerHit();
            }
            if (standButton.isClicked() && playerTurn) {
                playerStand();
            }
        }
    }
};

int main() {
    InitWindow(800, 600, "Blackjack with raylib");
    SetTargetFPS(60);

    unique_ptr<IGame> game = make_unique<BlackjackGame>();
    game->startGame();

    while (!WindowShouldClose()) {
        BeginDrawing();
        game->render();
        EndDrawing();

        game->handleInput();
    }

    CloseWindow();
    return 0;
}
