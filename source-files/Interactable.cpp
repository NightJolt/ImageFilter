#include "Interactable.h"

Interactable::Interactable(INTERACTION_FUN i_f) {
    r_pressed = false;
    r_hold = false;
    r_released = false;

    l_pressed = false;
    l_hold = false;
    l_released = false;

    hover_enter = false;
    hovered = false;
    hover_exit = false;

    interaction_fun = std::move(i_f);
}

Interactable::~Interactable() = default;

void Interactable::Update() {
    bool res = interaction_fun();

    hover_enter = false;
    hover_exit = false;

    if (res) {
        if (!hovered) {
            hover_enter = true;
            hovered = true;
        }
    } else {
        if (hovered) {
            hovered = false;
            hover_exit = true;
        }
    }

    r_pressed = false;
    r_released = false;

    if (fun::Input::Pressed(sf::Mouse::Right)) {
        if (hovered && !r_hold) {
            r_pressed = true;
            r_hold = true;
        }
    } else if (fun::Input::Released(sf::Mouse::Right)) {
        r_hold = false;
        r_released = hovered;
    }

    l_pressed = false;
    l_released = false;

    if (fun::Input::Pressed(sf::Mouse::Left)) {
        if (hovered && !l_hold) {
            l_pressed = true;
            l_hold = true;
        }
    } else if (fun::Input::Released(sf::Mouse::Left)) {
        l_hold = false;
        l_released = hovered;
    }
}

bool Interactable::Interact_RightPressed() const {
    return r_pressed;
}

bool Interactable::Interact_RightHold() const {
    return r_hold;
}

bool Interactable::Interact_RightReleased() const {
    return r_released;
}

bool Interactable::Interact_LeftPressed() const {
    return l_pressed;
}

bool Interactable::Interact_LeftHold() const {
    return l_hold;
}

bool Interactable::Interact_LeftReleased() const {
    return l_released;
}

bool Interactable::Interact_HoverEnter() const {
    return hover_enter;
}

bool Interactable::Interact_Hovered() const {
    return hovered;
}

bool Interactable::Interact_HoverExit() const {
    return hover_exit;
}