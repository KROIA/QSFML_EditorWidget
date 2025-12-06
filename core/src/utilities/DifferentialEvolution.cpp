#include "utilities/DifferentialEvolution.h"
#include <memory>

namespace QSFML
{
	namespace Utilities
	{
		DifferentialEvolution::Individual::Individual(size_t parameterCount)
			: parameters(parameterCount, 0)
		{
		}
		DifferentialEvolution::Individual::Individual(const Individual& other)
			: parameters(other.parameters)
		{
			fitness = other.fitness;
		}
		DifferentialEvolution::Individual::Individual(Individual&& other) noexcept
			: parameters(std::move(other.parameters))
		{
			fitness = other.fitness;
		}
		DifferentialEvolution::Individual::~Individual()
		{
			
		}

		DifferentialEvolution::Individual& DifferentialEvolution::Individual::operator=(const Individual& other)
		{
			if (this != &other)
			{
				if (parameters.size() != other.parameters.size())
				{
					parameters.resize(other.parameters.size());
				}
				fitness = other.fitness;
				std::copy(other.parameters.begin(), other.parameters.end(), parameters.begin());
			}
			return *this;
		}
		DifferentialEvolution::Individual& DifferentialEvolution::Individual::operator=(Individual&& other) noexcept
		{
			if (this != &other)
			{
				parameters = std::move(other.parameters);
				fitness = other.fitness;
			}
			return *this;
		}
		bool DifferentialEvolution::Individual::operator==(const Individual& other) const
		{
			if (parameters.size() != other.parameters.size())
				return false;
			for (size_t i = 0; i < parameters.size(); ++i)
			{
				if (parameters[i] != other.parameters[i])
					return false;
			}
			return true;
		}


		DifferentialEvolution::DifferentialEvolution(size_t parameterCount)
			: m_parameterCount(parameterCount)
		{
			
		}
		DifferentialEvolution::DifferentialEvolution(size_t populationSize, size_t parameterCount,
			double mutationFactor, double crossoverRate,
			size_t maxGeneration)
			: m_populationSize(populationSize)
			, m_parameterCount(parameterCount)
			, m_mutationFactor(mutationFactor)
			, m_crossoverRate(crossoverRate)
			, m_maxGenerations(maxGeneration)
		{
			if (populationSize < MINIMUM_POPULATION_SIZE)
			{
				populationSize = MINIMUM_POPULATION_SIZE;  // Ensure at least one individual in the population
			}
			m_population.resize(populationSize, Individual(parameterCount));
		}
		DifferentialEvolution::~DifferentialEvolution()
		{
			m_population.clear();
			if (m_useThreadPool)
			{
				shutdownThreadPool();
			}
		}

		bool DifferentialEvolution::enableThreadPool(bool enable, size_t numThreads)
		{
			if (enable && !m_useThreadPool)
			{
				initializeThreadPool(numThreads);
				m_useThreadPool = true;
				return true;
			}
			else if (!enable && m_useThreadPool)
			{
				shutdownThreadPool();
				m_useThreadPool = false;
				return true;
			}
			return false;
		}
		void DifferentialEvolution::setPopulationSize(size_t size)
		{
			if (m_populationSize == size)
				return;
			m_populationSize = size;
			if (m_populationSize < MINIMUM_POPULATION_SIZE)
			{
				m_populationSize = MINIMUM_POPULATION_SIZE;  // Ensure at least one individual in the population
			}
			m_population.resize(m_populationSize, Individual(m_parameterCount));
			m_currentGeneration = 0;
			if (m_useThreadPool)
			{
				size_t threadCount = m_workerThreads.size();
				shutdownThreadPool();
				initializeThreadPool(threadCount);
			}
		}

