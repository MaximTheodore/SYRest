#include <iostream>
#include <fstream>
#include <sstream>
#include "Administrator.h"
#include <memory>
#include "Product.h"
#include "MenuItem.h"
#include "Employee.h"
#include <iomanip>
#include "../LibHash/Header.h"
#include <chrono>
#include <ctime>
using namespace std;

Administrator::Administrator(std::map<int,std::shared_ptr <Product>>& _products,
    std::map<std::string, std::shared_ptr<Employee>>& employees,
    std::shared_ptr<MenuItem>& menu)
    : products(_products), employees(employees), menu(menu) {}

void Administrator::Run() {
    Administrator::AuditIn("Произведен вход за админа");
    int choice = -1;
    while (choice != 0) {
        std::cout << "Меню админа:\n";
        std::cout << "1. Добавить продукт\n";
        std::cout << "2. Изменить продукт\n";
        std::cout << "3. Удалить продукт\n";
        std::cout << "4. Просмотр продуктов\n";
        std::cout << "5. Добавить сотрудника\n";
        std::cout << "6. Изменить сотрудника\n";
        std::cout << "7. Удалить состружника\n";
        std::cout << "8. Список сотрудников\n";
        std::cout << "9. Добавить пункт меню\n";
        std::cout << "10. Изменить пункт меню\n";
        std::cout << "11. Удалить пункт меню\n";
        std::cout << "12. Посмотреть файл всех операций\n";
        std::cout << "0. Разлогиниться\n";
        std::cout << "Выбор: ";
        std::cin >> choice;

            int id, quantity;
            std::string name;
            double cost;
       switch(choice) {
        case 1:
            std::cout << "Введите ID продукта: ";
            std::cin >> id;
            std::cout << "Введите имя продукта: ";
            std::cin.ignore();
            std::getline(std::cin, name);
            std::cout << "Введите стоимость продукта: ";
            std::cin >> cost;
            std::cout << "Введите количество продукта: ";
            std::cin >> quantity;

            // Инициализация переменной newProduct перед использованием
            std::cout << "Продукт успешно добавлен." << std::endl;

            
 
            AddProduct(products, id,name,cost,quantity);
            AuditIn("Добавлен продукт " + name +" "+ std::to_string(quantity));
            break;
        case 2:
         
            std::cout << "Введите ID продукта для изменения: ";
            std::cin >> id;
            UpdateProduct(id);
            AuditIn("Изменен продукт по ID " + id);
            break;
        case 3:
          
            std::cout << "Введите ID продукта удаления: ";
            std::cin >> id;
            RemoveProduct(id);
            AuditIn("Удален продукт по ID " + id);
            break;
        case 4:
            DisplayProducts();
            AuditIn("Вызвана операция просмотра продуктов");
            break;
        case 5:
            AddEmployee();
            break;
        case 6:
            UpdateEmployee();
            break;
        case 7:
            RemoveEmployee();
            break;
        case 8:
            DisplayEmployees();
            break;
        case 9:
            AddMenuItem();
            break; 
        case 10:
            UpdateMenuItem();
            break; 
        case 11:
            DeleteMenuItem();
            break; 
        case 12:
            AuditOut();
            break;
        case 0:
            std::cout << "Выход...\n";
            break;
        default:
            std::cout << "Неправильный выбор. Попробуйте снова.\n";
            break;
        }

        std::cout << "\n-------------------------------------\n";
    }
}



void Administrator::AddProduct(std::map<int, std::shared_ptr<Product>>& products, int id, const std::string& name, double cost, int quantity) {
    std::shared_ptr<Product> product = std::make_shared<Product>(id, name, cost,quantity);
    products[id] = product;
    Product::SaveToFile("products.txt", products);
    std::cout << "Продукт успешно добавлен.\n";
    Administrator::AuditIn("Продукт " + name +" успешно добавлен");
}

