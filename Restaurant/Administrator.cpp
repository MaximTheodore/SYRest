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
    Administrator::AuditIn("���������� ���� �� ������");
    int choice = -1;
    while (choice != 0) {
        std::cout << "���� ������:\n";
        std::cout << "1. �������� �������\n";
        std::cout << "2. �������� �������\n";
        std::cout << "3. ������� �������\n";
        std::cout << "4. �������� ���������\n";
        std::cout << "5. �������� ����������\n";
        std::cout << "6. �������� ����������\n";
        std::cout << "7. ������� �����������\n";
        std::cout << "8. ������ �����������\n";
        std::cout << "9. �������� ����� ����\n";
        std::cout << "10. �������� ����� ����\n";
        std::cout << "11. ������� ����� ����\n";
        std::cout << "12. ���������� ���� ���� ��������\n";
        std::cout << "0. �������������\n";
        std::cout << "�����: ";
        std::cin >> choice;

            int id, quantity;
            std::string name;
            double cost;
       switch(choice) {
        case 1:
            std::cout << "������� ID ��������: ";
            std::cin >> id;
            std::cout << "������� ��� ��������: ";
            std::cin.ignore();
            std::getline(std::cin, name);
            std::cout << "������� ��������� ��������: ";
            std::cin >> cost;
            std::cout << "������� ���������� ��������: ";
            std::cin >> quantity;

            // ������������� ���������� newProduct ����� ��������������
            std::cout << "������� ������� ��������." << std::endl;

            
 
            AddProduct(products, id,name,cost,quantity);
            AuditIn("�������� ������� " + name +" "+ std::to_string(quantity));
            break;
        case 2:
         
            std::cout << "������� ID �������� ��� ���������: ";
            std::cin >> id;
            UpdateProduct(id);
            AuditIn("������� ������� �� ID " + id);
            break;
        case 3:
          
            std::cout << "������� ID �������� ��������: ";
            std::cin >> id;
            RemoveProduct(id);
            AuditIn("������ ������� �� ID " + id);
            break;
        case 4:
            DisplayProducts();
            AuditIn("������� �������� ��������� ���������");
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
            std::cout << "�����...\n";
            break;
        default:
            std::cout << "������������ �����. ���������� �����.\n";
            break;
        }

        std::cout << "\n-------------------------------------\n";
    }
}



void Administrator::AddProduct(std::map<int, std::shared_ptr<Product>>& products, int id, const std::string& name, double cost, int quantity) {
    std::shared_ptr<Product> product = std::make_shared<Product>(id, name, cost,quantity);
    products[id] = product;
    Product::SaveToFile("products.txt", products);
    std::cout << "������� ������� ��������.\n";
    Administrator::AuditIn("������� " + name +" ������� ��������");
}

void Administrator::UpdateProduct(int id) {

    auto it = products.find(id);
    if (it != products.end()) {
        std::string name;
        double cost;

        std::cout << "������� ����� ��� ��������: ";
        std::cin.ignore();
        std::getline(std::cin, name);
        std::cout << "������� ����� ��������� ��������: ";
        std::cin >> cost;
        
        it->second->SetName(name);
        it->second->SetCost(cost);

        std::cout << "������� ������� �������.\n";
    }
    else {
        std::cout << "������� �� ������.\n";
    }
}

void Administrator::RemoveProduct(int id) {

    auto it = products.find(id);
    if (it != products.end()) {
        products.erase(it);
        std::cout << "������ ������� ������.\n";
    }
    else {
        std::cout << "������� �� ������.\n";
    }
}

