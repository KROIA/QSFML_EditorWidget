#include "sandbox.h"

#include <QApplication>
#include <chrono>
#include <string>
#include <vector>
#include <QDebug>

using std::string;
using std::vector;

void benchmark();
void printTime(const string &msg, const std::chrono::duration<long long, std::ratio<1,1000000000>> &duration);



int main(int argc, char *argv[])
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    // Enable scaling for high resolution displays
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#endif
    QApplication a(argc, argv);

   // thread_pool.Schedule(f4);


   // EASY_PROFILER_ENABLE;
    SandBox* w = new SandBox();
    w->show();
    //benchmark();
    int ret = a.exec();
    delete w;
    qDebug() << "Objects that are not deleted: " << QSFML::Internal::LifetimeChecker::getAliveCount() << " of total: " << QSFML::Internal::LifetimeChecker::getTotalCount();
 //   profiler::dumpBlocksToFile("test_profile.prof");
    //QSFML::Scene::startEventLoop();
    return ret;
}

void benchmark()
{

    vector<size_t> list;
    size_t count = 10000000;
    list.reserve(count);
    auto start = std::chrono::high_resolution_clock::now();
    for(size_t i=0; i<count; ++i)
    {
        list.push_back(i);
    }
    auto end = std::chrono::high_resolution_clock::now();
    printTime("InitLists",end-start);


    start = std::chrono::high_resolution_clock::now();
    for(size_t i=0; i<list.size(); ++i)
    {
        ++list[i];
    }
    end = std::chrono::high_resolution_clock::now();
    printTime("Increment 1",end-start);

    start = std::chrono::high_resolution_clock::now();
    size_t* begin = list.data();
    size_t* endPos = begin + count;
    for(size_t* it = begin; it < endPos; ++it)
    {
        (*it)++;
    }
    end = std::chrono::high_resolution_clock::now();
    printTime("Increment 2",end-start);

    start = std::chrono::high_resolution_clock::now();
    for(std::vector<size_t>::iterator it = list.begin(); it != list.end(); ++it) {
        ++(*it);
    }
    end = std::chrono::high_resolution_clock::now();
    printTime("Increment 3",end-start);


    bool allOk = true;
    start = std::chrono::high_resolution_clock::now();
    for(size_t i=0; i<list.size(); ++i)
    {
        if(list[i] != i+3)
            allOk = false;
    }
    end = std::chrono::high_resolution_clock::now();
    printTime("Increment check",end-start);
    qDebug() << "Check = "<<allOk;

}

void printTime(const string &msg, const std::chrono::duration<long long, std::ratio<1,1000000000>> &duration)
{
    auto us = std::chrono::duration_cast<std::chrono::microseconds>(duration); // Microsecond (as int)
   // auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration); // Milliseconds (as int)
    const float ms_fractional = static_cast<float>(us.count()) / 1000;         // Milliseconds (as float)
    qDebug() << msg.c_str() << " Duration = " << us.count() << "µs (" << ms_fractional << "ms)";
}
