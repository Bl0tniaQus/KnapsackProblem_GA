#include <stdexcept>
#include "ga/algorithm.hpp"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>
#include <random>
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

class KnapsackProblem
{
public:
  using individual_type = std::vector<bool>;
  using generator_type = std::default_random_engine;
  using fitness_type = double;

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
    std::vector<double> weights = {1,2,3,4,5};
    std::vector<double> values = {4,1,2,5,3};
    double capacity = 6;
    int n_items = 5;
    double mutation_rate = 0.01;
    double recombination_rate = 0.5;
};



int main()
{
    std::default_random_engine generator = std::default_random_engine(time(0));
    int population_size = 100;
    int n_items = 5;
    int elite_count = 10;
    int generation_count = 1;
    std::vector<std::vector<bool>> initial_population;
    std::vector<bool> v = generateRandomIndividual(n_items, &generator);
  for (int i=0;i<population_size;i++)
  {
      initial_population.push_back(generateRandomIndividual(n_items, &generator));
  }
  KnapsackProblem problem;
  ga::algorithm<KnapsackProblem> alg = ga::make_algorithm(std::move(problem), std::move(initial_population),
                                      elite_count, std::move(generator));
for (int i = 0; i < generation_count; i++) {alg.iterate();}
for (const ga::solution<std::vector<bool>, double>& solution : alg.population())
{
     std::cout<< "x = ";
     for (auto gene : solution.x)
     {
         std::cout<<gene; 
     }
        std::cout << "\tf(x) = ";
    std::cout << ' ' << -solution.fitness;
    std::cout << "\n";
    }
  
}
