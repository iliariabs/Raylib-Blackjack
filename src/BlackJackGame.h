#ifndef BLACKJACKGAME_H
#define BLACKJACKGAME_H

#include <memory>
#include <string>
#include "IGame.h"
#include "Button.h"
#include "Deck.h"
#include "Player.h"

class BlackjackGame : public IGame {
private:
    Button hitButton;
    Button standButton;
    Button continueButton;
    std::unique_ptr<IDeck> deck;
    std::unique_ptr<IPlayer> player;
    std::unique_ptr<IPlayer> dealer;
    bool playerTurn;
    bool gameOver;
    std::string message;

    void checkBlackjack();
    void dealerTurn();
    void evaluateWinner();

public:
    BlackjackGame();

    void startGame() override;
    void playerHit() override;
    void playerStand() override;
    void render() override;
    void handleInput() override;
};

#endif // BLACKJACKGAME_H
