#pragma once

#include "Input.h"
#include "R.h"
#include "Pin.h"
#include "PinManager.h"
#include "Interactable.h"
#include "NodeInfo.h"
#include "GraphData.h"

#include "globals.h"

class Node : public sf::Drawable, public Interactable {
public:

    enum Type {
        Add,
        Sub,
        Mul,
        Div,
        Grayscale,
        Split,
        Join,
        ImgPos,
        ImgReader,
        ImgWriter,

        Count
    } type;

    explicit Node(Type, PinManager&, GraphData*);
    ~Node() override;

    Node(const Node&) = delete;
    Node& operator =(const Node&) = delete;
    Node(Node&&) noexcept;

    void Update() override;

    void Compute();

    std::vector <Pin*> in_pins;
    std::vector <Pin*> out_pins;

    bool processed;

private:

    bool InteractionFun();

    void draw(sf::RenderTarget&, sf::RenderStates) const override;

    static const NodeInfo nodes[];

    bool has_ownership;

    GraphData* graph_data;
    sf::Text label;
    sf::RectangleShape body;
    const sf::Vector2f init_body_size = { 40, 40 };
    sf::Vector2f body_size;
    sf::Vector2f body_pos;
    sf::Vector2f drag_offset;
};