void Administrator::UpdateProduct(int id) {

    auto it = products.find(id);
    if (it != products.end()) {
        std::string name;
        double cost;

        std::cout << "Введите новое имя продукта: ";
        std::cin.ignore();
        std::getline(std::cin, name);
        std::cout << "Введите новую стоимость продукта: ";
        std::cin >> cost;
        
        it->second->SetName(name);
        it->second->SetCost(cost);

        std::cout << "Продукт успешно изменен.\n";
    }
    else {
        std::cout << "Продукт не найден.\n";
    }
}

void Administrator::RemoveProduct(int id) {

    auto it = products.find(id);
    if (it != products.end()) {
        products.erase(it);
        std::cout << "Прдукт успешно удален.\n";
    }
    else {
        std::cout << "Продукт не найден.\n";
    }
}

void Administrator::DisplayProducts() const {
    if (products.empty()) {
        std::cout << "Нет доступный продуктов.\n";
    }
    else {
        std::cout << std::left << std::setw(5) << "ID"
            << std::setw(15) << "Имя"
            << std::setw(10) << "Цена USD"
            << std::setw(10) << "Количество" << std::endl;
        for (const auto& pair : products) {
            std::cout << std::setw(5) << pair.first;
            std::cout << std::setw(15) << pair.second->GetName();
            std::cout << std::setw(10) << pair.second->GetCost();
            std::cout << std::setw(10) << pair.second->GetQuantity();
        }
    }
}
void Administrator::ShowProducts(const std::map<int, std::shared_ptr<Product>>& products) {
    AuditIn("Вызвана операция ShowProducts для просмотра продуктов");

    std::cout << std::left << std::setw(5) << "ID"
        << std::setw(15) << "Имя"
        << std::setw(10) << "Цеца"
        << std::setw(10) << "Количество" << std::endl;

    for (const auto& pair : products) {
        std::cout << setw(5) << pair.second->GetId()
            << setw(15) << pair.second->GetName()
            << setw(10) << pair.second->GetCost()
            << setw(10) << pair.second->GetQuantity() << std::endl;
    }
}
void Administrator::AddEmployee() {
    std::string role, lastName, firstName, middleName, login, password;

    std::vector<std::string> roles = { "admin", "warehouse", "provider", "waiter", "cook", "accountant" };

    std::cout  << std::right << "Список ролей:\n";
    for (const std::string& role : roles) {
        std::cout  << std::right << role << std::endl;
    }
   
    std::cout << "Введите роль сотрудника: ";
    std::cin.ignore();
    std::getline(std::cin, role);
    std::cout << "Введите фамилию сотрудника: ";
    std::getline(std::cin, lastName);
    std::cout << "Введите имя сотрудника: ";
    std::getline(std::cin, firstName);
    std::cout << "Введите отчество сотрудника: ";
    std::getline(std::cin, middleName);
    std::cout << "Введите логин сотрудниика: ";
    std::getline(std::cin, login);
    std::cout << "Введите пароль для сотрудника: ";
    std::getline(std::cin, password);
    AuditIn("Добавлен сотрудник "+ firstName + " с ролью "+ role);

    std::shared_ptr<Employee> newEmployee = std::make_shared<Employee>(role, lastName, firstName,
        middleName, login, hashFunction(password));
    employees.insert(std::make_pair(login, newEmployee));

    std::cout << "Сотрудник успешно добавлен.\n";
}

