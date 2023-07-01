#include "MenuItem.h"
#include <iostream>
#include <fstream>
#include <sstream>
MenuItem::MenuItem()
    : name(""), price(0.0), quantity(0), weight(0), description(""), cookingTime(0) {}

MenuItem::MenuItem(const std::string& name, double price, int quantity)
    : name(name), price(price), quantity(quantity) {}

MenuItem::MenuItem(const std::string& name, int weight,
    const std::vector<std::shared_ptr<Product>>& ingredients,
    const std::string& description, int cookingTime, double price, int quantity)
    : name(name), weight(weight), ingredients(ingredients), description(description),
    cookingTime(cookingTime), price(price), quantity(quantity) {}

const std::string& MenuItem::GetName() const {
    return name;
}

int MenuItem::GetQuantity() const {
    return quantity;
}
void MenuItem::SetName(const std::string& newName) {
    name = newName;
}
void MenuItem::SetCookingTime(int newCookingTime) {
    cookingTime = newCookingTime;
}

void MenuItem::SetDescription(const std::string& newDescription) {
    description = newDescription;
}
void MenuItem::SetPrice(double newPrice) {
    price = newPrice;
}


void MenuItem::SetWeight(int newWeight) {
    weight = newWeight;
}

void MenuItem::SetQuantity(int newQuantity) {
    quantity = newQuantity;
}

double MenuItem::GetPrice() const {
    return price;
}

void MenuItem::LoadFromFile(const std::string& filename, std::map<int, std::shared_ptr<MenuItem>>& menuItems) {
    std::ifstream file(filename);
    if (file.is_open()) {
        menuItems.clear();  

        std::string line;
        int itemId = 1; 
        while (std::getline(file, line)) {
            std::string name = line;
            int quantity = 0, weight = 0, cookingTime = 0;
            double price = 0.0;
            std::vector<std::shared_ptr<Product>> ingredients;
            std::string description;

            file >> weight;
            file.ignore();
            std::getline(file, line); 
            std::stringstream ss(line);
            std::string ingredient;
            while (std::getline(ss, ingredient, ',')) {
                ingredients.push_back(std::make_shared<Product>(ingredient, 0));
            }

            std::getline(file, description);
            file >> cookingTime >> price >> quantity;
            file.ignore();

            std::shared_ptr<MenuItem> item = std::make_shared<MenuItem>(name, weight, ingredients, description, cookingTime, price, quantity);

            menuItems[itemId] = item; 
            itemId++; 
        }

        file.close();
    }
    else {
        std::cout << "Ошибка в открытии файла: " << filename << "\n";
    }
}

void MenuItem::SaveToFile(const std::string& filename, const std::map<int, std::shared_ptr<MenuItem>>& menuItems) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (const auto& pair : menuItems) {
            const std::shared_ptr<MenuItem>& menuItem = pair.second;

            file << menuItem->GetName() << "\n";
            file << menuItem->GetWeight() << "\n";

            const std::vector<std::shared_ptr<Product>>& ingredients = menuItem->GetIngredients();
            for (const auto& ingredient : ingredients) {
                file << ingredient->GetName() << ",";
            }
            file.seekp(-1, std::ios_base::cur);  // Удаление предыдущей команды
            file << "\n";

            file << menuItem->GetDescription() << "\n";
            file << menuItem->GetCookingTime() << " " << menuItem->GetPrice() << " " << menuItem->GetQuantity() << "\n";
        }
        file.close();
        std::cout << "Элемент меню сохранен в файл: " << filename << "\n";
    }
    else {
        std::cout << "Ошибка в открытии файла: " << filename << "\n";
    }
}

int MenuItem::GetWeight() const {
    return weight;
}

const std::vector<std::shared_ptr<Product>>& MenuItem::GetIngredients() const {
    return ingredients;
}
std::string MenuItem::GetItemsIngredients(const std::vector<std::shared_ptr<Product>>& ingredients) {
    std::string result;
    for (const auto& ingredient : ingredients) {
        result += ingredient->GetName() + "\n";
    }
    return result;
}

std::string MenuItem::GetDescription() const {
    return description;
}

