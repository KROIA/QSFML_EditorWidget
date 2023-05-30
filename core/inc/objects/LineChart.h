#pragma once

#include <vector>
#include "objects/base/CanvasObject.h"
#include "components/base/Drawable.h"
#include "QSFML_base.h"


namespace QSFML
{
namespace Objects
{
    class QSFML_EDITOR_WIDGET_EXPORT LineChart: public CanvasObject
    {
            class LineChartPainter;
            friend LineChartPainter;
        public:
            LineChart(const std::string &name = "LineChart",
                         CanvasObject *parent = nullptr);
            LineChart(const LineChart &other);
            ~LineChart();
            OBJECT_DECL(LineChart)

            void setOrigin(const sf::Vector2f &pos);
            const sf::Vector2f &getOrigin() const;

            void setSize(const sf::Vector2f &size);
            const sf::Vector2f &getSize() const;

            void setColor(const sf::Color &color);
            const sf::Color &getColor() const;

            void enableAutoScale(bool enable);
            bool autoScaleEnabled() const;
            void setScale(float yScale);
            float getScale() const;

            void addDataPoint(float value);
            void removeFirstDataPoint();
            void setDataPoints(const std::vector<float> &dataPoints);
            const std::vector<float> &getDataPoints() const;
            void clear();

            void update() override;

        private:
            void getMinMax(float &min, float &max);

            std::vector<float> m_dataPoints;
            
            sf::Vector2f m_origin;
            sf::Vector2f m_size;
            sf::Color m_color;
            float m_yScale;
            bool m_autoScale;

            LineChartPainter *m_painter;

            class LineChartPainter: public Components::Drawable
            {
                public:
                    LineChartPainter(const std::string &name = "LineChartPainter");
                    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

                    LineChart *m_chart;
            };
    };
}
}
