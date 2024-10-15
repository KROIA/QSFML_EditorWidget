#pragma once

#include "UnitTest.h"
#include "QSFML_EditorWidget.h"
#include <QObject>
#include <QCoreapplication>

 


class TST_VectorOperations : public UnitTest::Test
{
	TEST_CLASS(TST_VectorOperations)
public:
	TST_VectorOperations()
		: Test("TST_VectorOperations")
	{
		ADD_TEST(TST_VectorOperations::vecLength);
		ADD_TEST(TST_VectorOperations::vecRotating);
		ADD_TEST(TST_VectorOperations::dotP);
		ADD_TEST(TST_VectorOperations::crossP);
		ADD_TEST(TST_VectorOperations::angle);
		ADD_TEST(TST_VectorOperations::performanceTest);

		setBreakOnFail(false);

		setupVectors();
	}

private:
	void onFail(const std::string& message) override
	{
		//std::cout << "Failed: " << message << std::endl;
	}
	template <typename T>
	struct SingleVectorOperationData
	{
		sf::Vector2<T> vec;
		sf::Vector2<T> normalized;
		T length;
		T squareLength;
		T angle;
	};

	template <typename T>
	struct DoubleVectorOperationData
	{
		sf::Vector2<T> vec1;
		sf::Vector2<T> vec2;
		T length1;
		T length2;
		T angle1;
		T angle2;
		T dotProduct;
		T crossProduct;
		T angle;
	};

