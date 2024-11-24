#include <iostream>
#include <fstream>
#include <vector>

struct Item {
    int id;
    double volume;
    double value;
};

struct KnapsackData {
    double capacity;
    std::vector<Item> items;
};

KnapsackData loadKnapsackData(const std::string& fileName) {
    KnapsackData data;
    std::ifstream file(fileName);

    if (file.is_open()) {
        file >> data.capacity;

        Item item;
        while (file >> item.id >> item.volume >> item.value) {
            data.items.push_back(item);
        }

        file.close();
        return data;
    }

    return KnapsackData{0.0, {}};
}

int main() {
    const std::string fileName = "knapsack_data.txt";

    try {
        KnapsackData data = loadKnapsackData(fileName);

        std::cout << "Pojemność plecaka: " << data.capacity << "\n";
        std::cout << "Przedmioty:\n";
        for (const auto& item : data.items) {
            std::cout << "ID: " << item.id
            << ", Objętość: " << item.volume
            << ", Wartość: " << item.value << "\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Wystąpił błąd: " << e.what() << "\n";
    }

    return 0;
}

