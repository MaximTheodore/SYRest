#include "Accountant.h"
#include "Product.h"
#include "MenuItem.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Administrator.h"

void Accountant::SubmitProductRequest(const std::string& productName, int quantity) {
    if (quantity <= 0) {
        std::cout << "Неверное количество продукта. Попробуйте еще раз." << std::endl;
        return;
    }
    productRequests[productName] += quantity;
}

void Accountant::ReceiveProductDelivery(const std::string& productName, int quantity) {
    if (quantity <= 0) {
        std::cout << "Неверное количество продукта. Попробуйте еще раз." << std::endl;
        return;
    }
    productDeliveries[productName] += quantity;
}

void Accountant::GenerateFinancialReport(const std::map<std::string, std::shared_ptr<Product>>& products) {
    double totalRevenue = 0.0;
    double totalExpenses = 0.0;

    std::ofstream outputFile("operations.txt");
    if (outputFile.is_open()) {
        outputFile << "Финансовый отчет:" << std::endl;
        outputFile << "-----------------" << std::endl;

        for (const auto& pair : products) {
            double productRevenue = pair.second->GetCost() * pair.second->GetQuantity();
            outputFile << "Продукт: " << pair.second->GetName() << ", Продано: " << pair.second->GetQuantity() << ", Выручка: $" << productRevenue << std::endl;

            totalRevenue += productRevenue;
            totalExpenses += Product::GetProductPriceFromFile(pair.second->GetName()) * pair.second->GetQuantity();
        }

        outputFile << "-----------------" << std::endl;
        outputFile << "Общая выручка: $" << totalExpenses << std::endl;
        outputFile << "Баланс: $" << totalRevenue - totalExpenses << std::endl;
        outputFile.close();

        std::cout << "Финансовый отчет сохранен в файле 'operations.txt'." << std::endl;
        std::cout << "Общие выручка: $" << totalExpenses << std::endl;
        std::cout << "Баланс: $" << totalRevenue - totalExpenses << std::endl;
    }
    else {
        std::cout << "Не удалось открыть файл для сохранения финансового отчета." << std::endl;
    }
}

void Accountant::PrintProductRequests() {
   

        std::ifstream inputFile("OrderProduct.txt");
        if (inputFile.is_open()) {
            std::string line;
            while (std::getline(inputFile, line)) {
                std::cout << line << std::endl;
            }
            inputFile.close();
        }
        else {
            std::cout << "Не удалось открыть файл для чтения информации о заявках." << std::endl;
        }
 
}

void Accountant::PrintProductDeliveries() {
    std::ifstream inputFile("OrderProduct.txt");
    if (inputFile.is_open()) {
        std::string line;
        while (std::getline(inputFile, line)) {
            std::cout << line << std::endl;
        }
        inputFile.close();
    }
    else {
        std::cout << "Не удалось открыть файл для чтения информации о заявках." << std::endl;
    }
}

void Accountant::GetSoldItemsReport() {
    std::vector<std::pair<std::string, int>> soldItems;

    std::ifstream inputFile("sold_items_report.txt");
    if (inputFile.is_open()) {
        std::string line;
        std::string itemName;
        int quantity;
        while (std::getline(inputFile, itemName) && std::getline(inputFile, line)) {
            std::istringstream iss(line);
            if (iss >> quantity) {
                soldItems.push_back(std::make_pair(itemName, quantity));
            }
        }
        inputFile.close();
    }
    else {
        std::cout << "Не удалось открыть файл с отчетом о проданных блюдах." << std::endl;
        return;
    }

    std::ofstream outputFile("sold_items_report.txt", std::ios::app);
    if (outputFile.is_open()) {
        outputFile << "Отчет о проданных блюдах:" << std::endl;
        outputFile << "------------------------" << std::endl;

        if (!soldItems.empty()) {
            for (const auto& pair : soldItems) {
                outputFile << "Блюдо: " << pair.first << ", Продано: " << pair.second << std::endl;
            }

            outputFile << "------------------------" << std::endl;
        }
        else {
            outputFile << "Нет данных о проданных блюдах." << std::endl;
        }
        outputFile.close();
        std::cout << "Отчет о проданных блюдах сохранен в файле 'sold_items_report.txt'." << std::endl;
        std::cout << "Отчет о проданных блюдах:" << std::endl;

        if (!soldItems.empty()) {
            for (const auto& pair : soldItems) {
                std::cout << "Блюдо: " << pair.first << ", Продано: " << pair.second << std::endl;
            }
        }
        else {
            std::cout << "Нет данных о проданных блюдах." << std::endl;
        }
    }
    else {
        std::cout << "Не удалось открыть файл для сохранения отчета о проданных блюдах." << std::endl;
    }
}

