#ifndef BUTTON_H
#define BUTTON_H

#include "raylib.h"
#include <string>

class Button {
private:
    Rectangle bounds;       
    Color color;            
    Color hoverColor;       
    Color textColor;        
    std::string text;       
    bool isHovered;         

public:
    Button(float x, float y, float width, float height, const std::string& text, Color color = GRAY, Color hoverColor = DARKGRAY, Color textColor = WHITE);

    void render();
    void update(Vector2 mousePos);
    bool isClicked() const;
};

#endif // BUTTON_H
