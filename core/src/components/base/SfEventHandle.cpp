#include "components/base/SfEventHandle.h"

using namespace QSFML::Components;

SfEventHandle::SfEventHandle(const std::string &name)
    : Component(name)
{

}
SfEventHandle::SfEventHandle(const SfEventHandle &other)
    : Component(other)
{

}
SfEventHandle::~SfEventHandle()
{

}
