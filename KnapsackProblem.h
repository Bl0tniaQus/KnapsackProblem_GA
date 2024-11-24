#pragma once
#include <vector>
#include <random>
#include <stdexcept>
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
