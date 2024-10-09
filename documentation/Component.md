<!-- omit in toc -->
# Component
- [Predefined Components](#predefined-components)
- [Create new Components](#create-new-components)
  - [Drawable](#drawable)
  - [SfEventHandle](#sfeventhandle)
  - [AbstractPressEvent](#abstractpressevent)
  - [Updatable Component](#updatable-component)

---
Components are used to bring live to an GameObject. You can define your own Components.

## Predefined Components
* Drawables
  * Image
  * LinePainter
  * PathPainter
  * PixelPainter
  * PointPainter
  * RectPainter
  * Shape
  * Text
  * VectorFieldPainter
  * VectorPainter
* Physics Components
  * Collider
* Button
* KeyPressEvent
* MouseFollower
* MousePressEvent
* Transform

---
## Create new Components
Depending on what your Component should do, you can inherit one of the following Components:
* `Drawable`
* `SfEventHandle`
* `AbstractPressEvent`
* `QSFML::Utilities::Updatable` (Is not a Component, but can used in a Component)

### Drawable
Drawing stuff to the screen is essential in a game/simulation. Use the Drawable Component as a base class to create a custom painter.
Reimplement the `drawComponent` function to draw your stuff.
The `state` parameter already contains the transformed transform for this Component.
This enables a drawable Component to draw relative to its parent GameObject.
For exampe, if you rotate the GameObject, the painters transform also gets rotated.
If you don't want to draw relative to the parent. Just reset the transform:
``` C++
states.transform = sf::Transform::Identity;
```
<table>
<tr>
<td> LinePainter.cpp </td> <td> LinePainter.h </td>
</tr>
<tr>
<td>

```c++
Component_IMPL(LinePainter)
LinePainter::LinePainter(const std::string &name)
    : Drawable(name)
{

}
LinePainter::LinePainter(const LinePainter &other)
    : Drawable(other)
{

}
.
.
.
void LinePainter::drawComponent(sf::RenderTarget& target,
                                sf::RenderStates states) const
{ 
    for (auto& line : m_lines)
    {
        sf::Vertex m_relativeVertices[4];
        sf::Vector2f direction = line.end - line.start;
        float dist =  sqrt(direction.x*direction.x+direction.y*direction.y);
        if(dist == 0)
            continue;   
        sf::Vector2f unitDirection = direction / dist;
        sf::Vector2f unitPerpendicular(-unitDirection.y,unitDirection.x);

        sf::Vector2f offset = (line.thickness/2.f)*unitPerpendicular;

        m_relativeVertices[0].position = line.start + offset;
        m_relativeVertices[1].position = line.end + offset;
        m_relativeVertices[2].position = line.end - offset;
        m_relativeVertices[3].position = line.start - offset;
        for (int i=0; i<4; ++i)
            m_relativeVertices[i].color = line.color;
        target.draw(m_relativeVertices,4,sf::Quads, states);
    }
}

```

</td>
<td>
    
```c++

#pragma once

#include "QSFML_EditorWidget_base.h"
#include "Components/base/Drawable.h"

namespace QSFML
{
namespace Components
{
class QSFML_EDITOR_WIDGET_EXPORT LinePainter : public Drawable
{
    public:
        LinePainter(const std::string &name = "LinePainter");
        LinePainter(const LinePainter &other);
        Component_DECL(LinePainter);
       
        .
        .
        .

    private:
        void drawComponent(sf::RenderTarget& target,
            sf::RenderStates states) const override;
        ...
};
}
}












```
</td>
</tr>
</table>

To speed up a painter you can use OpenGL directly to draw your stuff.
``` C++
void LinePainter::drawComponent(sf::RenderTarget& target,
                                sf::RenderStates states) const
{ 
    // Always load a transform first because SFML does not reset the transform after drawing shapse
    glLoadMatrixf(states.transform.getMatrix());

    // Iterate through the lines and draw each one using OpenGL
    for (auto& line : m_lines)
    {
        // Set the color for the line
        const sf::Color& color = line.color;

        // Draw the quad using OpenGL immediate mode
        glBegin(GL_QUADS);
        glColor4ub(color.r, color.g, color.b, color.a); // Set color with RGBA
        glVertex2f(line.points[0].x, line.points[0].y);
        glVertex2f(line.points[1].x, line.points[1].y);
        glVertex2f(line.points[2].x, line.points[2].y);
        glVertex2f(line.points[3].x, line.points[3].y);
        glEnd();
    }
}
```

### SfEventHandle
If your Component should catch sf::Events, use this base class to derive from.
Here is an example of a `MouseFollower` Component that inherits from the SfEventHandle
<table>
<tr>
<td> MouseFollower.cpp </td> <td> MouseFollower.h </td>
</tr>
<tr>
<td>

```c++
#include "Components/MouseFollower.h"
#include "objects/base/GameObject.h"
#include "objects/CameraWindow.h"

namespace QSFML
{
namespace Components
{
Component_IMPL(MouseFollower)
MouseFollower::MouseFollower(const std::string &name)
    : QObject()
    , SfEventHandle(name)
{

}
MouseFollower::MouseFollower(const MouseFollower &other)
    : QObject()
    , SfEventHandle(other)
{

}

void MouseFollower::sfEvent(const std::pair<Objects::CameraWindow*, std::vector<sf::Event>>& events)
{
    Objects::CameraWindow* cam = events.first;
    for (auto& event : events.second)
    {
        switch (event.type)
        {
            case sf::Event::MouseMoved:
            {
                sf::Vector2f worldPos = cam->getThisCameraMouseWorldPosition();
                sf::Vector2i pixelPos = cam->getThisCameraMousePosition();
    
                emit mousePosChanged(worldPos, pixelPos);
                return;
            }
            default:
                break;
        }
    }
}
}
}

```

</td>
<td>
    
```c++
#pragma once

#include "QSFML_EditorWidget_base.h"
#include "Components/base/SfEventHandle.h"

#include <QObject>

namespace QSFML
{
namespace Components
{
/**
 * @class MouseFollower
 * @brief The MouseFollower is a Component that will emit a signal when the mouse moves
 */
class QSFML_EDITOR_WIDGET_EXPORT MouseFollower : public QObject, public SfEventHandle
{
        Q_OBJECT
    public:
    MouseFollower(const std::string &name = "MouseFollower");
    MouseFollower(const MouseFollower &other);
    Component_DECL(MouseFollower);

    /**
     * @brief 
     * Gets called by the Scene to process the events for a specific camera.
     * @param events 
     */
    void sfEvent(const std::pair<Objects::CameraWindow*, std::vector<sf::Event>> &events) override;


    signals:

    /**
     * @brief
     * This signal is emitted when the mouse position changes
     * @param worldPos
     * @param pixelPos
     */
    void mousePosChanged(const sf::Vector2f &worldPos,
                         const sf::Vector2i &pixelPos);
};
}
}

```
</td>
</tr>
</table>




### AbstractPressEvent
The AbstractPressEvent Component can emit fallingEdge, down, risingEdge signals. This can be used to trigger for Key inputs, or custom buttons inside your Scene.
Just reimplement the `getCurrentValue()` function and check if it is pressed or not.
Edge detection will be done in the base class. 

<table>
<tr>
<td> KeyPressEvent.cpp </td> <td> KeyPressEvent.h </td>
</tr>
<tr>
<td>

```c++
#include "Components/KeyPressEvent.h"

namespace QSFML
{
namespace Components
{
Component_IMPL(KeyPressEvent)
KeyPressEvent::KeyPressEvent(const std::string &name,
                             sf::Keyboard::Key key)
    : AbstractPressEvent(name)
    , m_key(key)
{
 
}
KeyPressEvent::KeyPressEvent(const KeyPressEvent &other)
    : AbstractPressEvent(other)
    , m_key(other.m_key)
{

}
...
bool KeyPressEvent::getCurrentValue()
{
    return sf::Keyboard::isKeyPressed(m_key);
}
}
}













```

</td>
<td>
    
```c++
#pragma once

#include "QSFML_EditorWidget_base.h"
#include "Components/base/AbstractPressEvent.h"

#include <QObject>
#include <SFML/Graphics.hpp>

namespace QSFML
{
namespace Components
{
class QSFML_EDITOR_WIDGET_EXPORT KeyPressEvent : public AbstractPressEvent
{
        Q_OBJECT
    public:
        /**
         * \brief KeyPressEvent constructor
         * \param name, the name of this Component
         * \see Component::setName()
         */
        KeyPressEvent(const std::string &name = "KeyPressEvent",
                      const sf::Keyboard::Key key = sf::Keyboard::Key::Space);
        KeyPressEvent(const KeyPressEvent &other);
        Component_DECL(KeyPressEvent);

        void setTriggerKey(sf::Keyboard::Key key);
        sf::Keyboard::Key getTriggerKey() const;

    protected:
        // Reimplement this function which is used to trigger the fallingEdge, down, risingEdge
        // signals in the base class
        bool getCurrentValue() override;
    private:
        sf::Keyboard::Key m_key;
};
}
}


```
</td>
</tr>
</table>


### Updatable Component
If you need to update your Component, you have to derive your Component from `QSFML::Utilities::Updatable` to overwrite the `update()` function.

<table>
<tr>
<td> MyUpdatableComp.cpp </td> <td> MyUpdatableComp.h </td>
</tr>
<tr>
<td>

```c++
...
MyUpdatableComp::update()
{
    // Update stuff
}







```

</td>
<td>
    
```c++
class MyUpdatableComp : public QSFML::Components::Component, public QSFML::Utilities::Updatable
{
    public:
        MyUpdatableComp(const std::string &name = "MyUpdatableComp");
        MyUpdatableComp(const MyUpdatableComp &other);

    protected:
        /**
         * \brief update will be called once per frame
         */
        void update() final;
};
```
</td>
</tr>
</table>