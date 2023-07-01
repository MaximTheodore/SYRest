#include "Accountant.h"
#include "Product.h"
#include "MenuItem.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Administrator.h"

void Accountant::SubmitProductRequest(const std::string& productName, int quantity) {
    if (quantity <= 0) {
        std::cout << "�������� ���������� ��������. ���������� ��� ���." << std::endl;
        return;
    }
    productRequests[productName] += quantity;
}

void Accountant::ReceiveProductDelivery(const std::string& productName, int quantity) {
    if (quantity <= 0) {
        std::cout << "�������� ���������� ��������. ���������� ��� ���." << std::endl;
        return;
    }
    productDeliveries[productName] += quantity;
}

void Accountant::GenerateFinancialReport(const std::map<std::string, std::shared_ptr<Product>>& products) {
    double totalRevenue = 0.0;
    double totalExpenses = 0.0;

    std::ofstream outputFile("operations.txt");
    if (outputFile.is_open()) {
        outputFile << "���������� �����:" << std::endl;
        outputFile << "-----------------" << std::endl;

        for (const auto& pair : products) {
            double productRevenue = pair.second->GetCost() * pair.second->GetQuantity();
            outputFile << "�������: " << pair.second->GetName() << ", �������: " << pair.second->GetQuantity() << ", �������: $" << productRevenue << std::endl;

            totalRevenue += productRevenue;
            totalExpenses += Product::GetProductPriceFromFile(pair.second->GetName()) * pair.second->GetQuantity();
        }

        outputFile << "-----------------" << std::endl;
        outputFile << "����� �������: $" << totalExpenses << std::endl;
        outputFile << "������: $" << totalRevenue - totalExpenses << std::endl;
        outputFile.close();

        std::cout << "���������� ����� �������� � ����� 'operations.txt'." << std::endl;
        std::cout << "����� �������: $" << totalExpenses << std::endl;
        std::cout << "������: $" << totalRevenue - totalExpenses << std::endl;
    }
    else {
        std::cout << "�� ������� ������� ���� ��� ���������� ����������� ������." << std::endl;
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
            std::cout << "�� ������� ������� ���� ��� ������ ���������� � �������." << std::endl;
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
        std::cout << "�� ������� ������� ���� ��� ������ ���������� � �������." << std::endl;
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
        std::cout << "�� ������� ������� ���� � ������� � ��������� ������." << std::endl;
        return;
    }

    std::ofstream outputFile("sold_items_report.txt", std::ios::app);
    if (outputFile.is_open()) {
        outputFile << "����� � ��������� ������:" << std::endl;
        outputFile << "------------------------" << std::endl;

        if (!soldItems.empty()) {
            for (const auto& pair : soldItems) {
                outputFile << "�����: " << pair.first << ", �������: " << pair.second << std::endl;
            }

            outputFile << "------------------------" << std::endl;
        }
        else {
            outputFile << "��� ������ � ��������� ������." << std::endl;
        }
        outputFile.close();
        std::cout << "����� � ��������� ������ �������� � ����� 'sold_items_report.txt'." << std::endl;
        std::cout << "����� � ��������� ������:" << std::endl;

        if (!soldItems.empty()) {
            for (const auto& pair : soldItems) {
                std::cout << "�����: " << pair.first << ", �������: " << pair.second << std::endl;
            }
        }
        else {
            std::cout << "��� ������ � ��������� ������." << std::endl;
        }
    }
    else {
        std::cout << "�� ������� ������� ���� ��� ���������� ������ � ��������� ������." << std::endl;
    }
}

void Accountant::GenerateSalesReport(const std::map<int, std::shared_ptr<MenuItem>>& menuItems) {
    double totalSales = 0.0;

    std::ofstream outputFile("operations.txt", std::ios::app);
    if (outputFile.is_open()) {
        outputFile << "����� � ��������:" << std::endl;
        outputFile << "-----------------" << std::endl;

        for (const auto& pair : menuItems) {
            std::shared_ptr<MenuItem> menuItem = pair.second;
            std::string itemName = menuItem->GetName();
            int quantity = menuItem->GetQuantity();
            double itemRevenue = menuItem->GetPrice() * quantity;

            outputFile << "�����: " << itemName << ", �������: " << quantity << ", �������: $" << itemRevenue << std::endl;

            totalSales += itemRevenue;
        }

        outputFile << "-----------------" << std::endl;
        outputFile << "����� �������: $" << totalSales << std::endl;
        outputFile.close();

        std::cout << "����� � �������� �������� � ����� 'operations.txt'." << std::endl;
        std::cout << "����� � ��������:" << std::endl;

        for (const auto& pair : menuItems) {
            std::shared_ptr<MenuItem> menuItem = pair.second;
            std::string itemName = menuItem->GetName();
            int quantity = menuItem->GetQuantity();
            double itemRevenue = menuItem->GetPrice() * quantity;

            std::cout << "�����: " << itemName << ", �������: " << quantity << ", �������: $" << itemRevenue << std::endl;
        }

        std::cout << "����� �������: $" << totalSales << std::endl;
    }
    else {
        std::cout << "�� ������� ������� ���� ��� ���������� ������ � ��������." << std::endl;
    }
}

void Accountant::run() {
    Administrator::AuditIn("���������� ���� �� ����������");
    std::cout << "����� ����������, ���������!" << std::endl;

    while (true) {
        std::cout << std::endl;
        std::cout << "�������� ��������:" << std::endl;
        std::cout << "1. ������� ������ ������������ ������ �� ��������" << std::endl;
        std::cout << "2. ������� ������ �������� �������� ���������" << std::endl;
        std::cout << "3. ������������� ���������� �����" << std::endl;
        std::cout << "4. ������������� ����� � ��������� ������" << std::endl;
        std::cout << "5. ����� �� ���������" << std::endl;

        int choice;
        std::cin >> choice;
        if (choice == 1) {
            Administrator::AuditIn("������ ������ ������������ ������ �� ��������");
            PrintProductRequests();
        }
        else if (choice == 2) {
            Administrator::AuditIn("������ ������ �������� �������� ���������");
            PrintProductDeliveries();
        }
        else if (choice == 3) {
            std::map<std::string, std::shared_ptr<Product>> products;
            Administrator::AuditIn("������������ ���������� �����");
            GenerateFinancialReport(products);
        }
        else if (choice == 4) {
            std::map<int, std::shared_ptr<MenuItem>> menuItems;
            Administrator::AuditIn(" ������������ ����� � ��������� ������");
            GenerateSalesReport(menuItems);
        }
        else if (choice == 5) {
            Administrator::AuditIn("����� �� ����������");
            std::cout << "�� ��������!" << std::endl;
            break;
        }
        else {
            std::cout << "�������� �����. ���������� ��� ���." << std::endl;
        }
    }
}

