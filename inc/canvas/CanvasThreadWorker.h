#pragma once
#include <mutex>
#include <thread>
#include <condition_variable>
#include "CanvasObjectGroup.h"


namespace QSFML
{
    class CanvasThreadWorker
    {
        public:
            CanvasThreadWorker(size_t threadCount,
                               std::vector<CanvasObjectGroup*> *group);
            ~CanvasThreadWorker();

            void process();


        private:
            void setupThreads(size_t count);

            struct ThreadsData;
            static void threadFunc(ThreadsData data);

            std::vector<CanvasObjectGroup*> *m_groups;

            std::condition_variable m_cv;
            std::vector<std::thread*> m_threads;


            bool *m_threadFinished;
            bool *m_threadFinishedCheckingBuffer;
            bool m_threadExit;
            bool m_threadReleaseToWork;
            size_t m_threadNextIndex;
            size_t m_threadMaxIndex;
            std::mutex m_mutex;
            std::mutex m_finishedMutex;
            struct ThreadsData
            {
                std::vector<CanvasObjectGroup*> *groupsList;
                bool *finished;
                bool *exit;
                bool *threadReleaseToWork;
                size_t *nextIndex;
                size_t *maxIndex;
                std::mutex *mutex;
                std::mutex *finishedMutex;
                std::condition_variable *cv;
            };
    };

}
