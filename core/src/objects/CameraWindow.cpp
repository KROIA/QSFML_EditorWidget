#include "objects/CameraWindow.h"
#include <QHBoxLayout>
#include <QResizeEvent>
#include "scene/Scene.h"
#include "utilities/AABB.h"
#include "utilities/VectorOperations.h"

namespace QSFML
{
    namespace Objects
    {
        OBJECT_IMPL(CameraWindow);
        CameraWindow::CameraWindow(const sf::ContextSettings &settings,
                                   const std::string& name,
                                   QWidget* parent)
            : QWidget(parent)
            , GameObject(name)
            , m_window(nullptr)
            , m_settings(settings)
			, m_dpiScale(1.f, 1.f)
        {
            setup();
        }
        CameraWindow::CameraWindow(const CameraWindow &other)
            : QWidget()
            , GameObject(other)
            , m_window(nullptr)
            , m_settings(other.m_settings)
			, m_dpiScale(other.m_dpiScale)
        {
            setup();
        }
        void CameraWindow::setup()
        {
            // Setup layout of this widget
            if (parentWidget())
            {
                if (!parentWidget()->layout())
                {
                    QHBoxLayout* layout = new QHBoxLayout(parentWidget());
                    layout->setContentsMargins(0, 0, 0, 0);
                    parentWidget()->setLayout(layout);
                }
                parentWidget()->layout()->addWidget(this);
            }

            // Setup some states to allow direct rendering into the widget
            setAttribute(Qt::WA_PaintOnScreen);
            setAttribute(Qt::WA_OpaquePaintEvent);
            setAttribute(Qt::WA_NoSystemBackground);

            // Set strong focus to enable keyboard events to be received
            setFocusPolicy(Qt::StrongFocus);

            //m_updateTimer.onFinished(std::bind(&Scene::update, this));
            connect(&m_frameTimer, &QTimer::timeout, this, &CameraWindow::onFrame);
            m_frameTimer.setInterval(1000 / 30.f);
            //setSettings(settings);

            show();
        }
        CameraWindow::~CameraWindow()
        {
            m_frameTimer.stop();
            if (m_window)
            {
                m_window->close();
                delete m_window;
            }
        }

        void CameraWindow::setThisCameraView(const sf::View& view)
        {
            if (m_window)
            {
                m_window->setView(view);
                
                sf::Transform transform;
                if(getParent())
                    transform = getParent()->getGlobalTransform().getInverse();

                sf::Vector2f center = transform.transformPoint(view.getCenter());
                float rotation = std::atan2(transform.getMatrix()[1], transform.getMatrix()[0]) * 180 / M_PI;
                sf::Vector2f size = transform.transformPoint(view.getCenter() + view.getSize() / 2.f) - center;
                sf::Vector2f scale = VectorMath::getScale(transform);
                GameObject::setPosition(center);
                GameObject::setRotation(rotation);
                GameObject::setScale(size);
            }
        }
        const sf::View& CameraWindow::getThisCameraView() const
        {
            if(m_window)
				return m_window->getView();
			
			static sf::View dummy;
			return dummy;
        }
        const sf::View& CameraWindow::getThisCameraDefaultView() const
        {
			if (m_window)
				return m_window->getDefaultView();
			static sf::View dummy;
			return dummy;
        }
        Utilities::AABB CameraWindow::getThisCameraViewRect() const
        {
			if (!m_window)
				return Utilities::AABB();
			sf::View view = m_window->getView();
            return Utilities::AABB(view.getCenter() - view.getSize() * 0.5f, view.getSize());
        }

        sf::Vector2f CameraWindow::getThisCameraViewCenterPosition() const
        {
            if (!m_window) return sf::Vector2f(0, 0);
            return m_window->getView().getCenter();
        }