	double m_margin = 0.0001;
	std::vector<SingleVectorOperationData<float>> m_singleVectorOperationData;
	std::vector<DoubleVectorOperationData<float>> m_doubleVectorOperationData;
	std::vector<SingleVectorOperationData<double>> m_D_singleVectorOperationData;
	std::vector<DoubleVectorOperationData<double>> m_D_doubleVectorOperationData;
	void setupVectors()
	{
		m_singleVectorOperationData.push_back(getSingleVectorData<float>(0.0f, 1.0f));
		m_singleVectorOperationData.push_back(getSingleVectorData<float>(0.0f, 2.0f));
		m_singleVectorOperationData.push_back(getSingleVectorData<float>(0.0f, 3.0f));
		m_singleVectorOperationData.push_back(getSingleVectorData<float>(M_PI_2, 1.0f));
		m_singleVectorOperationData.push_back(getSingleVectorData<float>(M_PI_2, 2.0f));
		m_singleVectorOperationData.push_back(getSingleVectorData<float>(M_PI_2, 3.0f));
		m_singleVectorOperationData.push_back(getSingleVectorData<float>(M_PI, 1.0f));
		m_singleVectorOperationData.push_back(getSingleVectorData<float>(M_PI, 2.0f));
		m_singleVectorOperationData.push_back(getSingleVectorData<float>(M_PI, 3.0f));
		m_singleVectorOperationData.push_back(getSingleVectorData<float>(0.1, 1.0f));
		m_singleVectorOperationData.push_back(getSingleVectorData<float>(0.2, 2.0f));
		m_singleVectorOperationData.push_back(getSingleVectorData<float>(0.3, 3.0f));
		m_singleVectorOperationData.push_back(getSingleVectorData<float>(0.4, 4.0f));
		m_singleVectorOperationData.push_back(getSingleVectorData<float>(0.5, 5.0f));
		m_singleVectorOperationData.push_back(getSingleVectorData<float>(0.6, 6.0f));
		m_singleVectorOperationData.push_back(getSingleVectorData<float>(0.7, 7.0f));

		m_doubleVectorOperationData.push_back(getDoubleVectorData<float>(0.0f, 1.0f, 0.0f, 1.0f));
		m_doubleVectorOperationData.push_back(getDoubleVectorData<float>(0.0f, 1.0f, M_PI_2, 1.0f));
		m_doubleVectorOperationData.push_back(getDoubleVectorData<float>(0.0f, 1.0f, M_PI, 1.0f));
		m_doubleVectorOperationData.push_back(getDoubleVectorData<float>(0.0f, 1.0f, -M_PI_2, 2.0f));
		m_doubleVectorOperationData.push_back(getDoubleVectorData<float>(0.0f, 1.0f, M_PI, 2.0f));
		m_doubleVectorOperationData.push_back(getDoubleVectorData<float>(M_PI, 1.0f, M_PI, 3.0f));
		m_doubleVectorOperationData.push_back(getDoubleVectorData<float>(M_PI, 1.0f, M_PI, 1.0f));
		m_doubleVectorOperationData.push_back(getDoubleVectorData<float>(M_PI, 2.0f, M_PI, 2.0f));
		m_doubleVectorOperationData.push_back(getDoubleVectorData<float>(M_PI, 1.0f, M_PI_2, 3.0f));
		m_doubleVectorOperationData.push_back(getDoubleVectorData<float>(0.4, 1.0f, -0.5, 3.0f));
		m_doubleVectorOperationData.push_back(getDoubleVectorData<float>(0.5, 1.0f, 0.5, 3.0f));
		m_doubleVectorOperationData.push_back(getDoubleVectorData<float>(0.5, 1.0f, 1.5, 3.0f));

		for (auto& data : m_singleVectorOperationData)
		{
			m_D_singleVectorOperationData.push_back(getSingleVectorData<double>(data.angle, data.length));
		}
		for (auto& data : m_doubleVectorOperationData)
		{
			m_D_doubleVectorOperationData.push_back(getDoubleVectorData<double>(data.angle1, data.length1, data.angle2, data.length2));
		}
	}
	template <typename T>
	SingleVectorOperationData<T> getSingleVectorData(T angle, T length)
	{
		SingleVectorOperationData<T> data;
		data.vec = sf::Vector2<T>(std::cos(angle), std::sin(angle)) * length;
		data.normalized = sf::Vector2<T>(std::cos(angle), std::sin(angle));
		data.length = length;
		data.squareLength = length * length;
		data.angle = angle;
		return data;
	}
	template<typename T>
	DoubleVectorOperationData<T> getDoubleVectorData(T angle1, T length1, T angle2, T length2)
	{
		DoubleVectorOperationData<T> data;
		data.vec1 = sf::Vector2<T>(std::cos(angle1), std::sin(angle1)) * length1;
		data.vec2 = sf::Vector2<T>(std::cos(angle2), std::sin(angle2)) * length2;
		data.dotProduct = data.vec1.x * data.vec2.x + data.vec1.y * data.vec2.y;
		data.crossProduct = data.vec1.x * data.vec2.y - data.vec1.y * data.vec2.x;
		T angle = angle2 - angle1;
		while (angle < 0)
		{
			angle += 2 * M_PI;
		}
		while (angle > M_PI)
		{
			angle -= 2 * M_PI;
		}
		data.angle = angle;
		data.length1 = length1;
		data.length2 = length2;
		data.angle1 = angle1;
		data.angle2 = angle2;
		return data;
	}

	template <typename T>
	bool isEqual(const sf::Vector2<T>& v1, const sf::Vector2<T>& v2, T margin)
	{
		return std::abs(v1.x - v2.x) < margin && std::abs(v1.y - v2.y) < margin;
	}
	template <typename T>
	bool isEqual(const sf::Vector2<T>& v1, const sf::Vector2<T>& v2)
	{
		return isEqual(v1, v2, (T)m_margin);
	}
	template <typename T>
	bool isEqual(T v1, T v2, T margin)
	{
		return std::abs(v1 - v2) < margin;
	}
	template <typename T>
	bool isEqual(T v1, T v2)
	{
		return isEqual(v1, v2, (T)m_margin);
	}

