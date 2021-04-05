#include "Pin.h"

#pragma region Bridge

Pin::Bridge::Bridge(Pin* pin) : pin(pin) {}

void Pin::Bridge::Update() {
    if (other) line.SetPoints(other->body.getPosition(), pin->body.getPosition());
}

void Pin::Bridge::TransferData() const {
    if (!other) return;

    switch (other->type) {
        case Pin::Type::F:
            *(float*)pin->data = *(float*)other->data;

            break;
        case Pin::Type::I:
            *(int*)pin->data = *(int*)other->data;

            break;
        case Pin::Type::V2F:
            *(sf::Vector2f*)pin->data = *(sf::Vector2f*)other->data;

            break;
        case Pin::Type::V3F:
            *(sf::Vector3f*)pin->data = *(sf::Vector3f*)other->data;

            break;
        case Pin::Type::V2I:
            *(sf::Vector2i*)pin->data = *(sf::Vector2i*)other->data;

            break;
        case Pin::Type::V3I:
            *(sf::Vector3i*)pin->data = *(sf::Vector3i*)other->data;

            break;
    }
}

void Pin::Bridge::draw(sf::RenderTarget& rt, sf::RenderStates rs) const {
    if (other) rt.draw(line);
}

#pragma endregion

Pin::Pin(Type t, Dir d, void* p, void* b) : Interactable(LOCALIZE(bool, { return InteractionFun(); })), Entity(), bridge(this), type(t), dir(d), node(p) {
    body.setRadius(radius);
    body.setPointCount(20);
    body.setOrigin(radius, radius);
    body.setFillColor(sf::Color(100, 100, 100));
    body.setOutlineThickness(0);

    data = malloc(type_sizes[static_cast<int>(type)]);
}

Pin::~Pin() {
    if (data) free(data);
}

void Pin::PrepareToDie() {
    Entity::PrepareToDie();
}


void Pin::draw(sf::RenderTarget& rt, sf::RenderStates rs) const {
    if (!alive) return;

    rt.draw(body);
}

void Pin::Update() {
    Interactable::Update();

    if (Interact_HoverEnter()) {
        body.setFillColor(sf::Color(255, 100, 100));
    } else if (Interact_HoverExit()) {
        body.setFillColor(sf::Color(100, 100, 100));
    }

    bridge.Update();
}

void Pin::CreateBridge(Pin* other) {
    bridge.other = other;
}

const Pin::Bridge& Pin::GetBridge() const {
    return bridge;
}

void Pin::DestroyBridge() {
    bridge.other = nullptr;
}

bool Pin::InteractionFun() {
    const sf::Vector2f relative_pos = fun::Math::ScreenToWorld(sf::Mouse::getPosition(*glob_window),*glob_window) - body.getPosition();

    return fun::Math::Magnitude(relative_pos) <= radius;
}