void Administrator::DisplayProducts() const {
    if (products.empty()) {
        std::cout << "��� ��������� ���������.\n";
    }
    else {
        std::cout << std::left << std::setw(5) << "ID"
            << std::setw(15) << "���"
            << std::setw(10) << "���� USD"
            << std::setw(10) << "����������" << std::endl;
        for (const auto& pair : products) {
            std::cout << std::setw(5) << pair.first;
            std::cout << std::setw(15) << pair.second->GetName();
            std::cout << std::setw(10) << pair.second->GetCost();
            std::cout << std::setw(10) << pair.second->GetQuantity();
        }
    }
}
void Administrator::ShowProducts(const std::map<int, std::shared_ptr<Product>>& products) {
    AuditIn("������� �������� ShowProducts ��� ��������� ���������");

    std::cout << std::left << std::setw(5) << "ID"
        << std::setw(15) << "���"
        << std::setw(10) << "����"
        << std::setw(10) << "����������" << std::endl;

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

    std::cout  << std::right << "������ �����:\n";
    for (const std::string& role : roles) {
        std::cout  << std::right << role << std::endl;
    }
   
    std::cout << "������� ���� ����������: ";
    std::cin.ignore();
    std::getline(std::cin, role);
    std::cout << "������� ������� ����������: ";
    std::getline(std::cin, lastName);
    std::cout << "������� ��� ����������: ";
    std::getline(std::cin, firstName);
    std::cout << "������� �������� ����������: ";
    std::getline(std::cin, middleName);
    std::cout << "������� ����� �����������: ";
    std::getline(std::cin, login);
    std::cout << "������� ������ ��� ����������: ";
    std::getline(std::cin, password);
    AuditIn("�������� ��������� "+ firstName + " � ����� "+ role);

    std::shared_ptr<Employee> newEmployee = std::make_shared<Employee>(role, lastName, firstName,
        middleName, login, hashFunction(password));
    employees.insert(std::make_pair(login, newEmployee));

    std::cout << "��������� ������� ��������.\n";
}

void Administrator::UpdateEmployee() {
    std::string login;
    std::cout << "������� ����� ���������� ��� ���������: ";
    std::cin.ignore();
    std::getline(std::cin, login);
    AuditIn("������� ��������� ��� ������� "+ login);
    std::vector<std::string> roles = { "admin", "warehouse", "provider", "waiter", "cook", "accountant" };
    std::cout << std::right << "������ �����:\n";
    for (const std::string& role : roles) {
        std::cout << std::right << role << std::endl;
    }
        auto it = employees.find(login);
        if (it != employees.end()) {
            std::string role, lastName, firstName, middleName, password;

            std::cout << "������� ����� ���� ����������: ";
            std::getline(std::cin, role);
            std::cout << "������� ����� ������� ����������: ";
            std::getline(std::cin, lastName);
            std::cout << "������� ����� ��� ����������: ";
            std::getline(std::cin, firstName);
            std::cout << "������� ����� �������� ����������: ";
            std::getline(std::cin, middleName);
            std::cout << "������� ����� ����� �����������: ";
            std::getline(std::cin, login);
            std::cout << "������� ����� ������ ��� ����������: ";
            std::getline(std::cin, password);

            it->second->SetRole(role);
            it->second->SetLastName(lastName);
            it->second->SetFirstName(firstName);
            it->second->SetMiddleName(middleName);
            it->second->SetPassword(hashFunction(password));

            std::cout << "��������� ������� �������.\n";
        }
        else {
            std::cout << "��������� �� ������.\n";
        }
   
}

void Administrator::RemoveEmployee() {
    std::string login;
    std::cout << "������� ����� ���������� ��� ��������: ";
    std::cin.ignore();
    std::getline(std::cin, login);
    AuditIn("��������� ������ ��� ����� "+ login);
    auto it = employees.find(login);
    if (it != employees.end()) {
        employees.erase(it);
        std::cout << "��������� ������� ������.\n";
    }
    else {
        std::cout << "��������� �� ������.\n";
    }
}

void Administrator::DisplayEmployees() const {
    if (employees.empty()) {
        std::cout << "��� ��������� �����������.\n";
    }
    else {
        for (const auto& pair : employees) {
            std::cout << "�����: " << pair.first << "\n";
            std::cout << "����: " << pair.second->GetRole() << "\n";
            std::cout << "������ ���: " << pair.second->GetFullName() << "\n";
            std::cout << "-------------------------------------\n";
        }
    }
}





