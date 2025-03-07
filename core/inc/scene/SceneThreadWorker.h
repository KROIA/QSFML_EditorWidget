#pragma once

#include "QSFML_EditorWidget_base.h"

#include <mutex>
#include <condition_variable>
#include <atomic>
#include <vector>


namespace QSFML
{
    class QSFML_EDITOR_WIDGET_EXPORT SceneThreadWorker
    {
        public:
            SceneThreadWorker(size_t threadCount,
                               QSFML::vector<GameObjectGroup*> *group);
            ~SceneThreadWorker();

            void process();


        private:
            void setupThreads(size_t count);

            struct ThreadsData;
            static void threadFunc(ThreadsData data);

            QSFML::vector<GameObjectGroup*> *m_groups;

            std::condition_variable m_cv;

            QSFML::vector<std::thread*> m_threads;


            std::atomic<bool> *m_threadRunning;
            size_t m_cycleCount;
            bool m_threadExit;
            std::atomic<bool> m_threadReleaseToWork;
            size_t m_threadNextIndex;
            size_t m_threadMaxIndex;
            std::mutex m_mutex;
            std::mutex m_finishedMutex;
            struct ThreadsData
            {
                QSFML::vector<GameObjectGroup*> *groupsList;
                std::atomic<bool> *running;
                bool *exit;
                std::atomic<bool> *threadReleaseToWork;
                size_t *nextIndex;
                size_t *maxIndex;
                std::mutex *mutex;
                std::mutex *finishedMutex;
                std::condition_variable *cv;
            };
    };

}
