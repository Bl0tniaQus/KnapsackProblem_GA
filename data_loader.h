#pragma once
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
std::vector<double> getWeights(KnapsackData* data)
{
    std::vector<double>weights = {};
    for (Item i : data->items)
    {
        weights.push_back(i.volume);
    }
    return weights;
}
std::vector<double> getValues(KnapsackData* data)
{
    std::vector<double>values = {};
    for (Item i : data->items)
    {
        values.push_back(i.value);
    }
    return values;
}
