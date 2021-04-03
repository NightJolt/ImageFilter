#include "Enitity.h"

Entity::Entity() : alive(true) {}

Entity::~Entity() = default;

void Entity::PrepareToDie() {
    alive = false;
}

bool Entity::IsAlive() const { return alive; }