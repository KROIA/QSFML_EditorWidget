#include "components/base/SfEventHandle.h"
#include "scene/Scene.h"

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

void SfEventHandle::setSceneParent(Scene* parent)
{
    Scene* oldParent = m_sceneParent;
    if (oldParent)
    {
        if (oldParent->getDefaultCamera())
            removeCameraFilter(oldParent->getDefaultCamera());
    }
    Component::setSceneParent(parent);
    if (!parent)
        return;
    Objects::CameraWindow* cam = parent->getDefaultCamera();
    if(m_cameraFilter.empty())
        addCameraFilter(cam);
}
