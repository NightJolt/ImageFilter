#pragma once

#include "Pin.h"
#include "CurvedLine.h"

#include "globals.h"

class PinManager : public sf::Drawable {
public:

    ~PinManager() override;

    void Update();

    void RegisterPin(Pin*);

private:

    void CollectGarbage();

    void draw(sf::RenderTarget&, sf::RenderStates) const override;

    std::vector <Pin*> pins;

    Pin* selected_pin = nullptr;
    CurvedLine drag_line;
};