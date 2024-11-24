#ifndef IDECK_H
#define IDECK_H

#include <memory>
#include "ICard.h"

class IDeck {
public:
    virtual std::shared_ptr<ICard> drawCard() = 0;
    virtual void shuffleDeck() = 0;
    virtual void refillAndShuffleDeck() = 0;
    virtual ~IDeck() = default;
};

#endif // IDECK_H
