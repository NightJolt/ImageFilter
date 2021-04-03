#pragma once

#include "globals.h"

#include "Node.h"
#include "PinManager.h"

class Graph : public sf::Drawable {
public:

    void Update();

    void Compute();
    PinManager& GetPinManager();
    void AddNode(Node::Type);

private:

    void ReverseSearch(Node&);

    PinManager pin_mgr;
    GraphData data;

    std::vector <Node> nodes;

    void draw(sf::RenderTarget&, sf::RenderStates) const override;
};