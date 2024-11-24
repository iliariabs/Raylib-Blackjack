#include "Player.h"

Player::Player(bool dealer) : isDealer(dealer) {}

void Player::addCard(std::shared_ptr<ICard> card) {
    if (!card) {
        return;
    }
    hand.push_back(card);
}

int Player::calculateScore() {
    int score = 0;
    int aces = 0;

    for (const auto& card : hand) {
        if (card != nullptr) { 
            score += card->getValue();
            if (card->getRank() == "Ace") {
                aces++;
            }
        }
    }

    while (score > 21 && aces > 0) {
        score -= 10;
        aces--;
    }

    return score;
}

void Player::resetHand() {
    hand.clear();
}

std::vector<std::shared_ptr<ICard>> Player::getHand() const {
    return hand;
}

bool Player::isBusted() {
    return calculateScore() > 21;
}

bool Player::hasBlackjack() {
    return hand.size() == 2 && calculateScore() == 21;
}

bool Player::isDealerPlayer() const {
    return isDealer;
}
