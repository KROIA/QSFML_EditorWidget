#pragma once

#include "QSFML_EditorWidget_base.h"
#include <functional>

namespace QSFML
{
namespace Utilities
{
    class QSFML_EDITOR_WIDGET_EXPORT Updatable
    {
        public:
            virtual ~Updatable()
            {
				delete m_onUpdateExternalFunction;
            }
            void emitUpdate() 
            { 
                if (m_onUpdateExternalFunction)
                {
                    (*m_onUpdateExternalFunction)();
                }
                else
				{
					update();
				}
            }

            void setUpdateFunction(const std::function<void()> &function)
			{
				m_onUpdateExternalFunction = new std::function<void()>(function);
			}
        protected:

            virtual void update() = 0;

        private:
            std::function<void()> *m_onUpdateExternalFunction = nullptr;
    };
}
}
