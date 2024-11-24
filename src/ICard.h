#ifndef ICARD_H
#define ICARD_H

#include <string>

class ICard {
public:
    virtual std::string getRank() const = 0;
    virtual std::string getSuit() const = 0;
    virtual int getValue() const = 0;
    virtual ~ICard() = default;
};

#endif // ICARD_H
