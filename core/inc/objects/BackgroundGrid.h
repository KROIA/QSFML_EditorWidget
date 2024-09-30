#pragma once

#include "QSFML_EditorWidget_base.h"
#include "objects/base/GameObject.h"
#include "components/base/Drawable.h"


namespace QSFML
{
namespace Objects
{
class QSFML_EDITOR_WIDGET_EXPORT BackgroundGrid: public GameObject
{
        class DrawableComp;
    public:
        BackgroundGrid(const std::string &name = "",
                       GameObject *parent = nullptr);
        BackgroundGrid(const BackgroundGrid &other);
        ~BackgroundGrid();
        COMPONENT_DECL(BackgroundGrid);

        void setSize(const sf::IntRect &size);
        const sf::IntRect &getSize() const;

        void setLineColor(const std::vector<sf::Color> &alternatingColors);
        const std::vector<sf::Color> &getLineColor() const;

        void setGridSpacing(unsigned int spacing);
        unsigned int getGridSpacing() const;

        void update() override;

    private:
        sf::IntRect m_gridArea;
        unsigned int m_gridSpacing;
        std::vector<sf::Color> m_alternatingColors;

        DrawableComp *m_draw;

        friend DrawableComp;

        class QSFML_EDITOR_WIDGET_EXPORT DrawableComp : public Components::Drawable
        {
        public:
            DrawableComp(const std::string& name = "DrawableComp");
            DrawableComp(const Drawable& other);
            void drawComponent(sf::RenderTarget& target, sf::RenderStates states) const final;

            void drawGrid(sf::RenderTarget& target,
                const sf::IntRect& area,
                unsigned int spacing,
                const std::vector<sf::Color>& alternatingColors) const;

            BackgroundGrid* m_grid;
        };
};

}
}