		const DifferentialEvolution::Individual &DifferentialEvolution::getBestIndividual() const
		{
			size_t bestIndex = 0;
			switch (m_optimizingDirection)
			{
				case OptimizingDirection::Minimize:
				{
					double bestFitness = std::numeric_limits<double>::max();
					for (size_t i = 0; i < m_population.size(); ++i)
					{
						const double& fitness = m_population[i].fitness;
						if (fitness < bestFitness)
						{
							bestIndex = i;
							bestFitness = fitness;
						}
					}
					return m_population[bestIndex];
				}
				case OptimizingDirection::Maximize:
				{
					double bestFitness = -std::numeric_limits<double>::max();
					for (size_t i = 0; i < m_population.size(); ++i)
					{
						const double& fitness = m_population[i].fitness;
						if (fitness > bestFitness)
						{
							bestIndex = i;
							bestFitness = fitness;
						}
					}
					return m_population[bestIndex];
				}
			}
			return m_population[bestIndex];
		}
		void DifferentialEvolution::setPopulation(const std::vector<Individual>& population)
		{
			bool sizeChanged = (m_populationSize != population.size());
			m_population = population;
			m_populationSize = population.size();
			if(population.size() > 0)
				m_parameterCount = population[0].parameters.size();
			if (m_populationSize < MINIMUM_POPULATION_SIZE)
			{
				m_populationSize = MINIMUM_POPULATION_SIZE;  // Ensure at least one individual in the population
				m_population.resize(m_populationSize, Individual(m_parameterCount));
			}
			m_currentGeneration = 0;
			if (m_useThreadPool && sizeChanged)
			{
				size_t threadCount = m_workerThreads.size();
				shutdownThreadPool();
				initializeThreadPool(threadCount);
			}
		}

