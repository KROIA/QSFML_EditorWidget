#pragma once

#include "QSFML_base.h"

namespace QSFML
{
namespace Utilities
{
    class QSFML_EDITOR_WIDGET_EXPORT Updatable
    {
        public:
            virtual ~Updatable()
            {
				if (m_onUpdateExternalFunction)
				{
					delete m_onUpdateExternalFunction;
				}
            }
            void emitUpdate() 
            { 
                update(); 
                if (m_onUpdateExternalFunction)
                {
                    (*m_onUpdateExternalFunction)();
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