void Administrator::AddMenuItem() {
    std::string name, description;
    int weight, cookingTime, quantity;
    double price;

    std::cout << "������� ��� �������� ����: ";
    std::cin.ignore();
    std::getline(std::cin, name);
    std::cout << "������� ��� �������� ����: ";
    std::cin >> weight;
    std::cout << "������� �������� �������� ����: ";
    std::cin.ignore();
    std::getline(std::cin, description);
    std::cout << "������� ����� ���������� �������� ����: ";
    std::cin >> cookingTime;
    std::cout << "������� ���� �������� ����: ";
    std::cin >> price;  
    std::cout << "������� ���������� ��������� ����: ";
    std::cin >> quantity;

    AuditIn("�������� ����� ������� ���� " + name);
    std::cout << "��������� ��������:\n";
    DisplayProducts();

    std::vector<std::shared_ptr<Product>> ingredients;
    std::string productIdStr;
    do {
        std::cout << "������� ID �������� ��� ���������� � �������� ����������� (-1 ��� ������): ";
        std::cin >> productIdStr;
        int productId = std::stoi(productIdStr);

        if (productId == -1) {
            break;
        }

        auto productIt = products.find(productId);
        if (productIt != products.end()) {
            ingredients.push_back(productIt->second);
            std::cout << "���������� ��������.\n";
        }
        else {
            std::cout << "�������������� ID ��������. ���������� �����.\n";
        }
    } while (true);

    std::shared_ptr<MenuItem> newMenuItem = std::make_shared<MenuItem>(name, weight,
        ingredients, description, cookingTime, price, quantity);
    menu->AddMenuItem(newMenuItem);

    std::cout << "������� ���� ������� ��������.\n";
}




void Administrator::UpdateMenuItem() {
    std::string name;
    std::cout << "������� ��� �������� ���� ��� ��������: ";
    std::cin.ignore();
    std::getline(std::cin, name);
    AuditIn("������� ������� ���� ��� ������ " + name);
    std::shared_ptr<MenuItem> menuItem = MenuItem::FindMenuItemByName(name);
    if (menuItem != nullptr) {
        std::string newName, newDescription;
        int newWeight, newCookingTime, newQuantity;
        double newPrice;
        std::cout << "������� ����� ��� ��� �������� ����: ";
        std::getline(std::cin, newName);
        std::cout << "������� ����� �������� ��� �������� ����: ";
        std::cin >> newWeight;
        std::cout << "������� ����� �������� ��� �������� ����: ";
        std::cin.ignore();
        std::getline(std::cin, newDescription);
        std::cout << "������� ����� ����� ���������� ��� �������� ����: ";
        std::cin >> newCookingTime;
        std::cout << "������� ����� ���� ��� �������� ����: ";
        std::cin >> newPrice;
        std::cout << "������� ����� ���������� ��� �������� ����: ";
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
            std::cout << "������� ID �������� ��� ���������� ��� ����������  (-1 �����): ";
            std::cin >> productIdStr;
            int productId = std::stoi(productIdStr);
            if (productId == -1) {
                break;
            }
            auto productIt = products.find(productId);
            if (productIt != products.end()) {
                ingredients.push_back(productIt->second);
                std::cout << "���������� ��������.\n";
            }
            else {
                std::cout << "������������ ID ��������.\n";
            }
        } while (true);
        if (ingredients.size() > 0) {
            menuItem->SetIngredients(ingredients);
        }
        std::cout << "������� ���� ������� ��������.\n";
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
        std::cout << "������� ���� �� ������.\n";
    }
}


void Administrator::DeleteMenuItem() {
    std::string name;
    std::cout << "������� ��� �������� ��� ��������: ";
    std::cin.ignore();
    std::getline(std::cin, name);
    AuditIn("������ ������� ���� ��� ������ " + name);

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
        std::cout << "������ � �������� �����" << std::endl;
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
        std::cout << "������ � �������� �����" << std::endl;
    }
}
