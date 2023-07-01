#include <iostream>
#include <map>
#include <memory>
#include "Administrator.h"
#include "Guest.h"
#include "Warehouse.h"
#include <Windows.h>
#include <windows.h>
#include "../LibHash/Header.h"
#include "Provider.h"
#include "Accountant.h"
#include "Cook.h"
#include "Waiter.h"

int main() {
    SetConsoleCP(1251);
    setlocale(LC_ALL, "Russian");


    std::map< int,std::shared_ptr<Product>> products;
    std::map<std::string, std::shared_ptr<Employee>> employees;
    std::shared_ptr<MenuItem> menu = std::make_shared<MenuItem>();

    Employee::LoadFromFile("employees.txt", employees);
    Product::LoadFromFile("products.txt", products);
   
    Administrator admin(products, employees, menu);
    Warehouse warehouse;

    Accountant acc;
    Guest run;
    Provider provider;


    Cook cook;
    Waiter waiter;
    int choice = -1;
    while (choice != 0) {
        
        std::cout << "Добро пожаловать в систему 'Ресторан'!\n";
        std::cout << "1. Логин\n";
        std::cout << "2. Продолжить как гость\n";
        std::cout << "0. Выход\n";
        std::cout << "Выбор: ";
        std::cin >> choice;

        switch (choice) {
        case 1: {
        
            system("cls");
            std::string login, password;
            std::cout << "Введите логин: ";
            std::cin.ignore();
            std::getline(std::cin, login);
            std::cout << "Введите пароль: ";
            std::getline(std::cin, password);

            auto it = employees.find(login);
            if (it != employees.end() && it->second->GetPassword() == hashFunction(password)) {
                if (it->second->GetRole() == "admin")  admin.Run();
                else if (it->second->GetRole() == "warehouse") warehouse.Run();
                else if (it->second->GetRole() == "provider") provider.RunProvider();
                else if (it->second->GetRole() == "accountant") acc.run();
                else if (it->second->GetRole() == "cook") cook.Run();
                else if (it->second->GetRole() == "waiter") waiter.Run();
            }
            else std::cout << "Неправильные данные.\n";
            break;
        }
        case 2:
            system("cls");
            std::cout << "Вход за гостя.\n";
            Administrator::AuditIn("Вход за гостя");
            run.Run();
            break;
        case 0:
            Administrator::AuditIn("Выход");
            std::cout << "Выход...\n";
            break;
        default:
            std::cout << "Неправильный выбор. Попробуйте снова.\n";
            break;
        }

        std::cout << "-------------------------------------\n";
        Product::SaveToFile("products.txt", products);
        Employee::SaveToFile("employees.txt", employees);
    }
    Order::SaveOrderStatus(OrderStatus::UNKNOWN);
    return 0;
}
