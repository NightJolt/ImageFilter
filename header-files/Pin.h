#pragma once

#include "Math.h"
#include "Input.h"

#include "CurvedLine.h"

#include "Interactable.h"
#include "Enitity.h"

#include "globals.h"

class Pin : public sf::Drawable, public Interactable, public Entity {
public:

    struct Bridge : public sf::Drawable {
        explicit Bridge(Pin*);

        void Update();
        void TransferData() const;

        Pin* pin;
        Pin* other = nullptr;
        CurvedLine line;

        void draw(sf::RenderTarget&, sf::RenderStates) const override;
    };

    enum struct Dir {
        IN,
        OUT
    } dir ;

    enum struct Type {
        F,
        I,
        V2F,
        V3F,
        V2I,
        V3I
    } type;

    Pin(Type, Dir, void*, void* = nullptr);
    ~Pin() override;

    Pin(const Pin&) = delete;
    Pin& operator =(const Pin&) = delete;
    Pin(Pin&&) noexcept = delete;

    void PrepareToDie() override;

    void Update() override;

    void CreateBridge(Pin*);
    const Bridge& GetBridge() const;
    void DestroyBridge();

    Bridge bridge;

    void* node;
    void* data;

    sf::CircleShape body;

private:

    bool InteractionFun();

    void draw(sf::RenderTarget&, sf::RenderStates) const override;

    static constexpr int type_sizes[] = {
            sizeof(float),
            sizeof(int),
            sizeof(sf::Vector2f),
            sizeof(sf::Vector3f),
            sizeof(sf::Vector2i),
            sizeof(sf::Vector3i)
    };

    const float radius = 10;
};