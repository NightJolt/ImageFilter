#pragma once

#include "globals.h"

struct NodeData {
    NodeData() {
        img_buffer = nullptr;
    }

    NodeData(const NodeData&) = delete;
    NodeData& operator =(const NodeData&) = delete;
    NodeData(NodeData&&) noexcept;

    sf::Image* img_buffer;
};