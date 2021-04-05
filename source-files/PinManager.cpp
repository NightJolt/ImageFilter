#include "PinManager.h"

PinManager::~PinManager() {
    CollectGarbage();
}

void PinManager::RegisterPin(Pin* pin) {
    pins.push_back(pin);
}

void PinManager::CollectGarbage() {
    std::vector <int> garbage;

    for (int i = 0; i < pins.size(); i++) {
        if (!pins[i]->IsAlive()) garbage.push_back(i);
    }

    for (auto pin : pins) {
        if (pin->dir == Pin::Dir::OUT || !pin->IsAlive()) continue;

        for (int i : garbage) {
            if (pin->GetBridge().other && pin->GetBridge().other == pins[i]) {
                pin->DestroyBridge();
            }
        }
    }

    for (int i = (int)garbage.size() - 1; i >= 0; i--) {
        delete pins[garbage[i]];

        pins.erase(pins.begin() + garbage[i]);
    }
}

void PinManager::Update() {
    CollectGarbage();

    //std::cout << pins.size() << std::endl;

    for (auto pin : pins) {
        pin->Update();

        if (pin->Interact_LeftPressed()) {
            selected_pin = pin;
        }

        if (pin->Interact_LeftReleased()) {
            if (selected_pin && pin != selected_pin && pin->type == selected_pin->type && pin->dir != selected_pin->dir && pin->node != selected_pin->node) {
                if (pin->dir == Pin::Dir::IN) {
                    pin->CreateBridge(selected_pin);
                } else {
                    selected_pin->CreateBridge(pin);
                }
            }
        }
    }

    if (fun::Input::Released(sf::Mouse::Left)) {
        selected_pin = nullptr;
    }

    if (selected_pin) drag_line.SetPoints(selected_pin->body.getPosition(), fun::Math::ScreenToWorld(sf::Mouse::getPosition(*glob_window), *glob_window));
}

void PinManager::draw(sf::RenderTarget& rt, sf::RenderStates rs) const {
    if (selected_pin) {
        rt.draw(drag_line);
    }

    for (auto pin : pins) {
        rt.draw(pin->bridge);
    }
}