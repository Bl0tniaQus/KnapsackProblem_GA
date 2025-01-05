#pragma once
#include <vector>
#include <random>
#include <stdexcept>
#include <fstream>
#include "ga/algorithm.hpp"
#include "data_loader.h"
class KnapsackProblem
{
public:
  using individual_type = std::vector<bool>;
  using generator_type = std::default_random_engine;
  using fitness_type = double;
  KnapsackProblem(KnapsackData* data, double mutation_rate, double recombination_rate)
    {
        weights = getWeights(data);
        values = getValues(data);
        n_items = data->items.size();
        capacity = data->capacity;
        this->mutation_rate = mutation_rate;
        this->recombination_rate = recombination_rate;
    }
  auto evaluate(const individual_type& x, generator_type&) const -> double
  {
      double res = 0;
      double weight = 0;
      for (int i=0;i<n_items;i++)
      {
          res = res + values[i] * x[i];
          weight = weight + weights[i] * x[i];
      }
      if (weight > capacity) {res = 0;}
      return -res;
  }

  auto mutate(individual_type& x, generator_type& g) const -> void
  {
    if (ga::draw(mutation_rate, g))
    {
        for (int i=0;i<n_items;i++)
        {
            x[i] = !x[i];
        }
    }
  }

  auto recombine(const individual_type& a, const individual_type& b,
                 generator_type& g) const -> std::array<individual_type, 2>
  {
    std::array<individual_type,2> new_individuals;

    if (ga::draw(recombination_rate, g))
    {
        new_individuals[0] = std::vector<bool>();
        new_individuals[1] = std::vector<bool>();
        for (int i=0;i<n_items;i++)
        {
            if (i%2==0)
            {
                new_individuals[0].push_back(a[i]);
                new_individuals[1].push_back(b[i]);
                }
            else
            {
                new_individuals[1].push_back(a[i]);
                new_individuals[0].push_back(b[i]);
            }
        }
    }
    else
    {
        new_individuals[0] = a;
        new_individuals[1] = b;
        }


    return new_individuals;
  };

  private:
    std::vector<double> weights;
    std::vector<double> values;
    double capacity;
    int n_items;
    double mutation_rate;
    double recombination_rate;
};
std::vector<bool> generateRandomIndividual(int n, std::default_random_engine* generator)
{
    std::vector<bool> individual(n, false);
    std::default_random_engine &gen = *generator;
    for (int i=0; i<n; i++)
    {
            individual[i] = gen()%2==0;
    }
    return individual;
}

double getPopulationAverage(ga::algorithm<KnapsackProblem>* alg)
{
    double sum = 0;
    for (const ga::solution<std::vector<bool>, double>& solution : alg->population())
    {
        sum += solution.fitness;
    }
    return -sum / alg->population().size();
}
void calculate(ga::algorithm<KnapsackProblem>* alg, int n_generations, std::vector<double>* averages)
{
    averages->push_back(getPopulationAverage(alg));
    for (int i=0; i<n_generations; i++)
    {
        alg->iterate();
        averages->push_back(getPopulationAverage(alg));
    }
}
double getMaxValue(ga::algorithm<KnapsackProblem>* alg)
{
    double max = 0;
    for (const ga::solution<std::vector<bool>, double>& solution : alg->population())
    {
        if (-solution.fitness > max)
        {
            max = -solution.fitness;
        }
    }
    return max;
}
std::vector<std::vector<bool>> getDifferentBestValues(ga::algorithm<KnapsackProblem>* alg)
{
    std::vector<std::vector<bool>> result = {};
    double max = getMaxValue(alg);

    for (const ga::solution<std::vector<bool>, double>& solution : alg->population())
    {
        if (-solution.fitness!=max) continue;
        else
        {
            if (result.size() == 0) {result.push_back(solution.x);}
            else
            {
                bool unique = true;
                    for (int i = 0; i<result.size(); i++)
                    {

                        if (result[i] == solution.x) {unique = false; break;}
                    }
                    if (unique)
                    {
                        result.push_back(solution.x);
                    }
            }
        }

    }
    return result;
}
std::vector<std::vector<int>> getItemNumbers(std::vector<std::vector<bool>> results)
{
    std::vector<std::vector<int>> numberResults = {};
    for (int i = 0; i<results.size(); i++)
    {
        std::vector<int> res = {};
        for (int j = 0; j<results[i].size(); j++)
        {
            if (results[i][j]) res.push_back(j+1);
        }
        numberResults.push_back(res);
    }
    return numberResults;
}
std::string saveAverages(std::vector<double> averages, std::string dataFile)
{
    std::string filename = "results_" + std::to_string(time(NULL)) + ".txt";
    std::fstream file;
    file.open(filename, std::fstream::out);
    std::string str = "";
    file << dataFile+"\n";
    for (int i = 0; i<averages.size(); i++)
    {
        str = str + std::to_string(averages[i]) + "\n";
    }
    file << str;
    file.close();
    return filename;
}

