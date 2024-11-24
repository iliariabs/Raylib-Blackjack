#ifndef IGAME_H
#define IGAME_H

class IGame {
public:
    virtual void startGame() = 0;
    virtual void playerHit() = 0;
    virtual void playerStand() = 0;
    virtual void render() = 0;
    virtual void handleInput() = 0;
    virtual ~IGame() = default;
};

#endif // IGAME_H
