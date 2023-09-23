#pragma once

#include "QSFML_base.h"
#include "objects/base/CanvasObject.h"
#include "components/Text.h"

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



        protected:
            void update() override;

        private:
            Components::Text* m_text;
        };
	}
}