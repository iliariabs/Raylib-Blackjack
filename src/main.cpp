#include "raylib.h"
#include "BlackJackGame.h"
#include "IGame.h"

#include <ctime>
#include <cstdlib>

using namespace std;

int main() {
    InitWindow(800, 600, "Blackjack with raylib");
    SetTargetFPS(60);

    unique_ptr<IGame> game = make_unique<BlackjackGame>();
    game->startGame();

    while (!WindowShouldClose()) {
        BeginDrawing();
        game->render();
        EndDrawing();

        game->handleInput();
    }

    CloseWindow();
    return 0;
}

