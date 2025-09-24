#pragma once

#include "QSFML_EditorWidget_base.h"
#include <SFML/Graphics/Image.hpp>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <QTimer>

namespace QSFML
{
	namespace Utilities
	{
		class QSFML_EDITOR_WIDGET_API CameraRecorder : public QObject
		{
			Q_OBJECT
			public:
			CameraRecorder(Objects::CameraWindow* cam);
			CameraRecorder(Objects::CameraWindow* cam, size_t threadCount);
			~CameraRecorder();

			bool startCapture(size_t frameCount, float timerInterval, const std::string &outputFolder);
			bool startCaptureBuffered(size_t frameCount, float timerInterval, const std::string &outputFolder);



			private slots:
			void onTimer();
			void captureInternal();

			private:
			void setup();
			void saveBufferedImages();

			struct ThreadData
			{
				struct ImageData
				{
					sf::Image* image = nullptr;
					size_t index;
				};
				ThreadData()
				{}
				ThreadData(const ThreadData&)
				{}
				std::thread* thread = nullptr;
				QSFML::vector<ImageData> images;
				std::mutex mutex;
				std::condition_variable condition;
				volatile bool running = true;
				//Log::LogObject log;
			};
			static void threadFunction(ThreadData& data, CameraRecorder &recorder);


			QSFML::vector<ThreadData> m_threadData;
			std::string m_outputFolder;
			bool m_isCapturing = false;
			bool m_bufferMode = false;
			size_t m_threadCount = 0;
			size_t m_frameCounter = 0;
			size_t m_targetFrameCount = 0;
			Objects::CameraWindow* m_cameraWindow = nullptr;
			QTimer* m_timer = nullptr;

			QSFML::vector<ThreadData::ImageData> m_bufferedImages;
		};
	}
}