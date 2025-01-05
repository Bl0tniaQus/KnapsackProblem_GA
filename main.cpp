#include "data_loader.h"
#include "KnapsackProblem.h"

int population_size = 100;
int elite_count = 10;
int generation_count = 100;
float mutation_rate = 0.01;
float recombination_rate = 0.5;

int main(int argc, char *argv[])
{
    std::string fileName = "";
    if (argc==1) {
        std::cout<<"Please specify data source."<<std::endl;
        std::cout<<"Use -h for more info."<<std::endl;
        return -1;
    }
    if (argc>1)
    {
        for (int i = 1; i<argc; i++)
        {
            if (std::string(argv[i]) == "-h")
            {
                std::cout<<"-h help"<<std::endl;
                std::cout<<"-f <file_name> data source"<<std::endl;
                std::cout<<"-g <int> generation count (default = 100)"<<std::endl;
                std::cout<<"-p <int> population size (default = 100)"<<std::endl;
                std::cout<<"-e <int> elite count (number of best individuals to preserve in each generation) mus be less than population size (default = 10)"<<std::endl;
                std::cout<<"-m <float> mutation rate <0;1> (default = 0.01)"<<std::endl;
                std::cout<<"-r <float> recombination rate <0;1> (default = 0.5)"<<std::endl;
                std::cout<<std::endl<<"Examples:"<<std::endl;
                std::string programName = std::string(argv[0]);
                std::cout<<"Run algoritm with default settings for data.txt:"<<std::endl;
                std::cout<<programName<<" -f data.txt"<<std::endl;
                std::cout<<"Run algoritm for data.txt with generation count od 30 and population size of 20:"<<std::endl;
                std::cout<<programName<<" -f data.txt -g 30 -p 20"<<std::endl;
                return 0;
            }
            if (std::string(argv[i]) == "-f")
            {
                if (i+1<argc)
                {
                    fileName = std::string(argv[i+1]);
                }
                else {std::cout<<"Invalid parameters"<<std::endl; return -1;}
            }
            if (std::string(argv[i]) == "-p")
            {
                if (i+1<argc)
                {
                    sscanf(argv[i+1], "%d", &population_size);

                }
                else {std::cout<<"Invalid parameters"<<std::endl; return -1;}
            }
            if (std::string(argv[i]) == "-g")
            {
                if (i+1<argc)
                {
                    sscanf(argv[i+1], "%d", &generation_count);
                }
                else {std::cout<<"Invalid parameters"<<std::endl; return -1;}
            }
            if (std::string(argv[i]) == "-e")
            {
                if (i+1<argc)
                {
                    sscanf(argv[i+1], "%d", &elite_count);
                    if (elite_count>=population_size) {std::cout<<"Invalid parameters"<<std::endl; return -1;}
                }
                else {std::cout<<"Invalid parameters"<<std::endl; return -1;}
            }
            if (std::string(argv[i]) == "-m")
            {
                if (i+1<argc)
                {
                    sscanf(argv[i+1], "%f", &mutation_rate);
                }
                else {std::cout<<"Invalid parameters"<<std::endl; return -1;}
            }
            if (std::string(argv[i]) == "-r")
            {
                if (i+1<argc)
                {
                    sscanf(argv[i+1], "%f", &recombination_rate);
                }
                else {std::cout<<"Invalid parameters"<<std::endl; return -1;}
            }

        }
    }
    if (fileName=="")
    { std::cout<<"Please specify data source."<<std::endl;std::cout<<"Use -h for more info."<<std::endl;return -1;}
    if (elite_count>=population_size) {std::cout<<"Invalid parameters"<<std::endl; return -1;}
    if (generation_count<1 || population_size<1 || elite_count < 1) {std::cout<<"Invalid parameters"<<std::endl; return -1;}
    if (mutation_rate<0 || mutation_rate > 1) {std::cout<<"Invalid parameters"<<std::endl; return -1;}
    if (recombination_rate<0 || recombination_rate > 1) {std::cout<<"Invalid parameters"<<std::endl; return -1;}

    std::cout<<"Running GA for "<<fileName<<std::endl;
    std::cout<<"Generations: "<<generation_count<<"; Population size: "<<population_size<<"; Elite count: "<<elite_count<<std::endl;
    std::cout<<"Recombination rate: "<<recombination_rate<<"; Mutation_rate: "<<mutation_rate<<std::endl;
    std::default_random_engine generator = std::default_random_engine(time(NULL));
    KnapsackData data = loadKnapsackData(fileName);
    KnapsackProblem problem(&data, mutation_rate, recombination_rate);
    int n_items = data.items.size();
    std::vector<std::vector<bool>> initial_population;
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

        std::cout<<"]";
        std::cout<<std::endl;
    }
    std::string resultsFile = saveAverages(averages, fileName);
    std::cout<<"Average values for each generation was saved into "<<resultsFile<<std::endl;
    return 0;
}