void Administrator::UpdateEmployee() {
    std::string login;
    std::cout << "Введите логин сотрудника для изменения: ";
    std::cin.ignore();
    std::getline(std::cin, login);
    AuditIn("Изменен сотрудник под логином "+ login);
    std::vector<std::string> roles = { "admin", "warehouse", "provider", "waiter", "cook", "accountant" };
    std::cout << std::right << "Список ролей:\n";
    for (const std::string& role : roles) {
        std::cout << std::right << role << std::endl;
    }
        auto it = employees.find(login);
        if (it != employees.end()) {
            std::string role, lastName, firstName, middleName, password;

            std::cout << "Введите новую роль сотрудника: ";
            std::getline(std::cin, role);
            std::cout << "Введите новую фамилию сотрудника: ";
            std::getline(std::cin, lastName);
            std::cout << "Введите новое имя сотрудника: ";
            std::getline(std::cin, firstName);
            std::cout << "Введите новое отчество сотрудника: ";
            std::getline(std::cin, middleName);
            std::cout << "Введите новый логин сотрудниика: ";
            std::getline(std::cin, login);
            std::cout << "Введите новый пароль для сотрудника: ";
            std::getline(std::cin, password);

            it->second->SetRole(role);
            it->second->SetLastName(lastName);
            it->second->SetFirstName(firstName);
            it->second->SetMiddleName(middleName);
            it->second->SetPassword(hashFunction(password));

            std::cout << "Сотрудник успешно изменен.\n";
        }
        else {
            std::cout << "Сотрудник не найден.\n";
        }
   
}

void Administrator::RemoveEmployee() {
    std::string login;
    std::cout << "Введите логин сотрудника для удаления: ";
    std::cin.ignore();
    std::getline(std::cin, login);
    AuditIn("Сотрудник удален под ником "+ login);
    auto it = employees.find(login);
    if (it != employees.end()) {
        employees.erase(it);
        std::cout << "Сотрудник успешно удален.\n";
    }
    else {
        std::cout << "Сотрудник не найден.\n";
    }
}

void Administrator::DisplayEmployees() const {
    if (employees.empty()) {
        std::cout << "Нет доступных сотрудников.\n";
    }
    else {
        for (const auto& pair : employees) {
            std::cout << "Логин: " << pair.first << "\n";
            std::cout << "Роль: " << pair.second->GetRole() << "\n";
            std::cout << "Полное имя: " << pair.second->GetFullName() << "\n";
            std::cout << "-------------------------------------\n";
        }
    }
}





void Administrator::AddMenuItem() {
    std::string name, description;
    int weight, cookingTime, quantity;
    double price;

    std::cout << "Введите имя элемента меню: ";
    std::cin.ignore();
    std::getline(std::cin, name);
    std::cout << "Введите вес элемента меню: ";
    std::cin >> weight;
    std::cout << "Введите описание элемента меню: ";
    std::cin.ignore();
    std::getline(std::cin, description);
    std::cout << "Введите время готовности элемента меню: ";
    std::cin >> cookingTime;
    std::cout << "Введите цену элемента меню: ";
    std::cin >> price;  
    std::cout << "Введите количество элементов меню: ";
    std::cin >> quantity;

    AuditIn("Добавлен новый элемент меню " + name);
    std::cout << "Доступные продукты:\n";
    DisplayProducts();

    std::vector<std::shared_ptr<Product>> ingredients;
    std::string productIdStr;
    do {
        std::cout << "Введите ID продукта для добавления в качестве ингридиента (-1 для выхода): ";
        std::cin >> productIdStr;
        int productId = std::stoi(productIdStr);

        if (productId == -1) {
            break;
        }

        auto productIt = products.find(productId);
        if (productIt != products.end()) {
            ingredients.push_back(productIt->second);
            std::cout << "Ингридиент добавлен.\n";
        }
        else {
            std::cout << "Несучествующий ID продукта. Попробуйте снова.\n";
        }
    } while (true);

    std::shared_ptr<MenuItem> newMenuItem = std::make_shared<MenuItem>(name, weight,
        ingredients, description, cookingTime, price, quantity);
    menu->AddMenuItem(newMenuItem);

    std::cout << "Элемент меню успешно добавлен.\n";
}




