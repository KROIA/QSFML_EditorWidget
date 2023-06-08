#include "utilities/ThreadWorker.h"


namespace QSFML
{
    namespace Utilities
    {



        ThreadWork::ThreadWork()
        {

        }

        ThreadWork::~ThreadWork()
        {

        }
        int ThreadWork::getThreadIndex() const
        {
            return m_threadIndex;
        }



        ThreadWorker::ThreadData::ThreadData()
        {
            work = nullptr;
            func = nullptr;
            thread = nullptr;
            shouldExecute.store(false);
            threadExited.store(false);
            threadCrashed.store(false);
            threadIndex = 0;
        }
        ThreadWorker::ThreadData::ThreadData(const ThreadData& other)
        {
            work = other.work;
            func = other.func;
            thread = other.thread;
            shouldExecute.store(other.shouldExecute.load());
            threadExited.store(other.threadExited.load());
            threadCrashed.store(other.threadCrashed.load());
            threadIndex = other.threadIndex;
            //mutex = other.mutex;
        }

        ThreadWorker::ThreadWorker(const std::string& name)
            : m_name(name)
            , m_sleepWorkFunc(nullptr)
        {

        }
        ThreadWorker::ThreadWorker(ThreadWork* work, const std::string& name)
            : m_name(name)
            , m_sleepWorkFunc(nullptr)
        {
            addWork(work);
        }
        ThreadWorker::ThreadWorker(const std::vector<ThreadWork*>& workload, const std::string& name)
            : m_name(name)
            , m_sleepWorkFunc(nullptr)
        {
            m_threads.reserve(workload.size());
            for (auto work : workload)
            {
                addWork(work);
            }
        }
        ThreadWorker::ThreadWorker(const std::function<void()>& executionFunction, const std::string& name)
            : m_name(name)
        {

            addFunction(executionFunction);
        }
        ThreadWorker::ThreadWorker(const std::vector < std::function<void()>>& executionFunctions, const std::string& name)
            : m_name(name)
            , m_sleepWorkFunc(nullptr)
        {

            m_threads.reserve(executionFunctions.size());
            for (auto& func : executionFunctions)
            {
                addFunction(func);
            }
        }
        ThreadWorker::~ThreadWorker()
        {
            stop();

            bool allFinished = true;
            do {
                allFinished = true;
                for (auto& data : m_threads)
                    allFinished &= data->threadExited.load();
                if (!allFinished)
                {
                    using namespace std::chrono_literals;
                    std::this_thread::sleep_for(10us);
                }
            } while (!allFinished);
            for (auto& data : m_threads)
            {
                delete data->func;
                data->thread->join();
                delete data->thread;
                delete data;
                // delete data.mutex;
            }
            delete m_sleepWorkFunc;
            m_threads.clear();
            m_sleepWorkFunc = nullptr;
        }

