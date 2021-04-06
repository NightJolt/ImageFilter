#include "Node.h"

Node::Node(Type t, PinManager& pin_mgr, GraphData* g_d) : Interactable(LOCALIZE(bool, { return InteractionFun(); })), type(t), graph_data(g_d), node_data(NodeData()) {
    label.setString(nodes[type].name);
    label.setFont(fun::R::fonts[0]);
    label.setScale(1, 1);
    label.setFillColor(sf::Color(200, 200, 200));

    sf::Vector2f label_size = sf::Vector2f(label.getGlobalBounds().width, label.getGlobalBounds().height);

    label.setOrigin(label_size * sf::Vector2f(.5f, .5f));

    body_size.x = init_body_size.x + label_size.x;
    body_size.y = init_body_size.y * (float)(std::max(nodes[type].in_cnt, nodes[type].out_cnt) + 1);

    body.setFillColor(sf::Color(54, 54, 54));
    body.setOrigin(body_size * .5f);
    body.setSize(body_size);

    in_pins.resize(nodes[type].in_cnt);
    out_pins.resize(nodes[type].out_cnt);

    for (int i = 0; i < in_pins.size(); i++) {
        in_pins[i] = new Pin(nodes[type].in_types[i], Pin::Dir::IN, this);

        pin_mgr.RegisterPin(in_pins[i]);
    }

    for (int i = 0; i < out_pins.size(); i++) {
        out_pins[i] = new Pin(nodes[type].out_types[i], Pin::Dir::OUT, this);

        pin_mgr.RegisterPin(out_pins[i]);
    }

    // for testing
    body_pos += sf::Vector2f(fun::Math::Random(0, 501) - 250, fun::Math::Random(0, 501) - 250);

    if (type == ImgWriter) {
        node_data.img_buffer = new sf::Image;
    }
}

Node::~Node() {
    for (auto in_pin : in_pins) {
        in_pin->PrepareToDie();
    }

    for (auto out_pin : out_pins) {
        out_pin->PrepareToDie();
    }

    if (type == ImgWriter) delete node_data.img_buffer;
}

Node::Node(Node&& node) noexcept : Interactable(LOCALIZE(bool, { return InteractionFun(); })),

    type(node.type),
    in_pins(std::move(node.in_pins)),
    out_pins(std::move(node.out_pins)),
    label(std::move(node.label)),
    body(std::move(node.body)),
    body_size(node.body_size),
    body_pos(node.body_pos),
    drag_offset(node.drag_offset),
    processed(node.processed),
    node_data(std::move(node.node_data)),
    graph_data(node.graph_data) {

    for (auto pin : in_pins) {
        pin->node = this;
    }

    for (auto pin : out_pins) {
        pin->node = this;
    }
}

void Node::draw(sf::RenderTarget& rt, sf::RenderStates rs) const {
    rt.draw(body);
    rt.draw(label);

    for (auto in_pin : in_pins) {
        rt.draw(*in_pin);
    }

    for (auto out_pin : out_pins) {
        rt.draw(*out_pin);
    }
}

void Node::Update() {
    Interactable::Update();

    sf::Vector2f relative_pos = fun::Math::ScreenToWorld(sf::Mouse::getPosition(*glob_window), *glob_window) - body_pos;

    if (Interact_RightPressed()) drag_offset = relative_pos;
    if (Interact_RightHold()) body_pos += relative_pos - drag_offset;

    sf::Vector2f in_start_pnt = body_pos - sf::Vector2f(body_size.x, body_size.y) * .5f;
    sf::Vector2f out_start_pnt = body_pos + sf::Vector2f(body_size.x, -body_size.y) * .5f;

    float in_step = body_size.y / (float)(nodes[type].in_cnt + 1);
    float out_step = body_size.y / (float)(nodes[type].out_cnt + 1);

    //float curve_step = 1.f / (in_pins.size() - 1);
    //float center = (in_pins.size() == 1) * .5f;

    for (int i = 0; i < in_pins.size(); i++) {
        in_pins[i]->body.setPosition(in_start_pnt + sf::Vector2f(0, in_step * (float)(i + 1)));
        //in_pins[i]->bridge.line.CurveAt(1.f - curve_step * i + center);
    }

    for (int i = 0; i < out_pins.size(); i++) {
        out_pins[i]->body.setPosition(out_start_pnt + sf::Vector2f(0, out_step * (float)(i + 1)));
    }

    body.setPosition(body_pos);
    label.setPosition(body_pos);
}

