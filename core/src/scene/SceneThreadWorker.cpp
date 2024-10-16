#include "Scene/SceneThreadWorker.h"
#include "Scene/GameObjectGroup.h"

#include <string>
#include <thread>

namespace QSFML
{
SceneThreadWorker::SceneThreadWorker(size_t threadCount,
                                       QSFML::vector<GameObjectGroup*> *group)
{
    m_groups = group;
    setupThreads(threadCount);
}
SceneThreadWorker::~SceneThreadWorker()
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

void SceneThreadWorker::process()
{
    QSFMLP_SCENE_BLOCK("Process threaded update", QSFML_COLOR_STAGE_1);
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
    QSFML_PROFILING_END_BLOCK;
}
void SceneThreadWorker::setupThreads(size_t count)
{
    if(m_threads.size() > 0)
        return;
    QSFMLP_SCENE_FUNCTION(QSFML_COLOR_STAGE_1);
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
        std::thread *th = new std::thread(&SceneThreadWorker::threadFunc,data);
        m_threads.push_back(th);
    }
}

void SceneThreadWorker::threadFunc(ThreadsData data)
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
        QSFMLP_SCENE_BLOCK("Threaded update", QSFML_COLOR_STAGE_1);
        releaseThreadToWorkComp = !releaseThreadToWorkComp;
        bool workOnGroups = true;
        GameObjectGroup *currentGroup = nullptr;
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


        QSFML_PROFILING_END_BLOCK;
        {
            std::lock_guard<std::mutex> lk(*data.finishedMutex);
            doExit = *data.exit;
            *data.running = false;
        }

    }
}
}
