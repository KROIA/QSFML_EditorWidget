#pragma once

#include "QSFML_EditorWidget.h"

class Gradient : public QObject, public QSFML::Objects::CanvasObject
{
	Q_OBJECT
public:
	Gradient(const std::string& name = "Gradient",
		CanvasObject* parent = nullptr);

	void setStart(const sf::Vector2f& start);
	void update() override;

private slots:

private:
	sf::Vector2f decend(sf::Vector2f p, float amount);
	sf::Vector2f newtenDecend(sf::Vector2f p);
	void drawPixel(sf::Vector2u point, double value);
	sf::Color getColor(double value);
	sf::Vector2u getPixelCoord(sf::Vector2f point);
    sf::Vector2f getWorldCoord(sf::Vector2f point);

	sf::Vector2f m_start;
	sf::Vector2f m_currentPos;

	QSFML::Components::PixelPainter *m_pixelPainter;

    class Lines : public QSFML::Components::Drawable
    {
    public:
        Lines(const std::string& name = "Lines");
        Lines(const Lines& other);
        COMPONENT_DECL(Lines);

        void addPoint(const sf::Vector2f& pos);
        void removePoint(size_t index = 0);
        void clear();

        //void setThickness(float thickness);
        //float getThickness() const;

        void setColor(const sf::Color& color);
        const sf::Color& getColor() const;

        void drawComponent(sf::RenderTarget& target,
            sf::RenderStates states) const override;

    private:
        //void updateShape();
        void updateColor();

        std::vector<sf::Vertex> m_vertices;
        //float m_thickness;
        sf::Color m_color;
    };

    struct Instance
    {
        sf::Vector2f pos;
        Lines* line;
       // int iterationCunt = 0;
    };
    std::vector<Instance> m_lines;
    QSFML::Components::Text* m_text;
};