void Node::Compute() {
    if (type == Add) {
        *reinterpret_cast<float*>(out_pins[0]->data) = *reinterpret_cast<float*>(in_pins[0]->data) + *reinterpret_cast<float*>(in_pins[1]->data);

        label.setString(nodes[type].name + '\n' + std::to_string(*reinterpret_cast<float*>(in_pins[0]->data) + *reinterpret_cast<float*>(in_pins[1]->data)));
    } else if (type == Sub) {
        *reinterpret_cast<float*>(out_pins[0]->data) = *reinterpret_cast<float*>(in_pins[0]->data) - *reinterpret_cast<float*>(in_pins[1]->data);
    } else if (type == Mul) {
        *reinterpret_cast<float*>(out_pins[0]->data) = *reinterpret_cast<float*>(in_pins[0]->data) * *reinterpret_cast<float*>(in_pins[1]->data);
    } else if (type == Div) {
        *reinterpret_cast<float*>(out_pins[0]->data) = *reinterpret_cast<float*>(in_pins[0]->data) / *reinterpret_cast<float*>(in_pins[1]->data);
    } else if (type == Grayscale) {
        auto* data = reinterpret_cast<sf::Vector3f*>(in_pins[0]->data);

        float val = (data->x + data->y + data->z) * 0.333f;

        reinterpret_cast<sf::Vector3f*>(out_pins[0]->data)->x = val;
        reinterpret_cast<sf::Vector3f*>(out_pins[0]->data)->y = val;
        reinterpret_cast<sf::Vector3f*>(out_pins[0]->data)->z = val;
    } else if (type == Split) {
        auto* data = reinterpret_cast<sf::Vector3f*>(in_pins[0]->data);

        *reinterpret_cast<float*>(out_pins[0]->data) = data->x;
        *reinterpret_cast<float*>(out_pins[1]->data) = data->y;
        *reinterpret_cast<float*>(out_pins[2]->data) = data->z;

        label.setString(nodes[type].name + '\n' + std::to_string(*reinterpret_cast<float*>(out_pins[0]->data)) + ' ' + std::to_string(*reinterpret_cast<float*>(out_pins[1]->data)) + ' ' + std::to_string(*reinterpret_cast<float*>(out_pins[2]->data)));
    } else if (type == Join) {
        reinterpret_cast<sf::Vector3f*>(out_pins[0]->data)->x = *reinterpret_cast<float*>(in_pins[0]->data);
        reinterpret_cast<sf::Vector3f*>(out_pins[0]->data)->y = *reinterpret_cast<float*>(in_pins[1]->data);
        reinterpret_cast<sf::Vector3f*>(out_pins[0]->data)->z = *reinterpret_cast<float*>(in_pins[2]->data);
    } else if (type == ImgPos) {
        *reinterpret_cast<sf::Vector2i*>(out_pins[0]->data) = graph_data->pixel_coord;

        label.setString(nodes[type].name + '\n' + to_string(*reinterpret_cast<sf::Vector2i*>(out_pins[0]->data)));
    } else if (type == ImgReader) {
        reinterpret_cast<sf::Vector3f*>(out_pins[0]->data)->x = (float)reinterpret_cast<sf::Vector2i*>(in_pins[0]->data)->x;
        reinterpret_cast<sf::Vector3f*>(out_pins[0]->data)->y = (float)reinterpret_cast<sf::Vector2i*>(in_pins[0]->data)->y;
        reinterpret_cast<sf::Vector3f*>(out_pins[0]->data)->z = 0;

        label.setString(nodes[type].name + '\n' + to_string(*reinterpret_cast<sf::Vector2f*>(out_pins[0]->data)));
    } else if (type == ImgWriter) {
        //*reinterpret_cast<sf::Vector3i*>(out_pins[0]->data) = *reinterpret_cast<sf::Vector3i*>(in_pins[0]->data);
    }
}

bool Node::InteractionFun() {
    return fun::Math::Abs(fun::Math::ScreenToWorld(sf::Mouse::getPosition(*glob_window), *glob_window) - body_pos) < body_size * .5f;
}