	// Tests
	TEST_FUNCTION(vecLength)
	{
		TEST_START;
		
		for (auto& data : m_singleVectorOperationData)
		{
			TEST_ASSERT_M(isEqual(QSFML::VectorMath::getLength(data.vec), data.length), "getLength");
		}
		for (auto& data : m_singleVectorOperationData)
		{
			TEST_ASSERT_M(isEqual(QSFML::VectorMath::getSquareLength(data.vec), data.squareLength), "getSquareLength");
		}
		for (auto& data : m_D_singleVectorOperationData)
		{
			TEST_ASSERT_M(isEqual(QSFML::VectorMath::getLength(data.vec), data.length), "getLength");
		}
		for (auto& data : m_D_singleVectorOperationData)
		{
			TEST_ASSERT_M(isEqual(QSFML::VectorMath::getSquareLength(data.vec), data.squareLength), "getSquareLength");
		}
	}




	TEST_FUNCTION(vecRotating)
	{
		TEST_START(results);

		for (auto& data : m_singleVectorOperationData)
		{
			float rotation = (float)(rand() % 1000) / 1000.0f * 2 * M_PI;
			sf::Vector2f correctRotated1 = sf::Vector2f(std::cos(data.angle + rotation), std::sin(data.angle + rotation)) * data.length;
			TEST_ASSERT_M(isEqual(QSFML::VectorMath::getRotatedRAD(data.vec, rotation), correctRotated1), "getRotated");
		}
		for (auto& data : m_singleVectorOperationData)
		{
			float rotation = (float)(rand() % 1000) / 1000.0f * 2 * M_PI;
			sf::Vector2f correctRotated2 = sf::Vector2f(std::cos(rotation), std::sin(rotation));
			TEST_ASSERT_M(isEqual(QSFML::VectorMath::getRotatedUnitVectorRAD(rotation), correctRotated2), "getRotatedUnitVector");
		}
		for (auto& data : m_D_singleVectorOperationData)
		{
			double rotation = (double)(rand() % 1000) / 1000.0f * 2 * M_PI;
			sf::Vector2<double> correctRotated1 = sf::Vector2<double>(std::cos(data.angle + rotation), std::sin(data.angle + rotation)) * data.length;
			TEST_ASSERT_M(isEqual(QSFML::VectorMath::getRotatedRAD(data.vec, rotation), correctRotated1), "getRotated");
		}
		for (auto& data : m_D_singleVectorOperationData)
		{
			double rotation = (double)(rand() % 1000) / 1000.0f * 2 * M_PI;
			sf::Vector2<double> correctRotated2 = sf::Vector2<double>(std::cos(rotation), std::sin(rotation));
			TEST_ASSERT_M(isEqual(QSFML::VectorMath::getRotatedUnitVectorRAD(rotation), correctRotated2), "getRotatedUnitVector");
		}
	}

	TEST_FUNCTION(dotP)
	{
		TEST_START(results);
		for (auto& data : m_doubleVectorOperationData)
		{
			TEST_ASSERT_M(isEqual(QSFML::VectorMath::dotProduct(data.vec1, data.vec2), data.dotProduct), "dotProduct");
		}
		for (auto& data : m_D_doubleVectorOperationData)
		{
			TEST_ASSERT_M(isEqual(QSFML::VectorMath::dotProduct(data.vec1, data.vec2), data.dotProduct), "dotProduct");
		}
	}

	TEST_FUNCTION(crossP)
	{
		TEST_START(results);
		for (auto& data : m_doubleVectorOperationData)
		{
			TEST_ASSERT_M(isEqual(QSFML::VectorMath::crossProduct(data.vec1, data.vec2), data.crossProduct), "crossProduct");
		}
		for (auto& data : m_D_doubleVectorOperationData)
		{
			TEST_ASSERT_M(isEqual(QSFML::VectorMath::crossProduct(data.vec1, data.vec2), data.crossProduct), "crossProduct");
		}
	}

