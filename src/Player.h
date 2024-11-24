#ifndef PLAYER_H
#define PLAYER_H

#include "IPlayer.h"

class Player : public IPlayer {
private:
    std::vector<std::shared_ptr<ICard>> hand;
    bool isDealer;

public:
    Player(bool dealer = false);

    void addCard(std::shared_ptr<ICard> card) override;
    int calculateScore() override;
    void resetHand() override;
    std::vector<std::shared_ptr<ICard>> getHand() const override;
    bool isBusted() override;
    bool hasBlackjack() override;
    bool isDealerPlayer() const;
};

#endif // PLAYER_H
