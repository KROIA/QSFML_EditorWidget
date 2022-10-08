#include "CameraController.h"
#include "QSFML_Canvas.h"
#include <QDebug>

CameraController::CameraController(CanvasObject *parent)
    :   CanvasObject(parent)
{
   // m_canvasParent = nullptr;
    m_currentZoom = 1;
    setMinZoom(0.1);
    setMaxZoom(10);
    setMaxMovingBounds(sf::FloatRect(-200,-200,900,900));
}
CameraController::~CameraController()
{

}
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
    if(!m_canvasParent) return;
    sf::View view = m_canvasParent->getCameraView();
    view.move(delta);
    positionCheck(view);
    m_canvasParent->setCameraView(view);
}
void CameraController::setPosition(const sf::Vector2f &pos)
{
    if(!m_canvasParent) return;
    sf::View view = m_canvasParent->getCameraView();
    view.setCenter(pos);
    positionCheck(view);
    m_canvasParent->setCameraView(view);
}
void CameraController::rotate(float angle)
{
    if(!m_canvasParent) return;
    sf::View view = m_canvasParent->getCameraView();
    view.rotate(angle);
    m_canvasParent->setCameraView(view);
}
void CameraController::setRotation(float angle)
{
    if(!m_canvasParent) return;
    sf::View view = m_canvasParent->getCameraView();
    view.setRotation(angle);
    m_canvasParent->setCameraView(view);
}
void CameraController::zoom(float amount)
{
    if(!m_canvasParent) return;
    float newZoom = m_currentZoom * amount;
    if(newZoom < m_minZoom || newZoom > m_maxZoom)
        return;
    m_currentZoom = newZoom;

    sf::View view = m_canvasParent->getCameraView();
    view.zoom(amount);
    positionCheck(view);
    m_canvasParent->setCameraView(view);
}
void CameraController::zoom(float amount, const sf::Vector2i &pixel)
{
    if(!m_canvasParent) return;
    float newZoom = m_currentZoom * amount;
    if(newZoom < m_minZoom || newZoom > m_maxZoom)
        return;
    m_currentZoom = newZoom;

    const sf::Vector2f beforeCoord{ getInWorldSpace(pixel) };
    sf::View view{ m_canvasParent->getCameraView() };
    view.zoom(amount);
    positionCheck(view);
    m_canvasParent->setCameraView(view);
    const sf::Vector2f afterCoord{ getInWorldSpace(pixel) };
    const sf::Vector2f offsetCoords{ beforeCoord - afterCoord };
    view.move(offsetCoords);
    positionCheck(view);
    m_canvasParent->setCameraView(view);
}
void CameraController::setZoom(float amount)
{
    if(!m_canvasParent) return;
    sf::View view = m_canvasParent->getCameraView();
    sf::View defaultView = m_canvasParent->getDefaultCameraView();

    defaultView.setCenter(view.getCenter());
    m_canvasParent->setCameraView(defaultView);
    zoom(amount);
}
void CameraController::setZoom(float amount, const sf::Vector2i &pixel)
{
    if(!m_canvasParent) return;
    sf::View view = m_canvasParent->getCameraView();
    sf::View defaultView = m_canvasParent->getDefaultCameraView();

    defaultView.setCenter(view.getCenter());
    m_canvasParent->setCameraView(defaultView);
    zoom(amount,pixel);
}
void CameraController::setView(const sf::View &view)
{
    if(!m_canvasParent) return;
    m_canvasParent->setCameraView(view);
}
const sf::View CameraController::getView()
{
    static const sf::View dummy;
    if(!m_canvasParent) return dummy;
    return m_canvasParent->getCameraView();
}

void CameraController::sfEvent(const sf::Event &e)
{
    if(!m_canvasParent) return;

    static bool mousePressed = false;
    static sf::Vector2f startPos;
    switch(e.type)
    {
        case sf::Event::MouseWheelScrolled:
        {
            //zoom(1+0.01f*(float)e.mouseWheelScroll.delta);
            //zoom(1+0.01f*(float)e.mouseWheelScroll.delta,getMousePosition());
            float zoomAmount = 1.1;
            if (e.mouseWheelScroll.delta > 0)
                zoom( 1/zoomAmount , { e.mouseWheelScroll.x, e.mouseWheelScroll.y });
            else if (e.mouseWheelScroll.delta < 0)
                zoom(zoomAmount , { e.mouseWheelScroll.x, e.mouseWheelScroll.y });
            break;
        }
        case sf::Event::MouseButtonPressed:
        {
            if(e.mouseButton.button == sf::Mouse::Button::Left)
            {
                startPos = sf::Vector2f(getMousePosition());
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
            sf::Vector2f deltaPos = (getInWorldSpace(sf::Vector2i(startPos*100.f))-
                                     getInWorldSpace(sf::Vector2i(newPos*100.f)))/100.f;

            movePosition(deltaPos);
            // Move our view accordingly and update the window
            //sf::View view = this->getView();
            //view.move(deltaPos); // <-- Here I use move
            //this->setView(view);

            // Save the new position as the old one
            // We're recalculating this, since we've changed the view
            startPos = newPos; // With move, I don't need to recalculate
            break;
        }
    }

}

void CameraController::internalOnCanvasParentChange(QSFML_Canvas *newParent)
{

}
void CameraController::internalOnParentChange(CanvasObject *newParent)
{

}

void CameraController::positionCheck(sf::View &view)
{
    sf::FloatRect viewRect = sf::FloatRect(view.getCenter()-view.getSize()/2.f,view.getSize());
//qDebug() << "1ViewRect= "<<viewRect.left << " " << viewRect.top << " w*h="<<viewRect.width<<" "<<viewRect.height;

    if(m_maxMovingBounds.width < viewRect.width)
        viewRect.width = m_maxMovingBounds.width;
    if(m_maxMovingBounds.height < viewRect.height)
        viewRect.height = m_maxMovingBounds.height;

    if(m_maxMovingBounds.left > viewRect.left)
        viewRect.left = m_maxMovingBounds.left;
    if(m_maxMovingBounds.top > viewRect.top)
        viewRect.top = m_maxMovingBounds.top;

    if(m_maxMovingBounds.left+m_maxMovingBounds.width < viewRect.left+viewRect.width)
        viewRect.left = m_maxMovingBounds.left+m_maxMovingBounds.width-viewRect.width;
    if(m_maxMovingBounds.top+m_maxMovingBounds.height < viewRect.top+viewRect.height)
        viewRect.top = m_maxMovingBounds.top+m_maxMovingBounds.height-viewRect.height;



 //   qDebug() << "2ViewRect= "<<viewRect.left << " " << viewRect.top << " w*h="<<viewRect.width<<" "<<viewRect.height;
    view.setSize(viewRect.width,viewRect.height);
    view.setCenter(viewRect.left+viewRect.width/2.f,viewRect.top+viewRect.height/2.f);
}
