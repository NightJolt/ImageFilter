#pragma once

#include "globals.h"

#include "Node.h"
#include "PinManager.h"

class Graph : public sf::Drawable {
public:

    void Update();

    void Compute();

    void AddNode(Node::Type);

private:

    void ReverseSearch(Node&);

    void draw(sf::RenderTarget&, sf::RenderStates) const override;

    PinManager pin_mgr;
    GraphData data;

    std::vector <Node> nodes;
};