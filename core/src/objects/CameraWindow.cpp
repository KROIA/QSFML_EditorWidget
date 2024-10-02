#include "objects/CameraWindow.h"
#include <QHBoxLayout>
#include <QResizeEvent>
#include "scene/Scene.h"
#include "utilities/AABB.h"

namespace QSFML
{
    namespace Objects
    {
        OBJECT_IMPL(CameraWindow);
        CameraWindow::CameraWindow(const std::string& name,
                                   QWidget* parent)
            : QWidget(parent)
            , GameObject(name)
            , m_window(nullptr)
			, m_dpiScale(1.f, 1.f)
        {
            setup();
        }
        CameraWindow::CameraWindow(const CameraWindow &other)
            : QWidget()
            , GameObject(other)
            , m_window(nullptr)
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
            m_window->close();
            delete m_window;
        }

        void CameraWindow::setThisCameraView(const sf::View& view)
        {
			if(m_window)
				m_window->setView(view);
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
        void CameraWindow::onFrame()
        {
            if (!getSceneParent() || !m_window)
                return;
            getSceneParent()->paint(*m_window);
        }
        void CameraWindow::pollEvents()
        {
            m_events.clear();
			if (!m_window) return;
            m_events.reserve(20);
            sf::Event event;
            while (m_window->pollEvent(event))
            {
                m_events.push_back(event);
            }
        }
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
                sf::ContextSettings settings;
                m_window = new sf::RenderWindow((sf::WindowHandle)QWidget::winId(), settings);

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
            }

            // Setup the timer to trigger a refresh at specified framerate
            if(m_enableFrameTimer)
                m_frameTimer.start();
        }
        void CameraWindow::closeEvent(QCloseEvent*)
        {
            m_frameTimer.stop();
        }
        void CameraWindow::hideEvent(QHideEvent*)
        {
			m_frameTimer.stop();
        }


        void CameraWindow::resizeEvent(QResizeEvent* event)
        {
            if (!m_window) return;

            QSize size = event->size();


			sf::View view = getThisCameraView();

            sf::Vector2u oldWindowSize = m_oldViewSize;
            sf::Vector2u newWindowSize = m_window->getSize();
            sf::FloatRect viewRect = sf::FloatRect(view.getCenter() - view.getSize() / 2.f, view.getSize());

            viewRect.width = viewRect.width / oldWindowSize.x * newWindowSize.x;
            viewRect.height = viewRect.height / oldWindowSize.y * newWindowSize.y;

            view.setSize(viewRect.width, viewRect.height);
            view.setCenter(viewRect.left + viewRect.width / 2.f, viewRect.top + viewRect.height / 2.f);

            float aspectRatio = (float)newWindowSize.x / (float)newWindowSize.y;

            if (viewRect.width / viewRect.height > aspectRatio)
            {
                viewRect.width = viewRect.height * aspectRatio;
            }
            else
            {
                viewRect.height = viewRect.width / aspectRatio;
            }
            view.setSize(viewRect.width, viewRect.height);
            //view.rotate(45);

            setThisCameraView(view);
            m_oldViewSize = m_window->getSize();
        }
    }
}