#include "Button.h"

Button::Button(float x, float y, float width, float height, const std::string& text, Color color, Color hoverColor, Color textColor)
    : bounds({x, y, width, height}), color(color), hoverColor(hoverColor), textColor(textColor), text(text), isHovered(false) {}

void Button::render() {
    DrawRectangleRec(bounds, isHovered ? hoverColor : color); 
    float textWidth = MeasureText(text.c_str(), 20);
    DrawText(text.c_str(), bounds.x + (bounds.width - textWidth) / 2, bounds.y + (bounds.height - 20) / 2, 20, textColor);
}

void Button::update(Vector2 mousePos) {
    isHovered = CheckCollisionPointRec(mousePos, bounds);
}

bool Button::isClicked() const {
    return isHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}
