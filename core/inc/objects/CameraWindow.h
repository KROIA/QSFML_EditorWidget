#pragma once

#include "QSFML_EditorWidget_base.h"
#include "objects/base/GameObject.h"
#include <QWidget>
#include <QTimer>
#include <SFML/Graphics.hpp>
#include <string>

namespace QSFML
{
	namespace Objects
	{
		class QSFML_EDITOR_WIDGET_EXPORT CameraWindow : public QWidget, public GameObject
		{
			Q_OBJECT
				void setup();
			friend class Scene;
			public:
			CameraWindow(const std::string& name = "CameraWindow",
						 QWidget* parent = nullptr);
			CameraWindow(const CameraWindow& other);
			~CameraWindow();

			OBJECT_DECL(CameraWindow);

			
			void setThisCameraView(const sf::View& view);
			const sf::View& getThisCameraView() const;
			const sf::View& getThisCameraDefaultView() const;
			Utilities::AABB getThisCameraViewRect() const;
			sf::Vector2f getThisCameraViewCenterPosition() const;
			sf::Vector2i getThisCameraMousePosition() const;
			sf::Vector2f getThisCameraMouseWorldPosition() const;
			sf::Vector2f getInThisCameraWorldSpace(sf::Vector2i pixelSpace) const;
			sf::Vector2i getInThisCameraScreenSpace(const sf::Vector2f& worldSpace) const;
			sf::Image captureThisCameraScreen() const;

			sf::Vector2u getThisCameraSize() const;
			const sf::Vector2u &getThisCameraOldSize() const { return m_oldViewSize; }
			const sf::Vector2f &getThisCameraDpiScale() const { return m_dpiScale; }

			void enableFrameTimer(bool enable);
			private slots:
			void onFrame();

			void pollEvents();
			const std::vector<sf::Event>& getThisCameraEvents() const { return m_events; }
			
			
			protected:
			QPaintEngine* paintEngine() const override;
			void showEvent(QShowEvent*) override;
			void closeEvent(QCloseEvent*) override;
			void hideEvent(QHideEvent*) override;

			void resizeEvent(QResizeEvent* event) override;

			sf::RenderWindow* getRenderWindow() { return m_window; }
			private:


			sf::RenderWindow* m_window = nullptr;
			//sf::View m_view;
			sf::Vector2f m_dpiScale;
			sf::Vector2u m_oldViewSize;
			QTimer m_frameTimer;
			bool m_enableFrameTimer = true;
			std::vector<sf::Event> m_events;

			//Scene* m_scene = nullptr;
		};
	}
}