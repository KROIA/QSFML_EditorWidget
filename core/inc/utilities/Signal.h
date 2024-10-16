#pragma once

#include "QSFML_EditorWidget_base.h"
#include <functional>
#include <vector>
#include <string>

namespace QSFML
{
    namespace Utilities
    {
        // Define a Signal class
        template<typename... Args>
        class Signal
        {
        public:
            using SlotFunction = std::function<void(Args...)>;

            // Connects an object member function to this signal
            template<typename ObjectType>
            static SlotFunction bindMember(ObjectType* obj, void(ObjectType::* memberFunc)(Args...))
            {
                return [obj, memberFunc](Args... args) { (obj->*memberFunc)(args...); };
            }





            Signal(const std::string& name)
                : m_name(name)
                , m_profilerName("Emit signal: " + m_name)
            {}

            const std::string& getName() const
            {
                return m_name;
            }

            void connectSlot(const SlotFunction& slot)
            {
                m_slotsWithArgs.emplace_back(SlotFunctionContainer(slot));
            }

            template<typename ObjectType>
            void connectSlot(ObjectType* obj, void(ObjectType::* memberFunc)(Args...))
            {
                m_slotsWithArgs.emplace_back(SlotFunctionContainer(obj, memberFunc));
            }
            void disconnectSlot(const SlotFunction& slot)
            {
                SlotFunctionContainer container = SlotFunctionContainer(slot);
                for (size_t i = 0; i < m_slotsWithArgs.size(); i++)
                {
                    if (m_slotsWithArgs[i].sameTarget(container))
                        //if (getAddress(m_slotsWithArgs[i]) == getAddress(slot))
                    {
                        m_slotsWithArgs.erase(m_slotsWithArgs.begin() + i);
                        break;
                    }
                }
            }
            template<typename ObjectType>
            void disconnectSlot(ObjectType* obj, void(ObjectType::* memberFunc)(Args...))
            {
                SlotFunctionContainer container = SlotFunctionContainer(obj, memberFunc);
                for (size_t i = 0; i < m_slotsWithArgs.size(); i++)
                {
                    if (m_slotsWithArgs[i].sameTarget(container))
                        //if (getAddress(m_slotsWithArgs[i]) == getAddress(slot))
                    {
                        m_slotsWithArgs.erase(m_slotsWithArgs.begin() + i);
                        break;
                    }
                }
            }
            void disconnectAll()
            {
                m_slotsWithArgs.clear();
            }

            size_t getSlotCount() const
            {
                return m_slotsWithArgs.size();
            }

            void emitSignal(Args... args) const
            {
                QSFMLP_GENERAL_BLOCK(m_profilerName.c_str(), QSFML_COLOR_STAGE_10);
                for (const auto& slot : m_slotsWithArgs) {
                    QSFMLP_GENERAL_BLOCK("Slot", QSFML_COLOR_STAGE_11);
                    (*slot.m_func)(args...);
                }
            }

        private:
            struct SlotFunctionContainer
            {
            public:
                SlotFunctionContainer(const SlotFunction& func)
                {
                    m_func = new SlotFunction(func);
                    m_obj = nullptr;
                    m_memberFunc = nullptr;
                }

                template<typename ObjectType>
                SlotFunctionContainer(ObjectType* obj, void(ObjectType::* memberFunc)(Args...))
                {
                    m_func = new SlotFunction(bindMember(obj, memberFunc));
                    m_obj = (void*&)obj;
                    m_memberFunc = (void*&)memberFunc;
                }

                SlotFunctionContainer(const SlotFunctionContainer& other)
                {
                    m_func = new SlotFunction(*other.m_func);
                    m_obj = other.m_obj;
                    m_memberFunc = other.m_memberFunc;
                }
                SlotFunctionContainer(SlotFunctionContainer&& other) noexcept
                {
                    m_func = other.m_func;
                    other.m_func = nullptr;
                    m_obj = other.m_obj;
                    m_memberFunc = other.m_memberFunc;
                }

                ~SlotFunctionContainer()
                {
                    delete m_func;
                }

                SlotFunctionContainer& operator=(SlotFunctionContainer&& other) noexcept
                {
                    m_func = other.m_func;
                    other.m_func = nullptr;
                    m_obj = other.m_obj;
                    m_memberFunc = other.m_memberFunc;
                    return *this;
                }

                bool sameTarget(const SlotFunctionContainer& other) const
                {
                    size_t a1 = getAddress(*m_func);
                    size_t a2 = getAddress(*(other.m_func));
                    if (a1 == a2 && a1 != 0)
                    {
                        return true;
                    }
                    if (m_obj == other.m_obj &&
                        m_memberFunc == other.m_memberFunc)
                    {
                        return true;
                    }
                    return false;
                }

                void operator()(Args... args)
                {
                    invoke(args...);
                }

                void invoke(Args... args)
                {
                    if (m_func)
                    {
                        (*m_func)(args...);
                    }
                }

                
                static size_t getAddress(const SlotFunction& f)
                {
                    typedef void(fnType)(Args...);
                    fnType* fnPointer = *f.target< fnType>();
                    return reinterpret_cast<size_t>(fnPointer);
                }


                SlotFunction* m_func;
                void* m_obj;
                void* m_memberFunc;
            };


            
            const std::string m_name;
            const std::string m_profilerName;
            QSFML::vector<SlotFunctionContainer> m_slotsWithArgs;
        };
    }
}
