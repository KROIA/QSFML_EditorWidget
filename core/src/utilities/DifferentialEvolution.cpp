#include "utilities/DifferentialEvolution.h"
#include <memory>

namespace QSFML
{
	namespace Utilities
	{
		DifferentialEvolution::Individual::Individual(size_t parameterCount)
		{
			this->parameterCount = parameterCount;
			parameters = new double[parameterCount];
			for (size_t i = 0; i < parameterCount; ++i)
				parameters[i] = 0;
		}
		DifferentialEvolution::Individual::Individual(const Individual& other)
		{
			parameterCount = other.parameterCount;
			parameters = new double[other.parameterCount];
			fitness = other.fitness;
			memcpy(parameters, other.parameters, sizeof(double) * other.parameterCount);
		}
		DifferentialEvolution::Individual::Individual(Individual&& other) noexcept
		{
			parameterCount = other.parameterCount;
			parameters = other.parameters;
			fitness = other.fitness;
			other.parameters = nullptr;
		}
		DifferentialEvolution::Individual::~Individual()
		{
			delete[] parameters;
		}

		DifferentialEvolution::Individual& DifferentialEvolution::Individual::operator=(const Individual& other)
		{
			if (this != &other)
			{
				if (parameterCount != other.parameterCount)
				{
					delete[] parameters;
					parameterCount = other.parameterCount;
					parameters = new double[parameterCount];
				}
				fitness = other.fitness;
				memcpy(parameters, other.parameters, sizeof(double) * other.parameterCount);
			}
			return *this;
		}
		DifferentialEvolution::Individual& DifferentialEvolution::Individual::operator=(Individual&& other) noexcept
		{
			if (this != &other)
			{
				delete[] parameters;
				parameterCount = other.parameterCount;
				parameters = other.parameters;
				fitness = other.fitness;
				other.parameters = nullptr;
			}
			return *this;
		}
		bool DifferentialEvolution::Individual::operator==(const Individual& other) const
		{
			if (parameterCount != other.parameterCount)
				return false;
			for (size_t i = 0; i < parameterCount; ++i)
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
		}

		void DifferentialEvolution::setPopulationSize(size_t size)
		{
			m_populationSize = size;
			if (m_populationSize < MINIMUM_POPULATION_SIZE)
			{
				m_populationSize = MINIMUM_POPULATION_SIZE;  // Ensure at least one individual in the population
			}
			m_population.resize(m_populationSize, Individual(m_parameterCount));
		}

		const DifferentialEvolution::Individual &DifferentialEvolution::getBestIndividual() const
		{
			size_t bestIndex = -1;
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
		void DifferentialEvolution::setPopulation(const std::vector<Individual>& population)
		{
			m_population = population;
			m_populationSize = population.size();
			if (m_populationSize < MINIMUM_POPULATION_SIZE)
			{
				m_populationSize = MINIMUM_POPULATION_SIZE;  // Ensure at least one individual in the population
				m_population.resize(m_populationSize, Individual(m_parameterCount));
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
		
			std::vector<Individual> newPopulation(m_populationSize, Individual(m_parameterCount));
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

				// Selection
				trial.fitness = m_fitnessFunction(std::vector<double>(trial.parameters, trial.parameters + m_parameterCount));
				if (trial.fitness > m_population[i].fitness)
				{
					newPopulation[i] = trial;
				}
				else
				{
					newPopulation[i] = m_population[i];
				}
			}
			m_population = std::move(newPopulation);
			++m_currentGeneration;
		}
	}
}