	TEST_FUNCTION(angle)
	{
		TEST_START(results);
		for (auto& data : m_doubleVectorOperationData)
		{
			TEST_ASSERT_M(isEqual(QSFML::VectorMath::getAngleRAD(data.vec1, data.vec2), data.angle), "getAngle");
		}
		for (auto& data : m_D_doubleVectorOperationData)
		{
			TEST_ASSERT_M(isEqual(QSFML::VectorMath::getAngleRAD(data.vec1, data.vec2), data.angle), "getAngle");
		}
	}

	TEST_FUNCTION(performanceTest)
	{
		TEST_START(results);

		size_t iterations = 10000000;

		{
			size_t time = 0;
			clock_t cpuTime = 0;
			for (size_t i = 0; i < iterations; i++)
			{
				float x = (float)(rand() % 1000) / 1000.0f;
				float y = (float)(rand() % 1000) / 1000.0f;
				auto startTime = std::chrono::high_resolution_clock::now();
				clock_t startCpuTime = std::clock();

				float angle = QSFML::VectorMath::getAngleRAD(sf::Vector2f(x, y));

				auto endTime = std::chrono::high_resolution_clock::now();
				cpuTime += std::clock() - startCpuTime;
				time += std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();

			}
			clock_t cpuTimeMs = cpuTime / (CLOCKS_PER_SEC / 1000);
			TEST_MESSAGE("getAngle in Loop mesurement: " + std::to_string(time) + "ns, CPU time: " + std::to_string(cpuTimeMs) + "ms");
		}
		{
			clock_t cpuTime = clock();
			TimePoint start = std::chrono::high_resolution_clock::now();
			for (size_t i = 0; i < iterations; i++)
			{
				float x = (float)(rand() % 1000) / 1000.0f;
				float y = (float)(rand() % 1000) / 1000.0f;
				float angle = QSFML::VectorMath::getAngleRAD(sf::Vector2f(x, y));
			}
			TimePoint end = std::chrono::high_resolution_clock::now();
			clock_t cpuTimeMs = (clock() - cpuTime) / (CLOCKS_PER_SEC / 1000);
			TEST_MESSAGE("getAngle mesurement: " + std::to_string(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()) + 
						 "ns, CPU time: " + std::to_string(cpuTimeMs) + "ms");
		}


		{
			clock_t cpuTime = clock();
			TimePoint start = std::chrono::high_resolution_clock::now();
			for (size_t i = 0; i < iterations; i++)
			{
				float x = (float)(rand() % 1000) / 1000.0f;
				float y = (float)(rand() % 1000) / 1000.0f;
				float angle = QSFML::VectorMath::getAngleRAD(sf::Vector2f(x, y), sf::Vector2f(y,x+5));
			}
			TimePoint end = std::chrono::high_resolution_clock::now();
			clock_t cpuTimeMs = (clock() - cpuTime) / (CLOCKS_PER_SEC / 1000);
			TEST_MESSAGE("getAngle between two vectors mesurement: " + std::to_string(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()) +
				"ns, CPU time: " + std::to_string(cpuTimeMs) + "ms");
		}
		{
			clock_t cpuTime = clock();
			TimePoint start = std::chrono::high_resolution_clock::now();
			for (size_t i = 0; i < iterations; i++)
			{
				double x = (double)(rand() % 1000) / 1000.0f;
				double y = (double)(rand() % 1000) / 1000.0f;
				double angle = QSFML::VectorMath::getAngleRAD(sf::Vector2<double>(x, y), sf::Vector2<double>(y, x + 5));
			}
			TimePoint end = std::chrono::high_resolution_clock::now();
			clock_t cpuTimeMs = (clock() - cpuTime) / (CLOCKS_PER_SEC / 1000);
			TEST_MESSAGE("getAngle between two vectors<D> mesurement: " + std::to_string(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()) +
				"ns, CPU time: " + std::to_string(cpuTimeMs) + "ms");
		}
	}

};