        sf::Vector2i CameraWindow::getThisCameraMousePosition() const
        {
            if (m_window)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(*m_window);
                mousePos.x /= m_dpiScale.x;
                mousePos.y /= m_dpiScale.y;
                return mousePos;
            }
            sf::Vector2i mousePos = sf::Mouse::getPosition();
            mousePos.x /= m_dpiScale.x;
            mousePos.y /= m_dpiScale.y;
            return mousePos;
        }
        sf::Vector2f CameraWindow::getThisCameraMouseWorldPosition() const
        {
            if (!m_window)
                return sf::Vector2f(0, 0);
            sf::Vector2i mousePos = sf::Mouse::getPosition(*m_window);
            return m_window->mapPixelToCoords(mousePos);
        }
        sf::Vector2f CameraWindow::getInThisCameraWorldSpace(sf::Vector2i pixelSpace) const
        {
            if (!m_window) 
                return sf::Vector2f(0, 0);
            pixelSpace.x *= m_dpiScale.x;
            pixelSpace.y *= m_dpiScale.y;
            return m_window->mapPixelToCoords(pixelSpace);
        }
        sf::Vector2i CameraWindow::getInThisCameraScreenSpace(const sf::Vector2f& worldSpace) const
        {
            sf::Vector2i pos(0, 0);
            if (!m_window) return pos;

            pos = m_window->mapCoordsToPixel(worldSpace);
            pos.x /= m_dpiScale.x;
            pos.y /= m_dpiScale.y;
            return pos;
        }

        sf::Image CameraWindow::captureThisCameraScreen() const
        {
            if (!m_window)
                return sf::Image();

            sf::Vector2u windowSize = m_window->getSize();
            sf::Texture texture;
            texture.create(windowSize.x, windowSize.y);
            texture.update(*m_window);
            sf::Image screenshot = texture.copyToImage();

            return screenshot;
        }
        void CameraWindow::enableFrameTimer(bool enable)
        {
			m_enableFrameTimer = enable;
            if (enable)
                m_frameTimer.start();
            else
                m_frameTimer.stop();
        }



        sf::Vector2u CameraWindow::getThisCameraSize() const
        {
			if (m_window)
				return m_window->getSize();
			static sf::Vector2u dummy;
			return dummy;
        }


        /*
        void CameraWindow::setPosition(float x, float y)
        {
            GameObject::setPosition(x, y);
            updateView();
        }
        void CameraWindow::setPosition(const sf::Vector2f& position)
        {
            GameObject::setPosition(position);
            updateView();
        }
        void CameraWindow::setRotation(float angle)
        {
            GameObject::setRotation(angle);
            updateView();
        }
        void CameraWindow::setScale(float factorX, float factorY)
        {
            GameObject::setScale(factorX, factorY);
            updateView();
        }
        void CameraWindow::setScale(const sf::Vector2f& factors)
        {
            GameObject::setScale(factors);
            updateView();
        }
        void CameraWindow::setOrigin(float x, float y)
        {
            GameObject::setOrigin(x, y);
            updateView();
        }
        void CameraWindow::setOrigin(const sf::Vector2f& origin)
        {
            GameObject::setOrigin(origin);
            updateView();
        }
        void CameraWindow::move(float offsetX, float offsetY)
        {
            GameObject::move(offsetX, offsetY);
            updateView();
        }
        void CameraWindow::move(const sf::Vector2f& offset)
        {
            GameObject::move(offset);
            updateView();
        }
        void CameraWindow::rotate(float angle)
        {
            GameObject::rotate(angle);
            updateView();
        }
        void CameraWindow::scale(float factorX, float factorY)
        {
            GameObject::scale(factorX, factorY);
            updateView();
        }
        void CameraWindow::scale(const sf::Vector2f& factor)
        {
            GameObject::scale(factor);
            updateView();
        }*/

        void CameraWindow::onFrame()
        {
            if (!getSceneParent() || !m_window)
                return;
            getSceneParent()->paint(*m_window);
        }
        void CameraWindow::pollEvents()
        {
			size_t lastSize = m_events.size();
            m_events.clear();
			if (!m_window) return;
            m_events.reserve(lastSize+5);
            sf::Event event;
            bool wasClosed = false;
            while (m_window->pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    wasClosed = true;
					
                }
                m_events.push_back(event);
            }

