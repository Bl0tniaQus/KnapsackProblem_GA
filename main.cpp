#include "data_loader.h"
#include "KnapsackProblem.h"
int main(int argc, char *argv[])
{
    std::default_random_engine generator = std::default_random_engine(time(0));

    int population_size = 100;
    int elite_count = 10;
    int generation_count = 1;
    KnapsackData data = loadKnapsackData("dane/dane1.txt");
    KnapsackProblem problem(&data, 0.01, 0.5);
    int n_items = data.items.size();
    std::vector<std::vector<bool>> initial_population;
    std::vector<bool> v = generateRandomIndividual(n_items, &generator);
    for (int i=0;i<population_size;i++)
        {
            initial_population.push_back(generateRandomIndividual(n_items, &generator));
        }

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
