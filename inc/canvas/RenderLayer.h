#pragma once

#include <cstddef>
namespace QSFML
{

enum RenderLayer: size_t
{
    layer_0,
    layer_1,
    layer_2,
    layer_3,
    layer_4,
    layer_5,
    count    // Do not use this layer, it is only used to create each layer
};
}