void Administrator::UpdateMenuItem() {
    std::string name;
    std::cout << "Введите имя элемента меню для удаления: ";
    std::cin.ignore();
    std::getline(std::cin, name);
    AuditIn("Изменен элемент меню под именем " + name);
    std::shared_ptr<MenuItem> menuItem = MenuItem::FindMenuItemByName(name);
    if (menuItem != nullptr) {
        std::string newName, newDescription;
        int newWeight, newCookingTime, newQuantity;
        double newPrice;
        std::cout << "Введите новое имя для элемента меню: ";
        std::getline(std::cin, newName);
        std::cout << "Введите новую грамовку для элемента меню: ";
        std::cin >> newWeight;
        std::cout << "Введите новое описание для элемента меню: ";
        std::cin.ignore();
        std::getline(std::cin, newDescription);
        std::cout << "Введите новое время готовности для элемента меню: ";
        std::cin >> newCookingTime;
        std::cout << "Введите новую цену для элемента меню: ";
        std::cin >> newPrice;
        std::cout << "Введите новое количество для элемента меню: ";
        std::cin >> newQuantity;
        menuItem->SetName(newName);
        menuItem->SetWeight(newWeight);
        menuItem->SetDescription(newDescription);
        menuItem->SetCookingTime(newCookingTime);
        menuItem->SetPrice(newPrice);
        menuItem->SetQuantity(newQuantity);
        std::cout << "Available products:\n";
        DisplayProducts();
        std::vector<std::shared_ptr<Product>> ingredients;
        std::string productIdStr;
        do {
            std::cout << "Введите ID продукта для добавления как ингридиент  (-1 выход): ";
            std::cin >> productIdStr;
            int productId = std::stoi(productIdStr);
            if (productId == -1) {
                break;
            }
            auto productIt = products.find(productId);
            if (productIt != products.end()) {
                ingredients.push_back(productIt->second);
                std::cout << "Ингридиент добавлен.\n";
            }
            else {
                std::cout << "Неправильный ID продукта.\n";
            }
        } while (true);
        if (ingredients.size() > 0) {
            menuItem->SetIngredients(ingredients);
        }
        std::cout << "Элемент меню успешно обновлен.\n";
        // Save the menu items to the file
        std::map<int, std::shared_ptr<MenuItem>> menuItems;
        MenuItem::LoadFromFile("menu_items.txt", menuItems);
        // Find the menu item in the map and update it
        for (auto& pair : menuItems) {
            if (pair.second->GetName() == name) {
                menuItems[pair.first] = menuItem;
                break;
            }
        }

        MenuItem::SaveToFile("menu_items.txt", menuItems);
    }
    else {
        std::cout << "Элемент меню не найден.\n";
    }
}


void Administrator::DeleteMenuItem() {
    std::string name;
    std::cout << "Введите имя продукта для удаления: ";
    std::cin.ignore();
    std::getline(std::cin, name);
    AuditIn("Удален элемент меню под именем " + name);

    std::shared_ptr<MenuItem> menuItem = MenuItem::FindMenuItemByName(name);
    if (menuItem != nullptr) {
        std::cout << "Menu item successfully deleted.\n";

        // Save changes to file
        std::map<int, std::shared_ptr<MenuItem>> menuItems;
        MenuItem::LoadFromFile("menu_items.txt", menuItems);
        MenuItem::RemoveMenuItem(name,menuItems);
        MenuItem::SaveToFile("menu_items.txt", menuItems);
    }
    else {
        std::cout << "Menu item not found.\n";
    }
}



void Administrator::AuditIn(const std::string& audit)
{
    ofstream AuditIn("AuditLog.txt", ios::app);
    if (AuditIn.is_open()) {
        std::time_t currentTime = std::time(nullptr);
        char timestamp[26];
        ctime_s(timestamp, sizeof(timestamp), &currentTime);
        std::string timestampStr(timestamp);
        timestampStr.erase(timestampStr.length() - 1);

        AuditIn << timestampStr << " - " << audit << std::endl;
        AuditIn.close();
    }
    else {
        std::cout << "Ошибка в открытии файла" << std::endl;
    }
}
void Administrator::AuditOut()
{
    std::string line;
    ifstream AuditOut("AuditLog.txt");
    if (AuditOut.is_open()) {
        while (std::getline(AuditOut,line))
        {
            std::cout << line << "\n";
        }
        AuditOut.close();
    }
    else {
        std::cout << "Ошибка в открытии файла" << std::endl;
    }
}
