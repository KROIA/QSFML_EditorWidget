#pragma once

#include "QSFML_EditorWidget_base.h"
#include "objects/base/CanvasObject.h"
#include "components/drawable/Text.h"
#include "utilities/Stats.h"

namespace QSFML
{
	namespace Objects
	{
        class QSFML_EDITOR_WIDGET_EXPORT RuntimeInfo : public CanvasObject
        {
        public:
            RuntimeInfo(const std::string& name = "RuntimeInfo");
            RuntimeInfo(const RuntimeInfo& other);
            ~RuntimeInfo();
            OBJECT_DECL(RuntimeInfo);

            void enableSmoothStats(bool enable);
            bool isSmoothStatsEnabled() const;

        protected:
            void update() override;

        private:
            Components::Text* m_text;
            QSFML::Utilities::Stats m_oldStats;
            bool m_smoothStats;
        };
	}
}