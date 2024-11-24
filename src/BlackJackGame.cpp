#include "BlackJackGame.h"

BlackjackGame::BlackjackGame()
    : hitButton(200, 520, 100, 40, "Hit"),
      standButton(500, 520, 100, 40, "Stand"),
      continueButton(350, 520, 100, 40, "Continue"),
      deck(std::make_unique<Deck>()),
      player(std::make_unique<Player>()),
      dealer(std::make_unique<Player>(true)),
      playerTurn(true),
      gameOver(false),
      message("") {}

void BlackjackGame::startGame() {
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

void BlackjackGame::checkBlackjack() {
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

void BlackjackGame::playerHit() {
    if (!gameOver && playerTurn) {
        player->addCard(deck->drawCard());
        if (player->isBusted()) {
            message = "Dealer wins!";
            gameOver = true;
        }
    }
}

void BlackjackGame::playerStand() {
    if (!gameOver && playerTurn) {
        playerTurn = false;
        dealerTurn();
    }
}

void BlackjackGame::dealerTurn() {
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

void BlackjackGame::evaluateWinner() {
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

void BlackjackGame::render() {
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
        } else if (i > 0 || !playerTurn) {
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
    }
}

void BlackjackGame::handleInput() {
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