		void DifferentialEvolution::resetPopulation()
		{
			if (m_individualInitializer)
			{
				m_currentGeneration = 0;
				for (auto &it : m_population)
					m_individualInitializer(it);
			}
		}
		void DifferentialEvolution::evolve()
		{
			if (!m_fitnessFunction || m_currentGeneration >= m_maxGenerations)
				return; // No fitness function defined or max generations reached

			if(m_currentGeneration == 0)
			{
				// Evaluate initial population
				size_t idx = 0;
				for (auto &it : m_population)
				{
					it.fitness = m_fitnessFunction(it.parameters, idx);
					++idx;
				}
			}
		
			std::vector<Individual> newPopulation(m_populationSize, Individual(m_parameterCount));
			m_threadPopulation.clear();
			m_threadPopulation.reserve(m_populationSize);
			for (size_t i = 0; i < m_populationSize; ++i)
			{
				// Mutation
				size_t a, b, c;
				do { a = rand() % m_populationSize; } while (a == i);
				do { b = rand() % m_populationSize; } while (b == i || b == a);
				do { c = rand() % m_populationSize; } while (c == i || c == a || c == b);

				Individual mutant(m_parameterCount);
				for (size_t j = 0; j < m_parameterCount; ++j)
				{
					mutant.parameters[j] = m_population[a].parameters[j] +
						m_mutationFactor * (m_population[b].parameters[j] - m_population[c].parameters[j]);
				}

				// Crossover
				Individual trial(m_parameterCount);
				size_t R = rand() % m_parameterCount; // Ensure at least one parameter from mutant
				for (size_t j = 0; j < m_parameterCount; ++j)
				{
					if (rand() / double(RAND_MAX) < m_crossoverRate || j == R)
					{
						trial.parameters[j] = mutant.parameters[j];
					}
					else
					{
						trial.parameters[j] = m_population[i].parameters[j];
					}
				}
				m_threadPopulation.emplace_back(std::move(trial));
			}

			// Evaluate fitness in parallel if thread pool is used
			if (m_useThreadPool)
			{
				m_threadsBusy = true;
				m_threadPopulation.clear();
				m_threadPopulation.reserve(m_populationSize);
				for (size_t i = 0; i < m_populationSize; ++i)
				{
					// Mutation
					size_t a, b, c;
					do { a = rand() % m_populationSize; } while (a == i);
					do { b = rand() % m_populationSize; } while (b == i || b == a);
					do { c = rand() % m_populationSize; } while (c == i || c == a || c == b);

					Individual mutant(m_parameterCount);
					for (size_t j = 0; j < m_parameterCount; ++j)
					{
						mutant.parameters[j] = m_population[a].parameters[j] +
							m_mutationFactor * (m_population[b].parameters[j] - m_population[c].parameters[j]);
					}

					// Crossover
					Individual trial(m_parameterCount);
					size_t R = rand() % m_parameterCount; // Ensure at least one parameter from mutant
					for (size_t j = 0; j < m_parameterCount; ++j)
					{
						if (rand() / double(RAND_MAX) < m_crossoverRate || j == R)
						{
							trial.parameters[j] = mutant.parameters[j];
						}
						else
						{
							trial.parameters[j] = m_population[i].parameters[j];
						}
					}
					m_threadPopulation.emplace_back(std::move(trial));
				}
				for (size_t i = 0; i < m_workerThreads.size(); ++i)
				{
					m_threadHasWork[i].store(true);
				}
				// Wake up all worker threads
				QSFMLP_GENERAL_NONSCOPED_BLOCK("Notify Worker Threads", QSFML_COLOR_STAGE_2);
				m_cvWork.notify_all();
				QSFMLP_GENERAL_END_BLOCK;

				// Wait for all threads to complete their work
				{
					QSFMLP_GENERAL_NONSCOPED_BLOCK("WaitForFinishedWork", QSFML_COLOR_STAGE_2);
					std::unique_lock<std::mutex> lock(m_mutex);
					m_cvComplete.wait(lock, [this] {
						for (size_t i = 0; i < m_workerThreads.size(); ++i)
						{
							if (m_threadHasWork[i].load())
								return false;
						}
						return true;
						});
					QSFMLP_GENERAL_END_BLOCK;
					//	qDebug() << "All threads completed work.";
					QSFMLP_GENERAL_NONSCOPED_BLOCK("Notify Worker Threads", QSFML_COLOR_STAGE_2);
					m_cvWork.notify_all();
					QSFMLP_GENERAL_END_BLOCK;
				}

				

				for (size_t i = 0; i < m_populationSize; ++i)
				{

					// Selection
					Individual& trial = m_threadPopulation[i];
					switch (m_optimizingDirection)
					{
						case OptimizingDirection::Minimize:
						{
							if (trial.fitness < m_population[i].fitness)
							{
								newPopulation[i] = trial;
							}
							else
							{
								newPopulation[i] = m_population[i];
							}
							continue;
						}
						case OptimizingDirection::Maximize:
						{
							if (trial.fitness > m_population[i].fitness)
							{
								newPopulation[i] = trial;
							}
							else
							{
								newPopulation[i] = m_population[i];
							}
							break;
						}
					}
				}
				m_threadsBusy = false;
			}
			else
			{
				for (size_t i = 0; i < m_populationSize; ++i)
				{
					// Mutation
					size_t a, b, c;
					do { a = rand() % m_populationSize; } while (a == i);
					do { b = rand() % m_populationSize; } while (b == i || b == a);
					do { c = rand() % m_populationSize; } while (c == i || c == a || c == b);

					Individual mutant(m_parameterCount);
					for (size_t j = 0; j < m_parameterCount; ++j)
					{
						mutant.parameters[j] = m_population[a].parameters[j] +
							m_mutationFactor * (m_population[b].parameters[j] - m_population[c].parameters[j]);
					}

					// Crossover
					Individual trial(m_parameterCount);
					size_t R = rand() % m_parameterCount; // Ensure at least one parameter from mutant
					for (size_t j = 0; j < m_parameterCount; ++j)
					{
						if (rand() / double(RAND_MAX) < m_crossoverRate || j == R)
						{
							trial.parameters[j] = mutant.parameters[j];
						}
						else
						{
							trial.parameters[j] = m_population[i].parameters[j];
						}
					}
					trial.fitness = m_fitnessFunction(trial.parameters, i);

					switch (m_optimizingDirection)
					{
						case OptimizingDirection::Minimize:
						{
							if (trial.fitness < m_population[i].fitness)
							{
								newPopulation[i] = trial;
							}
							else
							{
								newPopulation[i] = m_population[i];
							}
							continue;
						}
						case OptimizingDirection::Maximize:
						{
							if (trial.fitness > m_population[i].fitness)
							{
								newPopulation[i] = trial;
							}
							else
							{
								newPopulation[i] = m_population[i];
							}
							break;
						}
					}
				}
			}
			m_population = std::move(newPopulation);
			++m_currentGeneration;
		}



