#include "Graph.h"

void Graph::Update() {
    pin_mgr.Update();

    for (auto& node : nodes) {
        node.Update();
    }

    Compute();

    data.pixel_coord.x++;

    if (data.pixel_coord.x >= data.img_size.x)
        data.pixel_coord.y++,
        data.pixel_coord.x = 0;

    if (data.pixel_coord.y >= data.img_size.y)
        data.pixel_coord.y = 0;
}

void Graph::AddNode(Node::Type type) {
    nodes.emplace_back(type, pin_mgr, &data);
}

void Graph::draw(sf::RenderTarget& rt, sf::RenderStates rs) const {
    for (auto& node : nodes) {
        rt.draw(node);
    }

    rt.draw(pin_mgr);
}

void Graph::Compute() {
    for (auto& node : nodes) {
        node.processed = false;

        if (node.out_pins.empty()) {
            ReverseSearch(node);
        }
    }
}

void Graph::ReverseSearch(Node& node) {
    if (node.processed) return;

    for (auto in_pin : node.in_pins) {
        if (in_pin->bridge.other) {
            ReverseSearch(*(Node*)in_pin->bridge.other->node);

            in_pin->bridge.TransferData();
        }
    }

    node.processed = true;

    node.Compute();
}