int MenuItem::GetCookingTime() const {
    return cookingTime;
}
void MenuItem::AddMenuItem(const std::shared_ptr<MenuItem>& item) {
    menuItems.push_back(item);
    std::ofstream file("menu_items.txt", std::ios::app); // Открываем файл в режиме добавления (append)
    if (file.is_open()) {
        file << item->GetName() << "\n";
        file << item->GetWeight() << "\n";
        for (const auto& ingredient : item->GetIngredients()) {
            file << ingredient->GetName() << ",";
        }
        file.seekp(-1, std::ios_base::cur); // Удаляем завершающую запятую
        file << "\n";
        file << item->GetDescription() << "\n";
        file << item->GetCookingTime() << " " << item->GetPrice() << " " << item->GetQuantity() << "\n";

        file.close();
    }
    else {
        std::cout << "Ошибка в открытии файла: menu_items.txt\n";
    }
    
}
void MenuItem::LoadFromFileForCook(const std::string& filename, std::map<std::string, std::shared_ptr<MenuItem>>& menuItems) {
    std::ifstream file(filename);
    if (file.is_open()) {
        menuItems.clear(); 

        std::string line;

        while (std::getline(file, line)) {
            std::string name = line;
            int quantity = 0, weight = 0, cookingTime = 0;
            double price = 0.0;
            std::vector<std::shared_ptr<Product>> ingredients;
            std::string description;

            file >> weight;
            file.ignore();
            std::getline(file, line); 
            std::stringstream ss(line);
            std::string ingredient;
            while (std::getline(ss, ingredient, ',')) {
                ingredients.push_back(std::make_shared<Product>(ingredient, 0.0));
            }

            std::getline(file, description);
            file >> cookingTime >> price >> quantity;
            file.ignore();

            std::shared_ptr<MenuItem> item = std::make_shared<MenuItem>(name, weight, ingredients, description, cookingTime, price, quantity);

            menuItems[name] = item; 
        }

        file.close();
    }
    else {
        std::cout << "Ошибка в открытии файла: " << filename << "\n";
    }
}
void MenuItem::SaveToFileForCook(const std::string& filename, const std::map<std::string, std::shared_ptr<MenuItem>>& menuItems) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (const auto& pair : menuItems) {
            const std::shared_ptr<MenuItem>& item = pair.second;

            file << item->GetName() << "\n";
            file << item->GetWeight() << "\n";
            for (const auto& ingredient : item->GetIngredients()) {
                file << ingredient->GetName() << ",";
            }
            file.seekp(-1, std::ios_base::cur); 
            file << "\n";
            file << item->GetDescription() << "\n";
            file << item->GetCookingTime() << " " << item->GetPrice() << " " << item->GetQuantity() << "\n";
        }

        file.close();
    }
    else {
        std::cout << "Ошибка в открытии файла: " << filename << "\n";
    }
}
void MenuItem::RemoveMenuItem(const std::string& itemName, std::map<int, std::shared_ptr<MenuItem>>& menuItems) {
    auto it = std::find_if(menuItems.begin(), menuItems.end(), [&itemName](const auto& pair) {
        return pair.second->GetName() == itemName;
        });

    if (it != menuItems.end()) {
        menuItems.erase(it);
        std::cout << "Элемент меню успешно удален.\n";
    }
    else {
        std::cout << "Элемент меню не найден.\n";
    }
}
std::shared_ptr<MenuItem> MenuItem::FindMenuItemByName(const std::string& name) {
    std::ifstream file("menu_items.txt");

    if (file.is_open()) {
        std::string line;

        while (std::getline(file, line)) {
            if (line == name) {
                // Found the menu item with the specified name
                int weight, cookingTime, quantity;
                double price;
                std::vector<std::shared_ptr<Product>> ingredients;
                std::string description;

                file >> weight;
                file.ignore();
                std::getline(file, line); // Read the rest of the line
                std::stringstream ss(line);
                std::string ingredient;
                while (std::getline(ss, ingredient, ',')) {
                    ingredients.push_back(std::make_shared<Product>(ingredient, 0.0));
                }

                std::getline(file, description);
                file >> cookingTime >> price >> quantity;
                file.ignore();

                std::shared_ptr<MenuItem> menuItem = std::make_shared<MenuItem>(name, weight, ingredients, description, cookingTime, price, quantity);

                file.close();
                return menuItem;
            }
            else {

                std::getline(file, line); // Пропускаем weight
                std::getline(file, line); // Пропускаем ingredients
                std::getline(file, line); // Пропускаем description
                std::getline(file, line); // Пропуск cookingTime, price, and quantity
            }
        }

        file.close();
    }
    else {
        std::cout << "Ошибка в открытии: menu_items.txt\n";
    }

    return 0;
}