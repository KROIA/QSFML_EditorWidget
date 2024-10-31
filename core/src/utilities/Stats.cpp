#include "utilities/Stats.h"
#include "Scene/Scene.h"

#if IMPLOT_LIBRARY_AVAILABLE == 1
#include "implot.h"
#endif

#include <QDebug>

namespace QSFML
{
	namespace Utilities
	{
		Stats::Stats()
			: m_rootObjectsCount(0)
			, m_objectsCount(0)
			, m_componentsCount(0)
			, m_collisionChecks(0)
			, m_boundingBoxCollisionChecks(0)
			, m_collisions(0)
			, m_fps(0)
			, m_tps(0)
			, m_frameTime(0)
			, m_eventTime(0)
			, m_updateTime(0)
			, m_drawTime(0)
			, m_tick(0)
			, m_deltaT(0)
			, m_fixedDeltaT(0)
			, m_elapsedTime(0)
		{
#if IMPLOT_LIBRARY_AVAILABLE == 1
			clearAllPlotBuffer();
#endif
		}
		Stats::Stats(const Stats& other)
			: m_rootObjectsCount(other.m_rootObjectsCount.load())
			, m_objectsCount(other.m_objectsCount.load())
			, m_componentsCount(other.m_componentsCount.load())
			, m_collisionChecks(other.m_collisionChecks.load())
			, m_boundingBoxCollisionChecks(other.m_boundingBoxCollisionChecks.load())
			, m_collisions(other.m_collisions.load())
			, m_fps(other.m_fps)
			, m_tps(other.m_tps)
			, m_frameTime(other.m_frameTime)
			, m_eventTime(other.m_eventTime)
			, m_updateTime(other.m_updateTime)
			, m_drawTime(other.m_drawTime)
			, m_tick(other.m_tick)
			, m_deltaT(other.m_deltaT)
			, m_fixedDeltaT(other.m_fixedDeltaT)
			, m_elapsedTime(other.m_elapsedTime)
		{
#if IMPLOT_LIBRARY_AVAILABLE == 1
			clearAllPlotBuffer();
#endif
		}
		Stats& Stats::operator=(const Stats& other)
		{
			m_rootObjectsCount = other.m_rootObjectsCount.load();
			m_objectsCount = other.m_objectsCount.load();
			m_componentsCount = other.m_componentsCount.load();
			m_collisionChecks = other.m_collisionChecks.load();
			m_boundingBoxCollisionChecks = other.m_boundingBoxCollisionChecks.load();
			m_collisions = other.m_collisions.load();

			m_fps = other.m_fps;
			m_tps = other.m_tps;
			m_frameTime = other.m_frameTime;
			m_eventTime = other.m_eventTime;
			m_updateTime = other.m_updateTime;
			m_drawTime = other.m_drawTime;

			m_deltaT = other.m_deltaT;
			m_fixedDeltaT = other.m_fixedDeltaT;
			m_elapsedTime = other.m_elapsedTime;

			m_tick = other.m_tick;
			return *this;
		}

		Stats Stats::getSmothed(const Stats& oldStats, double smothness)
		{
			Stats s(*this);
			double diff = 1 - smothness;
			s.m_fps = diff * m_fps + smothness * oldStats.m_fps;
			s.m_tps = diff * m_tps + smothness * oldStats.m_tps;
			s.m_frameTime = diff * m_frameTime + smothness * oldStats.m_frameTime;
			s.m_eventTime = diff * m_eventTime + smothness * oldStats.m_eventTime;
			s.m_updateTime = diff * m_updateTime + smothness * oldStats.m_updateTime;
			s.m_drawTime = diff * m_drawTime + smothness * oldStats.m_drawTime;

			s.m_deltaT = diff * m_deltaT + smothness * oldStats.m_deltaT;
			return s;
		}

		

