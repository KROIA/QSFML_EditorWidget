#include "canvas/CanvasThreadWorker.h"
#include "canvas/CanvasObjectGroup.h"

#include <string>
#include <thread>

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
    m_groups = nullptr;
}

void CanvasThreadWorker::process()
{
    QSFMLP_CANVAS_BLOCK("Process threaded update", QSFMLP_COLOR_STAGE_1);
    m_threadNextIndex = 0;
    size_t threadCount = m_threads.size();
    m_threadMaxIndex = m_groups->size();

    for(size_t i=0; i<threadCount; ++i)
        m_threadRunning[i] = true;
    ++m_cycleCount;
    m_threadReleaseToWork = !m_threadReleaseToWork.load();

    using namespace std::chrono_literals;


    bool anyRunning = false;
    do
    {
        std::this_thread::sleep_for(5us);
        anyRunning = false;
        for(size_t i=0; i<threadCount; ++i)
        {
            anyRunning |= m_threadRunning[i].load();
        }
    }while(anyRunning);
    QSFMLP_END_BLOCK;
}
void CanvasThreadWorker::setupThreads(size_t count)
{
    if(m_threads.size() > 0)
        return;
    QSFMLP_CANVAS_FUNCTION(QSFMLP_COLOR_STAGE_1);
    m_cycleCount = 0;
    m_threadReleaseToWork = false;
    m_threadMaxIndex = m_groups->size();
    m_threadNextIndex = -1;
    m_threadRunning = new std::atomic<bool>[count];
    m_threadExit = false;
    for(size_t i=0; i<count; ++i)
        m_threadRunning[i] = false;

    ThreadsData dataPreset;
    dataPreset.cv = &m_cv;
    dataPreset.exit = &m_threadExit;
    dataPreset.groupsList = m_groups;
    dataPreset.nextIndex = &m_threadNextIndex;
    dataPreset.maxIndex = &m_threadMaxIndex;
    dataPreset.mutex = &m_mutex;
    dataPreset.finishedMutex = &m_finishedMutex;
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
        while(data.threadReleaseToWork->load() != releaseThreadToWorkComp)
        {
            std::this_thread::sleep_for(5us);
        }
        QSFMLP_CANVAS_BLOCK("Threaded update", QSFMLP_COLOR_STAGE_1);
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


        QSFMLP_END_BLOCK;
        {
            std::lock_guard<std::mutex> lk(*data.finishedMutex);
            doExit = *data.exit;
            *data.running = false;
        }

    }
}
}
