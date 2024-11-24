#ifndef DECK_H
#define DECK_H

#include "IDeck.h"
#include "Card.h"
#include <vector>
#include <random>
#include <algorithm>

class Deck : public IDeck {
private:
    std::vector<std::shared_ptr<ICard>> cards;

public:
    Deck();

    void shuffleDeck() override;
    void refillAndShuffleDeck() override;
    std::shared_ptr<ICard> drawCard() override;
};

#endif // DECK_H
