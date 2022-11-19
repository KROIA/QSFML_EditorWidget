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
    //m_nextGroupIndex = 0;
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
    delete[] m_threadFinished;
    delete[] m_threadFinishedCheckingBuffer;
    m_groups = nullptr;
}

void CanvasThreadWorker::process()
{
    QSFML_PROFILE_CANVAS(EASY_BLOCK("Process threaded update",profiler::colors::Green400))
    m_threadNextIndex = 0;
    m_threadMaxIndex = m_groups->size();
    size_t cpyCount = m_threads.size() * sizeof(bool);
    std::memset(m_threadFinished, 0, cpyCount);

    //qDebug() << "start threads";

    m_threadReleaseToWork = true;
    m_cv.notify_all();

    bool anyRunning;
    do
    {
        anyRunning = false;
        {
            std::unique_lock<std::mutex> lk(m_finishedMutex);
            std::memcpy(m_threadFinishedCheckingBuffer, m_threadFinished, cpyCount);
        }

        for(size_t i=0; i<m_threads.size(); ++i)
        {
            anyRunning |= !m_threadFinishedCheckingBuffer[i];
        }
       // qDebug() << "waiting for threads to finish...";
    }while(anyRunning);
    //qDebug() << "threads finished";

}
void CanvasThreadWorker::setupThreads(size_t count)
{
    if(m_threads.size() > 0)
        return;
    m_threadReleaseToWork = false;
    m_threadMaxIndex = m_groups->size();
    m_threadNextIndex = -1;
    m_threadFinished = new bool[count];
    m_threadFinishedCheckingBuffer = new bool[count];
    m_threadExit = false;
    std::memset(m_threadFinished, 0, count * sizeof(bool));
    std::memset(m_threadFinishedCheckingBuffer, 0, count * sizeof(bool));

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
        data.finished = &m_threadFinished[i];
        std::thread *th = new std::thread(&CanvasThreadWorker::threadFunc,data);
        m_threads.push_back(th);
    }
}

void CanvasThreadWorker::threadFunc(ThreadsData data)
{
    bool doExit = false;
    while(!doExit)
    {
        {
            bool *releaseToWork= data.threadReleaseToWork;
            std::unique_lock<std::mutex> lk(*data.mutex);

            data.cv->wait(lk, [releaseToWork]
            {
                return *releaseToWork;
            });
        }
        bool workOnGroups = true;
        CanvasObjectGroup *currentGroup = nullptr;
        while(workOnGroups)
        {
            {
                std::unique_lock<std::mutex> lk(*data.mutex);
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
            std::unique_lock<std::mutex> lk(*data.finishedMutex);
            doExit = *data.exit;
            *data.finished = true;
        }
    }
}
}
