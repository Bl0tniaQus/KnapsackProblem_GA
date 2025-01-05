#include "data_loader.h"
#include "KnapsackProblem.h"

int main(int argc, char *argv[])
{
    if (argc==1) {
        std::cout<<"Please provide source data file as an argument."<<std::endl;
        std::cout<<"example: ./main.out data.txt"<<std::endl;
        return -1;

    }
    std::default_random_engine generator = std::default_random_engine(time(0));
    int population_size = 100;
    int elite_count = 10;
    int generation_count = 100;
    KnapsackData data = loadKnapsackData(std::string(argv[1]));
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
    std::vector<double> averages = {};
    calculate(&alg, generation_count, &averages);
    double max = getMaxValue(&alg);
    std::vector<std::vector<int>> uniqueBest = getItemNumbers(getDifferentBestValues(&alg));

    std::cout<<"Best value: "<<max<<std::endl;
    std::cout<<"Number of unique combinations that give best value: "<<uniqueBest.size()<<std::endl;
    for (int i = 0; i<uniqueBest.size();i++)
    {
        std::cout<< "x = [";
        for (int j = 0; j<uniqueBest[i].size();j++)
        {
            if (j<uniqueBest[i].size()-1) std::cout<<uniqueBest[i][j]<<", ";
            if (j==uniqueBest[i].size()-1)std::cout<<uniqueBest[i][j];
        }
        for (int x : uniqueBest[i])
            {

            }
        std::cout<<"]";
        std::cout<<std::endl;
    }
    saveAverages(averages);
    return 0;
}
