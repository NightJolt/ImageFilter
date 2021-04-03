
#include <NodeInfo.h>

#include "NodeInfo.h"
#include "Node.h"

const NodeInfo Node::nodes[] = {
    NodeInfo {
        "add",
        2, 1,
        { Pin::Type::F, Pin::Type::F },
        { Pin::Type::F }
    },
    NodeInfo {
        "sub",
        2, 1,
        { Pin::Type::F, Pin::Type::F },
        { Pin::Type::F }
    },
    NodeInfo {
        "mul",
        2, 1,
        { Pin::Type::F, Pin::Type::F },
        { Pin::Type::F }
    },
    NodeInfo {
        "div",
        2, 1,
        { Pin::Type::F, Pin::Type::F },
        { Pin::Type::F }
    },
    NodeInfo {
        "grayscale",
        1, 1,
        { Pin::Type::V3F },
        { Pin::Type::V3F }
    },
    NodeInfo {
        "split",
        1, 3,
        { Pin::Type::V3F },
        { Pin::Type::F, Pin::Type::F, Pin::Type::F }
    },
    NodeInfo {
        "join",
        3, 1,
        { Pin::Type::F, Pin::Type::F, Pin::Type::F },
        { Pin::Type::V3F }
    },
    NodeInfo {
        "img pos",
        0, 1,
        {},
        { Pin::Type::V2I }
    },
    NodeInfo {
        "img reader",
        1, 1,
        { Pin::Type::V2I },
        { Pin::Type::V3F }
    },
    NodeInfo {
        "img writer",
        1, 0,
        { Pin::Type::V3F },
        {}
    }
};