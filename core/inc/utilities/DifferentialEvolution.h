#pragma once
#include "QSFML_EditorWidget_base.h"
#include <vector>
#include <functional>

namespace QSFML
{
	namespace Utilities
	{
		class QSFML_EDITOR_WIDGET_API DifferentialEvolution
		{
		public:
			struct Individual
			{
				std::vector<double> parameters;
				double fitness = 0;

				Individual(size_t parameterCount);
				Individual(const Individual& other);
				Individual(Individual&& other) noexcept;
				~Individual();

				Individual& operator=(const Individual& other);
				Individual& operator=(Individual&& other) noexcept;

				double& operator[](size_t index)
				{
					return parameters[index];
				}
				const double& operator[](size_t index) const
				{
					return parameters[index];
				}

				bool operator<(const Individual& other) const
				{
					return fitness < other.fitness;
				}
				bool operator>(const Individual& other) const
				{
					return fitness > other.fitness;
				}
				bool operator<=(const Individual& other) const
				{
					return fitness <= other.fitness;
				}
				bool operator>=(const Individual& other) const
				{
					return fitness >= other.fitness;
				}

				bool operator==(const Individual& other) const;
				bool operator!=(const Individual& other) const
				{
					return !(*this == other);
				}


			};

			static constexpr size_t MINIMUM_POPULATION_SIZE = 4;

			DifferentialEvolution(size_t parameterCount = 0);
			DifferentialEvolution(size_t populationSize, size_t parameterCount, 
								  double mutationFactor, double crossoverRate,
								  size_t maxGeneration);
			~DifferentialEvolution();

			void setPopulationSize(size_t size);
			size_t getPopulationSize() const { return m_populationSize; }
			size_t getParameterCount() const { return m_parameterCount; }

			void setMutationFactor(double factor) { m_mutationFactor = factor; }
			double getMutationFactor() const { return m_mutationFactor; }
			void setCrossoverRate(double rate) { m_crossoverRate = rate; }
			double getCrossoverRate() const { return m_crossoverRate; }
			void setMaxGenerations(size_t maxGen) { m_maxGenerations = maxGen; }
			size_t getMaxGenerations() const { return m_maxGenerations; }
			size_t getCurrentGeneration() const { return m_currentGeneration; }

			void setFitnessFunction(const std::function<double(const std::vector<double>&)>& func)
			{
				m_fitnessFunction = func;
			}
			const std::vector<Individual>& getPopulation() const { return m_population; }
			void setIndividualInitializer(const std::function<void(Individual&)>& func)
			{
				m_individualInitializer = func;
			}
			const Individual &getBestIndividual() const;
			void setPopulation(const std::vector<Individual>& population);

			void resetPopulation();
			void evolve();


		private:
			

			
			size_t m_populationSize;
			size_t m_parameterCount;
			double m_mutationFactor = 0.8;
			double m_crossoverRate = 0.7;
			size_t m_maxGenerations = 1000;
			std::vector<Individual> m_population;
			std::function<double(const std::vector<double>&)> m_fitnessFunction;
			std::function<void(Individual&)> m_individualInitializer;

			size_t m_currentGeneration = 0;
		};
	}
}