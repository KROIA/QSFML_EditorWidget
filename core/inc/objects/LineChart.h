#pragma once

#include <vector>
#include "objects/base/GameObject.h"
#include "components/base/Drawable.h"
#include "QSFML_EditorWidget_base.h"


namespace QSFML
{
namespace Objects
{
    class QSFML_EDITOR_WIDGET_EXPORT LineChart: public GameObject
    {
            class LineChartPainter;
            friend LineChartPainter;
        public:
            LineChart(const std::string &name = "LineChart",
                         GameObjectPtr parent = nullptr);
            LineChart(const LineChart &other);
            ~LineChart();
            OBJECT_DECL(LineChart)

            //void setOrigin(const sf::Vector2f &pos);
            //const sf::Vector2f &getOrigin() const;

            void setSize(const sf::Vector2f &size);
            const sf::Vector2f &getSize() const;

            void setColor(const sf::Color &color);
            const sf::Color &getColor() const;

			void setChatThickness(float thickness) { m_chatThickness = thickness; }
			float getChatThickness() const { return m_chatThickness; }

            void setMaxDataPoints(size_t maxDataPoints);
            size_t getMaxDataPoints() const;

            void enableAutoScale(bool enable);
            bool autoScaleEnabled() const;
            void setPlotScale(float yScale);
            float getPlotScale() const;

            void addDataPoint(float value);
            void removeFirstDataPoint();
            void setDataPoints(const QSFML::vector<float> &dataPoints);
            const QSFML::vector<float> &getDataPoints() const;
            void clear();

            void update() override;

        private:
            void getMinMax(float &min, float &max);

            QSFML::vector<float> m_dataPoints;
			QSFML::vector<sf::Vector2f> m_points;
            
           // sf::Vector2f m_origin;
            sf::Vector2f m_size;
            sf::Color m_color;
            float m_yScale;
            bool m_autoScale;
            size_t m_maxDataPoints;
			float m_chatThickness = 2;

            Components::Text* m_valueText = nullptr;

            LineChartPainter* m_painter;


            class LineChartPainter: public Components::Drawable
            {
                public:
                    LineChartPainter(const std::string &name = "LineChartPainter");
                    void drawComponent(sf::RenderTarget& target, sf::RenderStates states) const override;

                    LineChart *m_chart;
            };
    };
}
}
