#pragma once

#include "globals.h"

class Entity {
public:

    Entity();
    virtual ~Entity();

    virtual void PrepareToDie();

    [[nodiscard]] bool IsAlive() const;

protected:

    bool alive;
};