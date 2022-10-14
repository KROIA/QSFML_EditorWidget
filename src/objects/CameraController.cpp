#include "CameraController.h"
#include "Canvas.h"
#include <QDebug>

using namespace QSFML::Objects;

CameraController::CameraController(const std::string &name,
                                   CanvasObject *parent)
    : CanvasObject(name,parent)
{
   // getCanvasParent() = nullptr;
    m_currentZoom = 1;
    setMinZoom(0.1);
    setMaxZoom(3);
    setMaxMovingBounds(sf::FloatRect(-200,-200,900,900));

    m_eventHandleComponent = new SfEventComponent();
    m_eventHandleComponent->setController(this);
    addComponent(m_eventHandleComponent);
}
CameraController::~CameraController()
{}
void CameraController::setMinZoom(float min)
{
    m_minZoom = min;
}

void CameraController::setMaxZoom(float max)
{
    m_maxZoom = max;
}

float CameraController::getMinZoom()const
{
    return m_minZoom;
}

float CameraController::getMaxZoom()const
{
    return m_maxZoom;
}

void CameraController::setMaxMovingBounds(const sf::FloatRect &rect)
{
    m_maxMovingBounds = rect;
}
const sf::FloatRect &CameraController::getMaxMovingBounds() const
{
    return m_maxMovingBounds;
}

void CameraController::moveLeft(float amount)
{
    movePosition(sf::Vector2f(-amount,0));
}
void CameraController::moveRight(float amount)
{
    movePosition(sf::Vector2f(amount,0));
}
void CameraController::moveUp(float amount)
{
    movePosition(sf::Vector2f(0,-amount));
}
void CameraController::moveDown(float amount)
{
    movePosition(sf::Vector2f(0,amount));
}
void CameraController::movePosition(const sf::Vector2f &delta)
{
    if(!getCanvasParent()) return;
    sf::View view = getCanvasParent()->getCameraView();
    view.move(delta);
    positionCheck(view);
    getCanvasParent()->setCameraView(view);
}
void CameraController::setPosition(const sf::Vector2f &pos)
{
    if(!getCanvasParent()) return;
    sf::View view = getCanvasParent()->getCameraView();
    view.setCenter(pos);
    positionCheck(view);
    getCanvasParent()->setCameraView(view);
}
void CameraController::rotate(float angle)
{
    if(!getCanvasParent()) return;
    sf::View view = getCanvasParent()->getCameraView();
    view.rotate(angle);
    getCanvasParent()->setCameraView(view);
}
void CameraController::setRotation(float angle)
{
    if(!getCanvasParent()) return;
    sf::View view = getCanvasParent()->getCameraView();
    view.setRotation(angle);
    getCanvasParent()->setCameraView(view);
}
void CameraController::zoom(float amount)
{
    if(!getCanvasParent()) return;
    float newZoom = m_currentZoom * amount;
    if(newZoom < m_minZoom || newZoom > m_maxZoom)
        return;
    m_currentZoom = newZoom;

    sf::View view = getCanvasParent()->getCameraView();
    view.zoom(amount);
    positionCheck(view);
    getCanvasParent()->setCameraView(view);
}
void CameraController::zoom(float amount, const sf::Vector2i &pixel)
{
    if(!getCanvasParent()) return;
    float newZoom = m_currentZoom * amount;
    if(newZoom < m_minZoom || newZoom > m_maxZoom)
        return;
    sf::View view = getCanvasParent()->getCameraView();
    m_currentZoom = newZoom;

    const sf::Vector2f beforeCoord = getInWorldSpace(pixel);

    view.zoom(amount);
    positionCheck(view);
    getCanvasParent()->setCameraView(view);
    const sf::Vector2f afterCoord{ getInWorldSpace(pixel) };
    const sf::Vector2f offsetCoords{ beforeCoord - afterCoord };
    view.move(offsetCoords);
    positionCheck(view);
    getCanvasParent()->setCameraView(view);
}
void CameraController::setZoom(float amount)
{
    if(!getCanvasParent()) return;
    sf::View view = getCanvasParent()->getCameraView();
    sf::View defaultView = getCanvasParent()->getDefaultCameraView();

    defaultView.setCenter(view.getCenter());
    getCanvasParent()->setCameraView(defaultView);
    zoom(amount);
}
void CameraController::setZoom(float amount, const sf::Vector2i &pixel)
{
    if(!getCanvasParent()) return;
    sf::View view = getCanvasParent()->getCameraView();
    sf::View defaultView = getCanvasParent()->getDefaultCameraView();

    defaultView.setCenter(view.getCenter());
    getCanvasParent()->setCameraView(defaultView);
    zoom(amount,pixel);
}
void CameraController::setCameraView(const sf::View &view)
{
    if(!getCanvasParent()) return;
    getCanvasParent()->setCameraView(view);
}
void CameraController::update()
{

}

