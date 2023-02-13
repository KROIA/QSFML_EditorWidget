#include "canvas/CanvasThreadWorker.h"
#include <string>
#include <QDebug>
#include "QSFML_debugSettings.h"


namespace QSFML
{
CanvasThreadWorker::CanvasThreadWorker(size_t threadCount,
                                       std::vector<CanvasObjectGroup*> *group)
{
    m_groups = group;
    setupThreads(threadCount);
}
CanvasThreadWorker::~CanvasThreadWorker()
{
    m_threadExit = true;
    m_cv.notify_all();

    for(size_t i=0; i<m_threads.size(); ++i)
    {
        m_threads[i]->join();
        delete m_threads[i];
        m_threads[i] = nullptr;
    }
    delete[] m_threadRunning;
    //delete[] m_threadFinishedCheckingBuffer;
    m_groups = nullptr;
}

void CanvasThreadWorker::process()
{
    QSFML_PROFILE_CANVAS(EASY_BLOCK("Process threaded update",profiler::colors::Green400))
    m_threadNextIndex = 0;
    size_t threadCount = m_threads.size();
    m_threadMaxIndex = m_groups->size();
    //size_t cpyCount = threadCount * sizeof(size_t);
    //memset(m_threadFinishedCycleCount, 0, cpyCount);


    for(size_t i=0; i<threadCount; ++i)
        m_threadRunning[i] = true;
    ++m_cycleCount;
    m_threadReleaseToWork = !m_threadReleaseToWork.load();
    //m_cv.notify_all();
    using namespace std::chrono_literals;


    bool anyRunning = false;
    do
    {
        std::this_thread::sleep_for(50us);
        anyRunning = false;
        /*{
            std::lock_guard<std::mutex> lk(m_finishedMutex);
            memcpy(m_threadFinishedCheckingBuffer, m_threadFinishedCycleCount, cpyCount);
        }*/

        for(size_t i=0; i<threadCount; ++i)
        {
            anyRunning |= m_threadRunning[i].load();
            //if(m_threadFinishedCheckingBuffer[i] != m_cycleCount)
             //   anyRunning = true;
            //anyRunning &= m_threadFinishedCheckingBuffer[i];
        }
    }while(anyRunning);

}
void CanvasThreadWorker::setupThreads(size_t count)
{
    if(m_threads.size() > 0)
        return;
    m_cycleCount = 0;
    m_threadReleaseToWork = false;
    m_threadMaxIndex = m_groups->size();
    m_threadNextIndex = -1;
    m_threadRunning = new std::atomic<bool>[count];
   // m_threadFinishedCheckingBuffer = new size_t[count];
    m_threadExit = false;
    for(size_t i=0; i<count; ++i)
        m_threadRunning[i] = false;
    //memset(m_threadFinishedCycleCount, 0, count * sizeof(size_t));
    //memset(m_threadFinishedCheckingBuffer, 0, count * sizeof(size_t));

    ThreadsData dataPreset;
    dataPreset.cv = &m_cv;
    dataPreset.exit = &m_threadExit;
    dataPreset.groupsList = m_groups;
    dataPreset.nextIndex = &m_threadNextIndex;
    dataPreset.maxIndex = &m_threadMaxIndex;
    dataPreset.mutex = &m_mutex;
    dataPreset.finishedMutex = &m_finishedMutex;
    //dataPreset.threadReleaseToWork = &m_threadReleaseToWork;
    dataPreset.threadReleaseToWork = &m_threadReleaseToWork;
    m_threads.reserve(count);
    for(size_t i=0; i<count; ++i)
    {
        ThreadsData data(dataPreset);
        data.running = &m_threadRunning[i];
        std::thread *th = new std::thread(&CanvasThreadWorker::threadFunc,data);
        m_threads.push_back(th);
    }
}

void CanvasThreadWorker::threadFunc(ThreadsData data)
{
    using namespace std::chrono_literals;

    bool doExit = false;
    bool releaseThreadToWorkComp = true;
    while(!doExit)
    {
        {
            //std::atomic<bool> *releaseToWork = data.threadReleaseToWork;

            while(data.threadReleaseToWork->load() != releaseThreadToWorkComp)
            {
                std::this_thread::sleep_for(5us);
            }
            //std::atomic<bool> *current = data.finished;
            /*std::unique_lock<std::mutex> lk(*data.mutex);

          //  data.cv->wait(lk);


            data.cv->wait(lk, [releaseToWork, releaseThreadToWorkComp]
            {
                return *releaseToWork == releaseThreadToWorkComp;
            });*/

        }
        releaseThreadToWorkComp = !releaseThreadToWorkComp;
        bool workOnGroups = true;
        CanvasObjectGroup *currentGroup = nullptr;
        while(workOnGroups)
        {
            {
                std::lock_guard<std::mutex> lk(*data.mutex);
                if(*data.nextIndex == *data.maxIndex)
                {
                    workOnGroups = false;
                    continue;
                }
                currentGroup = data.groupsList->operator[]((*data.nextIndex)++);
            }

            if(currentGroup)
            {
                currentGroup->update();
                currentGroup = nullptr;
            }
        }



        {
            std::lock_guard<std::mutex> lk(*data.finishedMutex);
            doExit = *data.exit;
            *data.running = false;
        }

    }
}
}
