import random

output_file = "dane_problem_plecakowy.txt"
number_of_items = int(input("Podaj liczbę przedmiotów: "))
min_volume = float(input("Podaj minimalną objętość przedmiotu: "))
max_volume = float(input("Podaj maksymalną objętość przedmiotu: "))
min_value = float(input("Podaj minimalną wartość przedmiotu: "))
max_value = float(input("Podaj maksymalną wartość przedmiotu: "))

if min_volume >= max_volume or min_value >= max_value:
    print("Złe wartosci")
else:
    items = []
    total_volume = 0  

    for item_id in range(1, number_of_items + 1):
        volume = round(random.uniform(min_volume, max_volume), 2)
        value = round(random.uniform(min_value, max_value), 2)
        total_volume += volume
        items.append((item_id, volume, value))

    print(f"Łączna objętość wylosowanych przedmiotów: {total_volume}")

    knapsack_capacity = float(input("Podaj pojemność plecaka: "))

    with open(output_file, 'w') as file:
        file.write(f"{knapsack_capacity}\n")
        for item in items:
            file.write(f"{item[0]} {item[1]} {item[2]}\n")

    print(f"Dane zapisano do pliku: {output_file}")