void CameraController::SfEventComponent::setController(CameraController *controller)
{
    m_controller = controller;
}
void CameraController::SfEventComponent::sfEvent(const sf::Event &e)
{
    if(!m_controller) return;

    static bool mousePressed = false;
    static sf::Vector2f startPos;
    switch(e.type)
    {
        case sf::Event::MouseWheelScrolled:
        {
            float zoomAmount = 1.1;
            if (e.mouseWheelScroll.delta > 0)
                m_controller->zoom( 1/zoomAmount , { e.mouseWheelScroll.x, e.mouseWheelScroll.y });
            else if (e.mouseWheelScroll.delta < 0)
                m_controller->zoom(zoomAmount , { e.mouseWheelScroll.x, e.mouseWheelScroll.y });
            break;
        }
        case sf::Event::MouseButtonPressed:
        {
            if(e.mouseButton.button == sf::Mouse::Button::Left)
            {
                startPos = sf::Vector2f(m_controller->getMousePosition());
                mousePressed = true;
            }
            break;
        }
        case sf::Event::MouseButtonReleased:
        {
            if(e.mouseButton.button == sf::Mouse::Button::Left)
                mousePressed = false;
            break;
        }
        case sf::Event::MouseMoved:
        {
           /* if(mousePressed)
            {
                movePosition((getMouseWorldPosition() - startPos)*0.001f);
            }*/
            // Ignore mouse movement unless a button is pressed (see above)
            if (!mousePressed)
                break;
            // Determine the new position in world coordinates
            sf::Vector2f newPos = sf::Vector2f(e.mouseMove.x, e.mouseMove.y);
            // Determine how the cursor has moved
            // Swap these to invert the movement direction

            //this->mapPixelToCoords(sf::Vector2i(m_movingOldPos*100.f - newPos*100.f))/100.f;
            sf::Vector2f deltaPos = (m_controller->getInWorldSpace(sf::Vector2i(startPos*100.f))-
                                     m_controller->getInWorldSpace(sf::Vector2i(newPos*100.f)))/100.f;

            m_controller->movePosition(deltaPos);
            // Move our view accordingly and update the window
            //sf::View view = this->getView();
            //view.move(deltaPos); // <-- Here I use move
            //this->setView(view);

            // Save the new position as the old one
            // We're recalculating this, since we've changed the view
            startPos = newPos; // With move, I don't need to recalculate
            break;
        }
        case sf::Event::Resized:
        {
            sf::View view = m_controller->getCameraView();

            sf::Vector2u oldWindowSize = m_controller->getOldCanvasSize();
            sf::Vector2u newWindowSize = m_controller->getCanvasSize();
            sf::FloatRect viewRect = sf::FloatRect(view.getCenter()-view.getSize()/2.f,view.getSize());

            viewRect.width = viewRect.width/oldWindowSize.x * newWindowSize.x;
            viewRect.height = viewRect.height/oldWindowSize.y * newWindowSize.y;

            view.setSize(viewRect.width,viewRect.height);
            view.setCenter(viewRect.left+viewRect.width/2.f,viewRect.top+viewRect.height/2.f);
            m_controller->positionCheck(view);
            m_controller->setCameraView(view);
            break;
        }
    }

}

void CameraController::internalOnCanvasParentChange(Canvas *newParent)
{

}
void CameraController::internalOnParentChange(CanvasObject *newParent)
{

}

void CameraController::positionCheck(sf::View &view)
{
    //return;
    sf::FloatRect viewRect = sf::FloatRect(view.getCenter()-view.getSize()/2.f,view.getSize());
    sf::Vector2f cameraPos = view.getCenter();
    sf::Vector2u windowSize = getCanvasSize();
    float aspectRatio = (float)windowSize.x/(float)windowSize.y;

    //qDebug() << "1ViewRect= "<<viewRect.left << " " << viewRect.top << " w*h="<<viewRect.width<<" "<<viewRect.height;


    // Fixes the gridsize
    /*if(m_maxMovingBounds.width < viewRect.width)
        viewRect.width = m_maxMovingBounds.width;
    if(m_maxMovingBounds.height < viewRect.height)
        viewRect.height = m_maxMovingBounds.height;*/

    if(viewRect.width / viewRect.height > aspectRatio)
    {
        viewRect.width = viewRect.height * aspectRatio;
    }
    else
    {
        viewRect.height = viewRect.width/aspectRatio;
    }
/*
    if(m_maxMovingBounds.left+m_maxMovingBounds.width < viewRect.left+viewRect.width)
        viewRect.left = m_maxMovingBounds.left+m_maxMovingBounds.width-viewRect.width;
    if(m_maxMovingBounds.top+m_maxMovingBounds.height < viewRect.top+viewRect.height)
        viewRect.top = m_maxMovingBounds.top+m_maxMovingBounds.height-viewRect.height;


    if(m_maxMovingBounds.left > viewRect.left)
        viewRect.left = m_maxMovingBounds.left;
    if(m_maxMovingBounds.top > viewRect.top)
        viewRect.top = m_maxMovingBounds.top;
*/
    if(cameraPos.x < m_maxMovingBounds.left)
        cameraPos.x = m_maxMovingBounds.left;
    else if(cameraPos.x > m_maxMovingBounds.left+m_maxMovingBounds.width)
        cameraPos.x = m_maxMovingBounds.left+m_maxMovingBounds.width;

    if(cameraPos.y < m_maxMovingBounds.top)
        cameraPos.y = m_maxMovingBounds.top;
    else if(cameraPos.y > m_maxMovingBounds.top+m_maxMovingBounds.height)
        cameraPos.y = m_maxMovingBounds.top+m_maxMovingBounds.height;



 //   qDebug() << "2ViewRect= "<<viewRect.left << " " << viewRect.top << " w*h="<<viewRect.width<<" "<<viewRect.height;
    view.setSize(viewRect.width,viewRect.height);
    view.setCenter(cameraPos);
    //view.setCenter(viewRect.left+viewRect.width/2.f,viewRect.top+viewRect.height/2.f);
}
/*bool CameraController::isOutsideBounds(sf::View &view)
{
    sf::FloatRect viewRect = sf::FloatRect(view.getCenter()-view.getSize()/2.f,view.getSize());

    if(m_maxMovingBounds.left+m_maxMovingBounds.width < viewRect.left+viewRect.width)
        return true;
    if(m_maxMovingBounds.top+m_maxMovingBounds.height < viewRect.top+viewRect.height)
        return true;

    if(m_maxMovingBounds.left > viewRect.left)
        return true;
    if(m_maxMovingBounds.top > viewRect.top)
        return true;
    return false;
}*/