        void ThreadWorker::setWaitingFunc(const std::function<void()>& func)
        {
            if (m_sleepWorkFunc)
                delete m_sleepWorkFunc;
            m_sleepWorkFunc = new std::function<void()>(func);
        }
        void ThreadWorker::addFunction(const std::function<void()>& function)
        {
            ThreadData* data = new ThreadData();
            m_threads.push_back(data);
            // data.mutex       = new std::mutex;
            data->threadIndex = m_threads.size() - 1;
            //data.shouldExecute.store(false);
            data->func = new std::function<void()>(function);
            data->thread = new std::thread(&ThreadWorker::threadFunction, this, std::ref(*data));
        }
        void ThreadWorker::addWork(ThreadWork* work)
        {
            if (!work)
                return;
            ThreadData* data = new ThreadData();
            m_threads.push_back(data);
            // data.mutex = new std::mutex;
            data->work = work;
            data->threadIndex = m_threads.size() - 1;
            work->m_threadIndex = data->threadIndex;
            //data.shouldExecute.store(false);
            data->thread = new std::thread(&ThreadWorker::threadFunction, this, std::ref(*data));
        }
        void ThreadWorker::signalExecution(bool blocking)
        {
            waitForThreads();

            {
                std::lock_guard<std::mutex> lock(m_mutex);
                for (auto& el : m_threads)
                    el->shouldExecute.store(true);
            }
            m_conditionVar.notify_all();
            if (blocking)
            {
                waitForThreads();
            }
        }
        void ThreadWorker::waitForThreads()
        {
            while (getActiveThreadCount() != 0)
            {
                if (m_sleepWorkFunc)
                    (*m_sleepWorkFunc)();
                else
                    std::this_thread::sleep_for(std::chrono::microseconds(1));
            }
        }
        void ThreadWorker::waitForThreadsNoDelay()
        {
            while (getActiveThreadCount() != 0)
            {
                if (m_sleepWorkFunc)
                    (*m_sleepWorkFunc)();
            }
        }
        void ThreadWorker::waitForThreadsNoDelayNoExec()
        {
            while (getActiveThreadCount() > 0)
            {}
        }
        void ThreadWorker::stop()
        {
            m_run.store(false);
            signalExecution();
        }
        unsigned int ThreadWorker::getActiveThreadCount() const
        {
            return m_runCount;
            /*unsigned int count = 0;
            for (auto& work : m_threads)
                if (work->shouldExecute.load())
                    ++count;
            return count;*/
        }
        bool ThreadWorker::threadFinished(unsigned int index) const
        {
            return !m_threads[index]->shouldExecute.load();
        }
        size_t ThreadWorker::getThreadCount() const
        {
            return m_threads.size();
        }
        void ThreadWorker::clear()
        {
            stop();
            bool allFinished = true;
            do {
                allFinished = true;
                for (auto& data : m_threads)
                    allFinished &= data->threadExited.load();
                if (!allFinished)
                {
                    using namespace std::chrono_literals;
                    std::this_thread::sleep_for(10us);
                }
            } while (!allFinished);
            for (auto& data : m_threads)
            {
                delete data->func;
                data->thread->join();
                delete data->thread;
                delete data;
                // delete data.mutex;
            }
            m_threads.clear();

        }
        void ThreadWorker::threadFunction(ThreadData& data)
        {
            QSFMLP_THREAD((m_name + "[" + std::to_string(data.threadIndex) + "]").c_str());
            class CrashDetector
            {
            public:
                CrashDetector(ThreadData& result, std::atomic<int> &activeCount)
                    : m_signal(&result)
                    , m_activeCount(activeCount)
                {

                }
                void normalExit()
                {
                    m_signal = nullptr;
                }
                ~CrashDetector()
                {
                    if (m_signal)
                    {
                        m_signal->threadCrashed.store(true);
                        m_signal->threadExited.store(true);
                        m_activeCount -= m_signal->shouldExecute;
                        m_signal->shouldExecute.store(false);
                    }
                }
            private:
                ThreadData* m_signal = nullptr;
                std::atomic<int>& m_activeCount;
            };

            CrashDetector detector(data, m_runCount);
            ThreadWork* work = data.work;
            std::function<void()>* func = data.func;
            int threadIndex = data.threadIndex;
            std::atomic<bool>* shouldExecute = &data.shouldExecute;
            if (data.func)
            {
                while (m_run.load()) {
                    {
                        std::unique_lock<std::mutex> lock(m_mutex);
                        // Wait until the main thread signals to execute the member function
                        m_conditionVar.wait(lock, [shouldExecute] { return shouldExecute->load(); });
                    }
                    if (!m_run.load())
                        goto finish;

                    ++m_runCount;
                    // Execute the member function
                    QSFMLP_BLOCK("Start thread work", QSFMLP_THREAD_COLOR_1);
                    (*func)();
                    QSFMLP_END_BLOCK;

                    // Reset the signal flag
                    shouldExecute->store(false);
                    --m_runCount;
                }
            }
            else if (data.work)
            {
                while (m_run.load()) {
                    {
                        std::unique_lock<std::mutex> lock(m_mutex);
                        // Wait until the main thread signals to execute the member function
                        m_conditionVar.wait(lock, [shouldExecute] { return shouldExecute->load(); });
                    }
                    if (!m_run.load())
                        goto finish;

                    ++m_runCount;
                    // Execute the member function
                    QSFMLP_BLOCK("Start thread work", QSFMLP_THREAD_COLOR_1);
                    work->process(threadIndex);
                    QSFMLP_END_BLOCK;

                    // Reset the signal flag
                    shouldExecute->store(false);
                    --m_runCount;
                }
            }


        finish:
            {
                data.threadExited.store(true);
                shouldExecute->store(false);
                detector.normalExit();
            }
        }

    }
}