		std::string Stats::toString() const
		{
			return	"Stats:\n"
				" Tick:                 " + std::to_string(m_tick) + "\n" +
				" Root Objects:         " + std::to_string(m_rootObjectsCount) + "\n" +
				" Objects:              " + std::to_string(m_objectsCount) + "\n" +
				" Components:           " + std::to_string(m_componentsCount) + "\n" +
				" Physics:\n" +
				"  Collision checks:    " + std::to_string(m_collisionChecks) + "\n" +
				"  AABB checks:         " + std::to_string(m_boundingBoxCollisionChecks) + "\n" +
				"  Collisions:          " + std::to_string(m_collisions) + "\n" +
				" Timing:\n"
				"  FPS:                 " + std::to_string(m_fps) + "\n" +
				"  TPS:                 " + std::to_string(m_tps) + "\n" +
				"  DeltaT:              " + std::to_string(m_deltaT * 1000) + " ms\n" +
				"  Elapsed time:        " + std::to_string(m_elapsedTime) + " s\n" +
				"  Elapsed fixed time:  " + std::to_string(m_fixedDeltaT * m_tick) + " s\n" +
				"  Frame time:          " + std::to_string(m_frameTime * 1000) + " ms\n" +
				"   Event  time:        " + std::to_string(m_eventTime * 1000) + " ms\n" +
				"   Update time:        " + std::to_string(m_updateTime * 1000) + " ms\n" +
				"   Draw   time:        " + std::to_string(m_drawTime * 1000) + " ms\n";

		}
#if IMGUI_SFML_LIBRARY_AVAILABLE == 1
		void Stats::drawImGui() const
		{
			// Save current ImGui style colors
			ImGuiStyle& style = ImGui::GetStyle();
			ImVec4 originalColor = style.Colors[ImGuiCol_WindowBg];

			// Set background transparency
			style.Colors[ImGuiCol_WindowBg].w = 0.5f;

			// Set the predefined start size of the window
			//ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);

			if (c_rollingPlot)
			{
				// Shift data
				memmove(m_fpsData.data(), m_fpsData.data() + 1, (c_dataSize - 1) * sizeof(double));
				memmove(m_tpsData.data(), m_tpsData.data() + 1, (c_dataSize - 1) * sizeof(double));
				//memmove(m_frameTimeData.data(), m_frameTimeData.data() + 1, (c_dataSize - 1) * sizeof(double));
				memmove(m_eventTimeData.data(), m_eventTimeData.data() + 1, (c_dataSize - 1) * sizeof(double));
				memmove(m_updateTimeData.data(), m_updateTimeData.data() + 1, (c_dataSize - 1) * sizeof(double));
				memmove(m_drawTimeData.data(), m_drawTimeData.data() + 1, (c_dataSize - 1) * sizeof(double));
				//memmove(m_deltaTData.data(), m_deltaTData.data() + 1, (c_dataSize - 1) * sizeof(double));
				//memmove(m_elapsedTimeData.data(), m_elapsedTimeData.data() + 1, (c_dataSize - 1) * sizeof(double));

				// Add new data
				m_fpsData[c_dataSize - 1] = m_fps;
				m_tpsData[c_dataSize - 1] = m_tps;
				//m_frameTimeData[c_dataSize - 1] = m_frameTime * 1000; // convert frame time to ms
				m_eventTimeData[c_dataSize - 1] = m_eventTime * 1000; // convert event time to ms
				m_updateTimeData[c_dataSize - 1] = m_updateTime * 1000; // convert update time to ms
				m_drawTimeData[c_dataSize - 1] = m_drawTime * 1000; // convert draw time to ms
				//m_deltaTData[c_dataSize - 1] = m_deltaT * 1000; // convert deltaT to ms
				//m_elapsedTimeData[c_dataSize - 1] = m_elapsedTime * 1000; // convert elapsed time to ms

			}
			else
			{

				// Update data arrays with new values
				m_fpsData[m_currentDataIndex] = m_fps;
				m_tpsData[m_currentDataIndex] = m_tps;
				//m_frameTimeData[m_currentDataIndex] = m_frameTime * 1000; // convert frame time to ms
				m_eventTimeData[m_currentDataIndex] = m_eventTime * 1000; // convert event time to ms
				m_updateTimeData[m_currentDataIndex] = m_updateTime * 1000; // convert update time to ms
				m_drawTimeData[m_currentDataIndex] = m_drawTime * 1000; // convert draw time to ms
				//m_deltaTData[m_currentDataIndex] = m_deltaT * 1000; // convert deltaT to ms
				//m_elapsedTimeData[m_currentDataIndex] = m_elapsedTime * 1000; // convert elapsed time to ms

				m_currentDataIndex = (m_currentDataIndex + 1) % c_dataSize; // Keep circular buffer
			}

			// Start drawing the window
			if (ImGui::Begin("Stats Window"))
			{
				ImGui::Text("Stats:");

				if (ImGui::TreeNodeEx("General", ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::Text(" Tick:                 %d", m_tick);
					ImGui::Text(" Root Objects:         %d", m_rootObjectsCount.load());
					ImGui::Text(" Objects:              %d", m_objectsCount.load());
					ImGui::Text(" Components:           %d", m_componentsCount.load());
					ImGui::TreePop();
				}

				if (ImGui::TreeNodeEx("Physics", ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::Text(" Collision checks:     %d", m_collisionChecks.load());
					ImGui::Text(" AABB checks:          %d", m_boundingBoxCollisionChecks.load());
					ImGui::Text(" Collisions:           %d", m_collisions.load());
					ImGui::TreePop();
				}

				if (ImGui::TreeNodeEx("Timing", ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::Text(" FPS:                  %f", m_fps);
					ImGui::Text(" TPS:                  %f", m_tps);
					ImGui::Text(" DeltaT:               %f ms", m_deltaT * 1000);
					ImGui::Text(" Elapsed time:         %f s", m_elapsedTime);
					ImGui::Text(" Elapsed fixed time:   %f s", m_fixedDeltaT * m_tick);

					if (ImGui::TreeNodeEx("Frame Timing", ImGuiTreeNodeFlags_DefaultOpen))
					{
						ImGui::Text(" Frame time:           %f ms", m_frameTime * 1000);
						ImGui::Text("  Event  time:         %f ms", m_eventTime * 1000);
						ImGui::Text("  Update time:         %f ms", m_updateTime * 1000);
						ImGui::Text("  Draw   time:         %f ms", m_drawTime * 1000);
						ImGui::TreePop();
					}

					ImGui::TreePop();
				}

#if IMPLOT_LIBRARY_AVAILABLE == 1
				// Begin a child window with dynamic width and fixed height
				ImGui::BeginChild("PlotWindow", ImVec2(0, ImGui::GetContentRegionAvail().y), true);

				// Adjust plot dimensions
				ImVec2 plotSize = ImGui::GetContentRegionAvail();
				plotSize.y *= 0.5f;

				// Plotting using ImPlot if available
				if (ImPlot::BeginPlot("Performance Metrics", plotSize))
				{
					ImPlot::SetupAxesLimits(0, c_dataSize - 1, 0, 100, ImPlotCond_Once); // 0 for Y means auto-scaling
					ImPlot::SetupAxes("Frame", "Value");
					ImPlot::SetupAxes("Y-Axis", NULL, ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoTickLabels);
					// Set the y-axis to automatically fit the data
					//ImPlot::SetupAxis(ImAxis_Y1, NULL, ImPlotAxisFlags_AutoFit);

					// Plot FPS
					ImPlot::PlotLine("FPS", m_fpsData.data(), c_dataSize, c_dataSize, 0, sizeof(float));

					// Plot TPS
					ImPlot::PlotLine("TPS", m_tpsData.data(), c_dataSize, c_dataSize, 0, sizeof(float));

					ImPlot::EndPlot();
				}
			//ImGui::EndChild();
				//ImGui::BeginChild("PlotWindow", ImVec2(0, 200), true);

				// Adjust plot dimensions
				//plotSize = ImGui::GetContentRegionAvail();
				// Plotting event, update and draw time
				if (ImPlot::BeginPlot("Frame Timing", plotSize))
				{
					// Set fixed X-axis limits (e.g., 0 to 99)
					ImPlot::SetupAxesLimits(0, c_dataSize-1, 0, 100, ImPlotCond_Once); // 0 for Y means auto-scaling
					ImPlot::SetupAxes("", "Time (ms)");
					// Setup Y-axis (auto-scaling)
					ImPlot::SetupAxes("Y-Axis", NULL, ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoTickLabels);


					// Plot Event Time
					ImPlot::PlotLine("Event Time (ms)", m_eventTimeData.data(), c_dataSize, c_dataSize, 0, sizeof(float));

					// Plot Update Time
					ImPlot::PlotLine("Update Time (ms)", m_updateTimeData.data(), c_dataSize, c_dataSize, 0, sizeof(float));

					// Plot Draw Time
					ImPlot::PlotLine("Draw Time (ms)", m_drawTimeData.data(), c_dataSize, c_dataSize, 0, sizeof(float));

					ImPlot::EndPlot();
				}
				ImGui::EndChild();
#endif
			}
			ImGui::End();

			// Restore original background color
			style.Colors[ImGuiCol_WindowBg] = originalColor;
		}
#endif
		void Stats::print() const
		{
			qDebug() << toString().c_str();
		}
		void Stats::resetFrame_synced(Stats& copyTo)
		{
			copyTo.m_frameTime = m_frameTime;
			m_frameTime = 0;
		}
		void Stats::resetFrame_eventloop(Stats& copyTo)
		{
			copyTo.m_eventTime = m_eventTime;
			m_eventTime = 0;
		}
		void Stats::resetFrame_updateLoop(Stats& copyTo)
		{
			copyTo.m_updateTime = m_updateTime;
			copyTo.m_collisionChecks.store(m_collisionChecks);
			copyTo.m_boundingBoxCollisionChecks.store(m_boundingBoxCollisionChecks);
			copyTo.m_collisions.store(m_collisions);
			copyTo.m_tick = m_tick;
			copyTo.m_deltaT = m_deltaT;
			copyTo.m_fixedDeltaT = m_fixedDeltaT;
			copyTo.m_elapsedTime = m_elapsedTime;
			copyTo.m_tps = m_tps;

			m_updateTime = 0;
			m_collisionChecks = 0;
			m_boundingBoxCollisionChecks = 0;
			m_collisions = 0;
		}
		void Stats::resetFrame_paintLoop(Stats& copyTo)
		{
			copyTo.m_fps = m_fps;
			copyTo.m_drawTime = m_drawTime;

			//m_fps = 0;
			m_drawTime = 0;
		}
		void Stats::copyObjectCounts(const Stats& from)
		{
			m_rootObjectsCount = from.m_rootObjectsCount.load();
			m_objectsCount = from.m_objectsCount.load();
			m_componentsCount = from.m_componentsCount.load();
		}
		void Stats::resetTiming()
		{
			//m_fps = 0;
			m_frameTime = 0;
			m_eventTime = 0;
			m_updateTime = 0;
			m_drawTime = 0;

			m_deltaT = 0;
			m_elapsedTime = 0;
		}
#if IMPLOT_LIBRARY_AVAILABLE == 1
		void Stats::clearAllPlotBuffer() const
		{
			memset(m_fpsData.data(), 0, c_dataSize * sizeof(double));
			memset(m_tpsData.data(), 0, c_dataSize * sizeof(double));
			//memset(m_frameTimeData.data(), 0, c_dataSize * sizeof(double));
			memset(m_eventTimeData.data(), 0, c_dataSize * sizeof(double));
			memset(m_updateTimeData.data(), 0, c_dataSize * sizeof(double));
			memset(m_drawTimeData.data(), 0, c_dataSize * sizeof(double));
			//memset(m_deltaTData.data(), 0, c_dataSize * sizeof(double));
			//memset(m_elapsedTimeData.data(), 0, c_dataSize * sizeof(double));
		}
#endif
	}
}
