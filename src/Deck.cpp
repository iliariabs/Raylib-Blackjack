#include "Deck.h"

Deck::Deck() {
    std::string suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
    std::string ranks[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King", "Ace"};
    int values[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11};

    for (const auto& suit : suits) {
        for (int i = 0; i < 13; i++) {
            cards.push_back(std::make_shared<Card>(ranks[i], suit, values[i]));
        }
    }
    shuffleDeck();
}

void Deck::shuffleDeck() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cards.begin(), cards.end(), g);
}

void Deck::refillAndShuffleDeck() {
    cards.clear();
    std::string suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
    std::string ranks[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King", "Ace"};
    int values[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11};

    for (const auto& suit : suits) {
        for (int i = 0; i < 13; i++) {
            cards.push_back(std::make_shared<Card>(ranks[i], suit, values[i]));
        }
    }
    shuffleDeck();
}

std::shared_ptr<ICard> Deck::drawCard() {
    if (cards.empty()) {
        refillAndShuffleDeck();
    }
    std::shared_ptr<ICard> card = cards.back();
    cards.pop_back();
    return card;
}
