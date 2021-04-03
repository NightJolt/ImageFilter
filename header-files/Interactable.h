#pragma once

#include "globals.h"

#include "Math.h"
#include "Input.h"

class Interactable {
public:

    typedef std::function<bool()> INTERACTION_FUN;

    explicit Interactable(INTERACTION_FUN);
    virtual ~Interactable();

    virtual void Update();

    [[nodiscard]] bool Interact_RightPressed() const;
    [[nodiscard]] bool Interact_RightHold() const;
    [[nodiscard]] bool Interact_RightReleased() const;

    [[nodiscard]] bool Interact_LeftPressed() const;
    [[nodiscard]] bool Interact_LeftHold() const;
    [[nodiscard]] bool Interact_LeftReleased() const;

    [[nodiscard]] bool Interact_HoverEnter() const;
    [[nodiscard]] bool Interact_Hovered() const;
    [[nodiscard]] bool Interact_HoverExit() const;

    static const sf::Vector2f INTERACT_FALSE;

private:

    bool r_pressed;
    bool r_hold;
    bool r_released;

    bool l_pressed;
    bool l_hold;
    bool l_released;

    bool hover_enter;
    bool hovered;
    bool hover_exit;

    INTERACTION_FUN interaction_fun;
};