            if (wasClosed)
            {
                m_frameTimer.stop();
                close();
            }
        }

        void CameraWindow::update()
        {
            if (!m_window) return;
            sf::Transform transform = getGlobalTransform();
            if(m_lastTransform == transform)
				return;
            m_lastTransform = transform;
			sf::View view = m_window->getView();
            
            view.setCenter(transform.transformPoint(sf::Vector2f(0,0)));
            float rotation = std::atan2(transform.getMatrix()[1], transform.getMatrix()[0]) * 180 / M_PI;
            view.setRotation(rotation);
            //view.setSize(sf::Vector2f(
            //    std::sqrt(transform.getMatrix()[0] * transform.getMatrix()[0] + transform.getMatrix()[1] * transform.getMatrix()[1]),
            //    std::sqrt(transform.getMatrix()[4] * transform.getMatrix()[4] + transform.getMatrix()[5] * transform.getMatrix()[5])));
            //view.setSize(transform.transformRect(sf::FloatRect(sf::Vector2f(0, 0), getScale())).getSize());
            

            m_window->setView(view);
        }
        //void CameraWindow::updateView()
        //{
            //if (!m_window) return;
			//sf::View view = m_window->getView();
			//view.setCenter(getPosition());
			//view.setRotation(getRotation());
			//view.setSize(getScale());
			////view.setOrigin(getOrigin());
			//m_window->setView(view);
        //}


        QPaintEngine* CameraWindow::paintEngine() const
        {
            return nullptr;
        }
        void CameraWindow::showEvent(QShowEvent*)
        {
            if (!m_window)
            {
                // Under X11, we need to flush the commands sent to the server to ensure that
                // SFML will get an updated view of the windows
#ifdef Q_WS_X11
                XFlush(QX11Info::display());
#endif

                // Create the SFML window with the widget handle
                m_window = new sf::RenderWindow((sf::WindowHandle)QWidget::winId(), m_settings);
                m_window->setFramerateLimit(0);
                //m_window->setVerticalSyncEnabled(false);
                //m_view = m_window->getView();

               
                //m_updateTimer.autoRestart(true);
                //m_updateTimer.start();


               // m_syncedUpdateT_t1 = std::chrono::high_resolution_clock::now();
               // m_update_t1 = m_syncedUpdateT_t1;
               // m_paint_t1 = m_syncedUpdateT_t1;
                m_oldViewSize = m_window->getSize();

                // Calculate the dpi scale
                QRect geometry = QWidget::geometry();
                m_dpiScale.x = (float)m_oldViewSize.x / geometry.width();
                m_dpiScale.y = (float)m_oldViewSize.y / geometry.height();


                //sf::View view = m_window->getView();
                //GameObject::setPosition(view.getCenter());
                //GameObject::setRotation(view.getRotation());
                //GameObject::setScale(view.getSize());
            }

            // Setup the timer to trigger a refresh at specified framerate
            if(m_enableFrameTimer)
                m_frameTimer.start();
        }
        void CameraWindow::closeEvent(QCloseEvent*)
        {
            m_frameTimer.stop();
            if (m_window)
            {
                m_window->close();
                delete m_window;
                m_window = nullptr;
            }
        }
        void CameraWindow::hideEvent(QHideEvent*)
        {
			m_frameTimer.stop();
            if (m_window)
            {
                m_window->close();
                delete m_window;
                m_window = nullptr;
            }
        }


        void CameraWindow::resizeEvent(QResizeEvent* event)
        {
            if (!m_window) return;

            QSize size = event->size();
            QSize oldSize = event->oldSize();
            m_oldViewSize.x = oldSize.width();
            m_oldViewSize.y = oldSize.height();

            sf::View oldView = getThisCameraView();
			sf::View view = getThisCameraDefaultView();


            // resize the view such that it shows the same area as before by respecting the aspect ratio
            float aspectRatio = (float)size.height() / (float)size.width();
            float newWidth = oldView.getSize().x;
            float newHeight = newWidth * aspectRatio;

            //view.setSize(newWidth, newHeight);
            view.setSize(newWidth, newHeight);
            view.setCenter(oldView.getCenter());

            // Update the view
            setThisCameraView(view);
            
        }
    }
}