#pragma once

#include "globals.h"

struct GraphData {
    sf::Vector2i img_size;
    sf::Vector2i pixel_coord;

    void* active_node = nullptr;
    bool is_ui_active = false;
};