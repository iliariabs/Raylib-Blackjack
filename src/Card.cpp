#include "Card.h"

Card::Card(std::string r, std::string s, int v) : rank(r), suit(s), value(v) {}

std::string Card::getRank() const {
    return rank;
}

std::string Card::getSuit() const {
    return suit;
}

int Card::getValue() const {
    return value;
}
