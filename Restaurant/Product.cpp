#include "Product.h"
#include <iostream>
#include <fstream>

Product::Product(const std::string& name, double cost) : name(name), cost(cost)
{

}


Product::Product(int id, const std::string& name, double cost, int quantity)
    : id(id), name(name), cost(cost), quantity(quantity) {}


int Product::GetId() const {
    return id;
}

const std::string& Product::GetName() const {
    return name;
}

double Product::GetCost() const {
    return cost;
}

int Product::GetQuantity() const {
    return quantity;
}

void Product::SetName(const std::string& newName) {
    name = newName;
}

void Product::SetCost(double newCost) {
    cost = newCost;
}

void Product::SetQuantity(int newQuantity) {
    quantity = newQuantity;
}

void Product::LoadFromFile(const std::string& filename, std::map<int, std::shared_ptr<Product>>& products) {
    std::ifstream file(filename);
    if (file.is_open()) {
        products.clear();

        int id;
        std::string name;
        double cost;
        int quantity;

        while (file >> id >> name >> cost >> quantity) {
            std::shared_ptr<Product> product = std::make_shared<Product>(id, name, cost, quantity);
            products.emplace(std::make_pair(id, product));

        }

        file.close();
    }
    else {
        std::cout << "Ошибка в открытии файла: " << filename << std::endl;
    }
}
void Product::SaveToFile(const std::string& filename, const std::map<int, std::shared_ptr<Product>>& products) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file.clear();
        for (const auto& pair : products) {
            file << pair.first << " " << pair.second->GetName() << " " << pair.second->GetCost()
                << " " << pair.second->GetQuantity() << std::endl;
        }


        file.close();
    }
    else {
        std::cout << "Ошибка в открытии файла: " << filename << std::endl;
    }
}



double Product::GetProductPriceFromFile(const std::string& productName) {
    std::ifstream file("products.txt");
    if (file.is_open()) {
        std::string name;
        double cost;
        int quantity, id;

        while (file >> id >> name >> cost >> quantity) {
            if (name == productName) {
                file.close();
                return cost;
            }
        }

        file.close();
    }
    else {
        std::cout << "Ошибка в открытии файла: products.txt" << std::endl;
    }

    return 9.99;
}