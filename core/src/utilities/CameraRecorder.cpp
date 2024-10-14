#include "utilities/CameraRecorder.h"
#include "objects/CameraWindow.h"
#include <QDir>

namespace QSFML
{
	namespace Utilities
	{
		CameraRecorder::CameraRecorder(Objects::CameraWindow* cam)
			: m_cameraWindow(cam)
		{
			setup();
		}
		CameraRecorder::CameraRecorder(Objects::CameraWindow* cam, size_t threadCount)
			: m_threadCount(threadCount)
			, m_cameraWindow(cam)
		{
			setup();
		}
		CameraRecorder::~CameraRecorder()
		{
			for (auto& data : m_threadData)
			{
				data.running = false;
				data.condition.notify_all();
				if (data.thread)
				{
					data.thread->join();
					delete data.thread;
				}
			}
			m_threadData.clear();
		}



		void CameraRecorder::setup()
		{
			if (m_threadCount > 0)
			{
				if (m_threadCount > std::thread::hardware_concurrency())
				{
					m_threadCount = std::thread::hardware_concurrency();
				}
				m_threadData.resize(m_threadCount);
				for (size_t i = 0; i < m_threadCount; ++i)
				{
					m_threadData[i].thread = new std::thread(threadFunction, std::ref(m_threadData[i]), std::ref(*this));
					HANDLE threadHandle = m_threadData[i].thread->native_handle();
					SetThreadDescription(threadHandle, L"CameraRecorder Thread");
					SetThreadPriority(threadHandle, THREAD_PRIORITY_LOWEST);
				}
			}
		}

		

		bool CameraRecorder::startCapture(size_t frameCount, float timerInterval, const std::string& outputFolder)
		{
			if (m_isCapturing)
				return false;

			m_outputFolder = outputFolder;
			m_targetFrameCount = frameCount;
			if (!m_timer)
			{
				m_timer = new QTimer(this);
				connect(m_timer, &QTimer::timeout, this, &CameraRecorder::onTimer);
			}
			if (!QDir().exists(QString::fromStdString(m_outputFolder)))
			{
				if (!QDir().mkdir(QString::fromStdString(m_outputFolder)))
				{
					return false;
				}
			}
			m_timer->start(timerInterval*1000);
			m_isCapturing = true;
			m_frameCounter = 0;

			return true;
		}
		bool CameraRecorder::startCaptureBuffered(size_t frameCount, float timerInterval, const std::string& outputFolder)
		{
			if (m_isCapturing)
				return false;
			m_bufferMode = true;
			m_bufferedImages.reserve(frameCount);
			return startCapture(frameCount, timerInterval, outputFolder);
		}


		void CameraRecorder::onTimer()
		{
			if (m_frameCounter >= m_targetFrameCount)
			{
				m_timer->stop();
				m_isCapturing = false;
				if (m_bufferMode)
				{
					saveBufferedImages();
				}
				return;
			}
			captureInternal();
		}
		void CameraRecorder::captureInternal()
		{
			QSFMLP_GENERAL_FUNCTION(QSFML_COLOR_STAGE_1);

			ThreadData::ImageData newImage;
			newImage.image = new sf::Image();
			newImage.index = m_frameCounter;
			{
				QSFMLP_GENERAL_BLOCK("Capture Image", QSFML_COLOR_STAGE_2);
				m_cameraWindow->captureThisCameraScreen(*newImage.image);
			}
			++m_frameCounter;

			if (m_bufferMode)
			{
				m_bufferedImages.push_back(newImage);
			}
			else
			{
				if (m_threadCount > 0)
				{
					size_t currentIndex = m_frameCounter % m_threadCount;
					ThreadData& data = m_threadData[currentIndex];

					{
						std::lock_guard<std::mutex> lock(data.mutex);
						data.images.push_back(newImage);
						data.condition.notify_one();
					}
				}
				else
				{
					QSFMLP_GENERAL_BLOCK("Saving Image", QSFML_COLOR_STAGE_2);
					QSFMLP_GENERAL_TEXT("ImageIndex", std::to_string(m_frameCounter));
					newImage.image->saveToFile(m_outputFolder + "/" + std::to_string(newImage.index) + ".jpg");
					delete newImage.image;
				}
			}			
		}

		void CameraRecorder::saveBufferedImages()
		{
			QSFMLP_GENERAL_FUNCTION(QSFML_COLOR_STAGE_1);
			size_t threadCount = m_threadData.size();
			if (threadCount > 0)
			{
				// Split the images and give them to the threads
				for (size_t i = 0; i < threadCount; ++i)
				{
					ThreadData& data = m_threadData[i];
					size_t start = i * m_bufferedImages.size() / threadCount;
					size_t end = (i + 1) * m_bufferedImages.size() / threadCount;
					if (i == threadCount - 1)
					{
						end = m_bufferedImages.size();
					}
					{
						std::lock_guard<std::mutex> lock(data.mutex);
						data.images.insert(data.images.end(), m_bufferedImages.begin() + start, m_bufferedImages.begin() + end);
						data.condition.notify_one();
					}
				}
			}
			else
			{
				QSFMLP_GENERAL_BLOCK("Saving Images", QSFML_COLOR_STAGE_2);
				for (auto image : m_bufferedImages)
				{
					QSFMLP_GENERAL_TEXT("ImageIndex", std::to_string(image.index));
					image.image->saveToFile(m_outputFolder + "/" + std::to_string(image.index) + ".jpg");
					delete image.image;
				}
			}
			m_bufferedImages.clear();
			m_bufferMode = false;
		}
		void CameraRecorder::threadFunction(ThreadData& data, CameraRecorder& recorder)
		{
			QSFML_PROFILING_THREAD("CameraRecorder Thread");
			while (true)
			{
				bool isRunning = false;
				std::vector<ThreadData::ImageData> images;
				{
					QSFMLP_GENERAL_BLOCK("Waiting for image", QSFML_COLOR_STAGE_1);
					std::unique_lock<std::mutex> lock(data.mutex);
					data.condition.wait(lock, [&data] { return !data.images.empty() || !data.running; });

					images.swap(data.images);
					data.images.reserve(10);
					isRunning = data.running;
					lock.unlock();
				}
				{
					QSFMLP_GENERAL_BLOCK("Saving Images", QSFML_COLOR_STAGE_1);
					if (!isRunning)
					{
						break;
					}
					for (auto image : images)
					{
						QSFMLP_GENERAL_BLOCK("Saving Image", QSFML_COLOR_STAGE_2);
						QSFMLP_GENERAL_TEXT("ImageIndex", std::to_string(image.index));
						image.image->saveToFile(recorder.m_outputFolder + "/" + std::to_string(image.index) + ".jpg");
						delete image.image;
					}
				}
				// data.log.logInfo("Saved " + std::to_string(images.size()) + " images");
			}
		}
	}
}