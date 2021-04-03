#pragma once

#include "globals.h"

#include "Pin.h"

struct NodeInfo {
    NodeInfo(std::string  n, int i_c, int o_c, std::vector <Pin::Type> i_t, std::vector <Pin::Type> o_t) :
            name(std::move(n)),
            in_cnt(i_c),
            out_cnt(o_c),
            in_types(std::move(i_t)),
            out_types(std::move(o_t))
    {}

    std::string name;

    int in_cnt;
    int out_cnt;

    std::vector <Pin::Type> in_types;
    std::vector <Pin::Type> out_types;
};