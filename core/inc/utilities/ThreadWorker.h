#pragma once

#include "QSFML_EditorWidget_base.h"

#include <thread>
#include <functional>
#include <condition_variable>
#include <vector>

namespace QSFML
{
    namespace Utilities
    {
        class ThreadWorker;
        class ThreadWork
        {
            friend ThreadWorker;
        public:
            ThreadWork();
            virtual ~ThreadWork();

            int getThreadIndex() const;

        protected:
            virtual void process(int threadIndex) = 0;

        private:
            int m_threadIndex;
        };

        class ThreadWorker {
        public:
            ThreadWorker(const std::string& name = "ThreadWorker");
            ThreadWorker(ThreadWork* work, const std::string& name = "ThreadWorker");
            ThreadWorker(const std::vector<ThreadWork*>& workload, const std::string& name = "ThreadWorker");
            ThreadWorker(const std::function<void()>& executionFunction, const std::string& name = "ThreadWorker");
            ThreadWorker(const std::vector<std::function<void()>>& executionFunctions, const std::string& name = "ThreadWorker");
            ~ThreadWorker();

            void setWaitingFunc(const std::function<void()>& func);

            void addFunction(const std::function<void()>& function);
            void addWork(ThreadWork* work);

            void signalExecution(bool blocking = false);
            void waitForThreads();
            void waitForThreadsNoDelay();
            void waitForThreadsNoDelayNoExec();
            void stop();

            unsigned int getActiveThreadCount() const;
            bool threadFinished(unsigned int index = 0) const;
            size_t getThreadCount() const;

            void clear();

        private:
            class ThreadData
            {
            public:
                ThreadData();
                ThreadData(const ThreadData& other);

                ThreadWork* work;
                std::function<void()>* func;
                std::thread* thread;
                //void* param;
                std::atomic<bool> shouldExecute;
                std::atomic<bool> threadExited;
                std::atomic<bool> threadCrashed;
                int threadIndex = 0;
                //std::mutex *mutex = nullptr;
            };
            void threadFunction(ThreadData& data);

            std::string m_name;
            std::vector<ThreadData*> m_threads;
            std::mutex m_mutex;
            std::condition_variable m_conditionVar;
            std::function<void()>* m_sleepWorkFunc = nullptr; // function gets called while waiting for threads

            std::atomic<bool> m_run = true;
            std::atomic<int> m_runCount = 0;
        };
    }
}
