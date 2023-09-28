#pragma once

#include "QSFML_EditorWidget.h"
#include "LaserInfo.h"


class OpticalElement : public QSFML::Objects::CanvasObject
{
public:
    OpticalElement(const std::string& name = "OpticalElement");
    OpticalElement(const OpticalElement& other);
    ~OpticalElement();
    OBJECT_DECL(OpticalElement);

    static const std::vector<OpticalElement*> const& getOpticalElements();

    virtual bool processLaser(const QSFML::Utilities::Ray& ray, 
                              std::vector< QSFML::Utilities::Ray> & reflectedOut,
                              std::vector< LaserInfo> &additionalLightPathsOut) const;
    virtual bool getRaycastDistance(const QSFML::Utilities::Ray& ray, float &distanceOut) const;

protected:
    //void update() override;

private:

    static std::vector<OpticalElement*> s_opticalElements;
    
};