#include "Provider.h"
#include "Warehouse.h"
#include <iostream>
#include <istream>
#include <fstream>
#include <sstream>   
#include <iomanip>   
#include <algorithm> 
#include "Accountant.h"
#include "Administrator.h"


void Provider::ProcessOrder(const std::map<std::string, int>& order, double& balance) {
    std::cout << "Обработка заказа..." << std::endl;
    double totalCost = 0.0;
    Warehouse warehouse;

    Accountant accountant;

    // Передача информации о поставках продуктов Accountant
    for (const auto& item : order) {
        const std::string& productName = item.first;
        int quantity = item.second;
        accountant.ReceiveProductDelivery(productName, quantity);
    }

    for (const auto& item : order) {
        const std::string& productName = item.first;
        int quantity = item.second;

        // Выполнение необходимых действий для выполнения заказа, например, связь с поставщиками, отправка товаров и т.д.
        std::cout << "Товар: " << productName << ", Количество: " << quantity << " - Отгружено" << std::endl;

        // Обновление инвентаря на складе
        warehouse.AddProduct(productName, quantity);

        // Расчет общей стоимости заказа
        double productPrice = warehouse.GetProductPrice(productName);
        totalCost += productPrice * quantity;
    }

    // Вычитание общей стоимости из баланса
    balance += totalCost;

    std::cout << "Заказ успешно обработан. Общая стоимость: $" << totalCost << std::endl;
    std::cout << "Оставшийся баланс: $" << balance << std::endl;

    // Генерация финансового отчета
    std::map<std::string, std::shared_ptr<Product>> products = warehouse.GetProducts();
    accountant.GenerateFinancialReport(products);
}
std::map<std::string, int> Provider::ViewOrder(const std::string& orderFilename) {
    std::map<std::string, int> order;
    std::ifstream file(orderFilename);
    if (!file) {
        std::cout << "Ошибка при открытии файла: " << orderFilename << std::endl;
        return order;
    }

    std::cout << "Детали заказа из файла \"" << orderFilename << "\":" << std::endl;
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string productName;
        int quantity;
        if (iss >> productName >> quantity) {
            std::cout << "Товар: " << productName << ", Количество: " << quantity << std::endl;
            order[productName] = quantity;
        }
    }

    file.close();
    return order;
}
void Provider::RunProvider() {
    Administrator::AuditIn("Произведен вход за поставщика");
    std::map<std::string, int> order;
    double balance = 10000.00;
    std::cout << "Система поставщика" << std::endl;
    while (true) {
        std::cout << std::endl;
        std::cout << "1. Просмотреть заказ" << std::endl;
        std::cout << "2. Обработать заказ" << std::endl;
        std::cout << "3. Выход" << std::endl;
        std::cout << "Введите ваш выбор: ";
        int choice;
        std::cin >> choice;
        switch (choice) {
        case 1:
            Administrator::AuditIn("Просмотр заказа на продукты поставщиком");
            order = ViewOrder("OrderProduct.txt");
            break;
        case 2:
            if (!order.empty()) {
                Administrator::AuditIn("Обработка заказа на продукты поставщиком");
                ProcessOrder(order, balance);
                order.clear();
            }
            else {
                std::cout << "Нет заказа для обработки." << std::endl;
            }
            break;
        case 3:
            Administrator::AuditIn("Поставщик вышел");
            std::cout << "Выход..." << std::endl;
            return;
        default:
            std::cout << "Некорректный выбор. Пожалуйста, повторите попытку." << std::endl;
            break;
        }
    }
}
