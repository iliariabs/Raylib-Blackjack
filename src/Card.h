#ifndef CARD_H
#define CARD_H

#include "ICard.h"

class Card : public ICard {
private:
    std::string rank;
    std::string suit;
    int value;

public:
    Card(std::string r, std::string s, int v);

    std::string getRank() const override;
    std::string getSuit() const override;
    int getValue() const override;
};

#endif // CARD_H
