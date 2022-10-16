#include "AbstractPressEvent.h"

namespace QSFML
{
namespace Components
{
AbstractPressEvent::AbstractPressEvent(const std::string &name)
    : QObject()
    , Component(name)
{
    m_fallingEdge = false;
    m_down = false;
    m_risingEdge = false;
}

void AbstractPressEvent::update()
{
    if(getCurrentValue())
    {
        if(!m_down)
            m_fallingEdge = true;
    }else
    {
        if(m_down)
        {
            m_down = false;
            m_risingEdge = true;
        }
    }

    if(m_fallingEdge)
    {
        emit fallingEdge();
        m_fallingEdge = false;
        m_down = true;
    }
    else if(m_down)
    {
        emit down();
    }else if(m_risingEdge)
    {
        emit risingEdge();
        m_risingEdge = false;
    }
}

}
}
