#ifndef IPLAYER_H
#define IPLAYER_H

#include <vector>
#include <memory>
#include "ICard.h"

class IPlayer {
public:
    virtual void addCard(std::shared_ptr<ICard> card) = 0;
    virtual int calculateScore() = 0;
    virtual void resetHand() = 0;
    virtual bool isBusted() = 0;
    virtual bool hasBlackjack() = 0;
    virtual std::vector<std::shared_ptr<ICard>> getHand() const = 0;
    virtual ~IPlayer() = default;
};

#endif // IPLAYER_H
