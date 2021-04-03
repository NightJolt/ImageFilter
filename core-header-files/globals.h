#pragma once

#pragma region flags

#define ENABLE_PRINTING

#pragma endregion

#pragma region libs

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <imgui.h>
#include <imgui-SFML.h>

#pragma endregion

#pragma region stdlibs

#if defined(ENABLE_PRINTING)
    #include <iostream>
#endif

#include <memory>
#include <cstring>
#include <map>
#include <cmath>
#include <vector>
#include <array>

#pragma endregion

#pragma region window

#define INIT_SCREEN_SIZE sf::Vector2i(1920, 1440)
#define INIT_VIEW_SIZE sf::Vector2f(1920, 1440)
#define INIT_VIEW_ORIGIN sf::Vector2f(0, 0)

#pragma endregion

#pragma region tilemap

#define CHUNK_SIZE 16
#define TILE_SIZE 8.f
#define TILE_TEXTURE_SIZE 8
#define TILE_ATLAS_SIZE 256
#define RAND_TEXTURE_COUNT 2

#pragma endregion

extern sf::RenderWindow* glob_window; // todo: Move to window manager

#define LOCALIZE(type, body) [this]() -> type body


struct UniqueKey {
    UniqueKey(void* p, const char* k) : ptr(p), key(strdup(k)) {}
    ~UniqueKey() { free(key); }

    bool operator <(const UniqueKey& other) const {
        return strcmp(key, other.key) < 0;
    }

    UniqueKey(const UniqueKey& other) {
        ptr = other.ptr;
        key = strdup(other.key);
    }

    UniqueKey& operator =(const UniqueKey& other) {
        if (&other == this) return *this;

        ptr = other.ptr;
        key = strdup(other.key);

        return *this;
    }

    UniqueKey(UniqueKey&& other) noexcept {
        ptr = other.ptr;
        key = other.key;

        other.ptr = nullptr;
        other.key = nullptr;
    }

    void* ptr;
    char* key;
};

struct Col {
    float r, g, b;

    Col() : r(1.f), g(1.f), b(1.f) {}

    Col(float r, float g, float b) : r(r), g(g), b(b) {}

    explicit Col(const sf::Color& c) {
        r = (float)c.r / 255.f;
        g = (float)c.g / 255.f;
        b = (float)c.b / 255.f;
    }

    explicit operator sf::Color() const {
        return sf::Color(r * 255.f, g * 255.f, b * 255.f);
    }

    Col operator *(float v) const {
        return Col(r * v, g * v, b * v);
    }

    Col operator *(const Col& c) const {
        return Col(r * c.r, g * c.g, b * c.b);
    }

    Col pow(int v) const {
        Col n_col(r, g, b);

        for (int i = 0; i < v; i++) n_col = n_col * *this;

        return n_col;
    }
};


template <typename T>
std::string to_string_key(const sf::Vector2<T>& a) {
    return std::to_string(a.x) + ' ' + std::to_string(a.y);
}

template <typename T>
std::string to_string(const sf::Vector2<T>& a) {
    return "Vector2(" + std::to_string(a.x) + ", " + std::to_string(a.y) + ")";
}

template <typename T>
std::ostream& operator <<(std::ostream& out, const sf::Vector2<T>& a) {
    return out << to_string(a);
}

template <typename T>
sf::Vector2<T> operator *(const sf::Vector2<T>& a, const sf::Vector2<T>& b) {
    return sf::Vector2<T>(a.x * b.x, a.y * b.y);
}

template <typename T>
void operator *=(sf::Vector2<T>& a, const sf::Vector2<T>& b) {
    a = a * b;
}

template <typename T>
sf::Vector2<T> operator /(const sf::Vector2<T>& a, const sf::Vector2<T>& b) {
    return sf::Vector2<T>(a.x / b.x, a.y / b.y);
}

template <typename T>
void operator /=(sf::Vector2<T>& a, const sf::Vector2<T>& b) {
    a = a / b;
}

template <typename T>
sf::Vector2<T> operator +(const sf::Vector2<T>& a, const T& b) {
    return sf::Vector2<T>(a.x + b, a.y + b);
}

template <typename T>
void operator +=(sf::Vector2<T>& a, const T& b) {
    a = a + b;
}

template <typename T>
sf::Vector2<T> operator -(const sf::Vector2<T>& a, const T& b) {
    return sf::Vector2<T>(a.x - b, a.y - b);
}

template <typename T>
void operator -=(sf::Vector2<T>& a, const T& b) {
    a = a - b;
}

template <typename T>
bool operator <(const sf::Vector2<T>& a, const sf::Vector2<T>& b) {
    return a.x < b.x && a.y < b.y;
}

template <typename T>
bool operator <=(const sf::Vector2<T>& a, const sf::Vector2<T>& b) {
    return a.x <= b.x && a.y <= b.y;
}

template <typename T>
bool operator >(const sf::Vector2<T>& a, const sf::Vector2<T>& b) {
    return b < a;
}

template <typename T>
bool operator >=(const sf::Vector2<T>& a, const sf::Vector2<T>& b) {
    return b <= a;
}