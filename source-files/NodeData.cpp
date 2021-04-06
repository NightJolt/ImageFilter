#include <NodeData.h>

NodeData::NodeData(NodeData&& node_data) noexcept {
    img_buffer = node_data.img_buffer;
    node_data.img_buffer = nullptr;
}