#pragma once

#include "QSFML_EditorWidget_base.h"

namespace QSFML
{
    /**
     *  @brief A template class for creating singleton objects.
     *  @tparam T The type of the singleton object.
     */
    template<class T>
    class QSFML_EDITOR_WIDGET_API Singleton {
    public:
        /*
         *  @brief Returns the instance of the singleton object.
         *  @return The instance of the singleton object.
         */
        static T& getInstance() {
            static T instance;
            return instance;
        }

        /**
         *  @brief Deletes the copy constructor of the Singleton class.
         */
        Singleton(const Singleton&) = delete;

        /**
         *  @brief Deletes the assignment operator of the Singleton class.
         */
        Singleton& operator=(const Singleton&) = delete;

    protected:
        /**
         *  @brief Constructs a Singleton object.
         */
        Singleton()
            : m_className(typeid(T).name())
        {

        }

        /**
         *  @brief Destroys the Singleton object.
         */
        ~Singleton() = default;

        const char* m_className; /**< The name of the class of the singleton object. */
    };


}