		void DifferentialEvolution::initializeThreadPool(size_t numThreads)
		{
			if (m_workerThreads.size() != 0)
			{
				shutdownThreadPool();
			}
			m_stopThreads = false;
			//m_hasWork = 0;
			m_workerThreads.reserve(numThreads);

			// Calculate fixed ranges for each thread
			size_t populationSize = m_population.size();
			numThreads = std::min(numThreads, populationSize);
			numThreads = std::min(numThreads, s_maxThreadWorkerCount);
			size_t chunkSize = (populationSize + numThreads - 1) / numThreads;

			m_populationSizeOnSetup = populationSize;

			m_threadHasWork = new std::atomic<bool>[numThreads];

			for (unsigned int i = 0; i < numThreads; ++i)
			{
				size_t start = i * chunkSize;
				size_t end = std::min(start + chunkSize, populationSize);

				m_threadHasWork[i].store(false);
				m_workerThreads.emplace_back(&DifferentialEvolution::workerThread, this, i, start, end);
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(30)); // Give some time for threads to go to sleep
			qDebug() << "GeneticSolver: Initialized thread pool with " << numThreads << " threads.";
		}

		void DifferentialEvolution::workerThread(size_t threadId, size_t start, size_t end)
		{
			QSFML_PROFILING_THREAD("GeneticSolver Worker Thread");
			//bool isLast = false;
			std::atomic<bool>& threadHasWork = m_threadHasWork[threadId];
			while (true)
			{
				std::vector<Individual*> localAgents;
				{
					QSFMLP_GENERAL_BLOCK("GeneticSolver Worker Thread Wait", QSFML_COLOR_STAGE_1);
					std::unique_lock<std::mutex> lock(m_mutex);
					// Wait for work or shutdown signal

					//if (!isLast)
					//{
					//	m_cvWork.wait(lock, [this] {
					//		return !threadHasWork || m_stopThreads;
					//		});
					//}
					//isLast = false;

					//qDebug() << "GeneticSolver: Worker thread " << threadId << " waiting for work...";
					// Wait for work or shutdown signal
					m_cvWork.wait(lock, [this, &threadHasWork] {
						return m_stopThreads || threadHasWork.load();
						});

					//qDebug() << "GeneticSolver: Worker thread " << threadId << " woke up.";
					if (m_stopThreads)
					{
						qDebug() << "GeneticSolver: Worker thread " << threadId << " stopping.";
						return;
					}
				}
				for (size_t i = start; i < end && i < m_threadPopulation.size(); ++i)
				{
					localAgents.push_back(&m_threadPopulation[i]);
				}

				// Process the fixed range assigned to this thread
				QSFMLP_GENERAL_NONSCOPED_BLOCK("Agents Test", QSFML_COLOR_STAGE_1);
				for (size_t i = 0; i < localAgents.size(); ++i)
				{
					auto& agent = localAgents[i];
					agent->fitness = m_fitnessFunction(agent->parameters, start + i);
				}
				QSFMLP_GENERAL_END_BLOCK;

				if (m_stopThreads)
				{
					return;
				}

				// Signal completion
				QSFMLP_GENERAL_NONSCOPED_BLOCK("Notify completion", QSFML_COLOR_STAGE_1);
				//size_t completed = ++m_completedThreads;
				//if (completed == m_workerThreads.size())
				//qDebug() << "GeneticSolver: Worker thread " << threadId << " completed work.";
				{
					std::lock_guard<std::mutex> lock(m_mutex);
					threadHasWork = false;
					m_cvComplete.notify_one();
				}
				QSFMLP_GENERAL_END_BLOCK;

			}
			qDebug() << "GeneticSolver: Worker thread " << threadId << " stopping2.";
		}
		void DifferentialEvolution::shutdownThreadPool()
		{
			qDebug() << "GeneticSolver: Shutting down thread pool...";
			{
				std::lock_guard<std::mutex> lock(m_mutex);
				m_stopThreads = true;
			}
			m_cvWork.notify_all();

			for (auto& thread : m_workerThreads)
			{
				if (thread.joinable())
					thread.join();
			}
			qDebug() << "GeneticSolver: Thread pool shut down.";
			m_workerThreads.clear();

			delete[] m_threadHasWork;
			m_threadHasWork = nullptr;
		}
	}	
}