void Accountant::GenerateSalesReport(const std::map<int, std::shared_ptr<MenuItem>>& menuItems) {
    double totalSales = 0.0;

    std::ofstream outputFile("operations.txt", std::ios::app);
    if (outputFile.is_open()) {
        outputFile << "Отчет о продажах:" << std::endl;
        outputFile << "-----------------" << std::endl;

        for (const auto& pair : menuItems) {
            std::shared_ptr<MenuItem> menuItem = pair.second;
            std::string itemName = menuItem->GetName();
            int quantity = menuItem->GetQuantity();
            double itemRevenue = menuItem->GetPrice() * quantity;

            outputFile << "Блюдо: " << itemName << ", Продано: " << quantity << ", Выручка: $" << itemRevenue << std::endl;

            totalSales += itemRevenue;
        }

        outputFile << "-----------------" << std::endl;
        outputFile << "Общая выручка: $" << totalSales << std::endl;
        outputFile.close();

        std::cout << "Отчет о продажах сохранен в файле 'operations.txt'." << std::endl;
        std::cout << "Отчет о продажах:" << std::endl;

        for (const auto& pair : menuItems) {
            std::shared_ptr<MenuItem> menuItem = pair.second;
            std::string itemName = menuItem->GetName();
            int quantity = menuItem->GetQuantity();
            double itemRevenue = menuItem->GetPrice() * quantity;

            std::cout << "Блюдо: " << itemName << ", Продано: " << quantity << ", Выручка: $" << itemRevenue << std::endl;
        }

        std::cout << "Общая выручка: $" << totalSales << std::endl;
    }
    else {
        std::cout << "Не удалось открыть файл для сохранения отчета о продажах." << std::endl;
    }
}

void Accountant::run() {
    Administrator::AuditIn("Произведен вход за бухгалтера");
    std::cout << "Добро пожаловать, бухгалтер!" << std::endl;

    while (true) {
        std::cout << std::endl;
        std::cout << "Выберите действие:" << std::endl;
        std::cout << "1. Вывести список отправленных заявок на продукты" << std::endl;
        std::cout << "2. Вывести список принятых поставок продуктов" << std::endl;
        std::cout << "3. Сгенерировать финансовый отчет" << std::endl;
        std::cout << "4. Сгенерировать отчет о проданных блюдах" << std::endl;
        std::cout << "5. Выйти из программы" << std::endl;

        int choice;
        std::cin >> choice;
        if (choice == 1) {
            Administrator::AuditIn("Открыт список отправленных заявок на продукты");
            PrintProductRequests();
        }
        else if (choice == 2) {
            Administrator::AuditIn("Открыт список принятых поставок продуктов");
            PrintProductDeliveries();
        }
        else if (choice == 3) {
            std::map<std::string, std::shared_ptr<Product>> products;
            Administrator::AuditIn("Сгенерирован финансовый отчет");
            GenerateFinancialReport(products);
        }
        else if (choice == 4) {
            std::map<int, std::shared_ptr<MenuItem>> menuItems;
            Administrator::AuditIn(" Сгенерирован отчет о проданных блюдах");
            GenerateSalesReport(menuItems);
        }
        else if (choice == 5) {
            Administrator::AuditIn("Выход из бухгалтера");
            std::cout << "До свидания!" << std::endl;
            break;
        }
        else {
            std::cout << "Неверный выбор. Попробуйте еще раз." << std::endl;
        }
    }
}

