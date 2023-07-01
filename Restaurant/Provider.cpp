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
    std::cout << "��������� ������..." << std::endl;
    double totalCost = 0.0;
    Warehouse warehouse;

    Accountant accountant;

    // �������� ���������� � ��������� ��������� Accountant
    for (const auto& item : order) {
        const std::string& productName = item.first;
        int quantity = item.second;
        accountant.ReceiveProductDelivery(productName, quantity);
    }

    for (const auto& item : order) {
        const std::string& productName = item.first;
        int quantity = item.second;

        // ���������� ����������� �������� ��� ���������� ������, ��������, ����� � ������������, �������� ������� � �.�.
        std::cout << "�����: " << productName << ", ����������: " << quantity << " - ���������" << std::endl;

        // ���������� ��������� �� ������
        warehouse.AddProduct(productName, quantity);

        // ������ ����� ��������� ������
        double productPrice = warehouse.GetProductPrice(productName);
        totalCost += productPrice * quantity;
    }

    // ��������� ����� ��������� �� �������
    balance += totalCost;

    std::cout << "����� ������� ���������. ����� ���������: $" << totalCost << std::endl;
    std::cout << "���������� ������: $" << balance << std::endl;

    // ��������� ����������� ������
    std::map<std::string, std::shared_ptr<Product>> products = warehouse.GetProducts();
    accountant.GenerateFinancialReport(products);
}
std::map<std::string, int> Provider::ViewOrder(const std::string& orderFilename) {
    std::map<std::string, int> order;
    std::ifstream file(orderFilename);
    if (!file) {
        std::cout << "������ ��� �������� �����: " << orderFilename << std::endl;
        return order;
    }

    std::cout << "������ ������ �� ����� \"" << orderFilename << "\":" << std::endl;
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string productName;
        int quantity;
        if (iss >> productName >> quantity) {
            std::cout << "�����: " << productName << ", ����������: " << quantity << std::endl;
            order[productName] = quantity;
        }
    }

    file.close();
    return order;
}
void Provider::RunProvider() {
    Administrator::AuditIn("���������� ���� �� ����������");
    std::map<std::string, int> order;
    double balance = 10000.00;
    std::cout << "������� ����������" << std::endl;
    while (true) {
        std::cout << std::endl;
        std::cout << "1. ����������� �����" << std::endl;
        std::cout << "2. ���������� �����" << std::endl;
        std::cout << "3. �����" << std::endl;
        std::cout << "������� ��� �����: ";
        int choice;
        std::cin >> choice;
        switch (choice) {
        case 1:
            Administrator::AuditIn("�������� ������ �� �������� �����������");
            order = ViewOrder("OrderProduct.txt");
            break;
        case 2:
            if (!order.empty()) {
                Administrator::AuditIn("��������� ������ �� �������� �����������");
                ProcessOrder(order, balance);
                order.clear();
            }
            else {
                std::cout << "��� ������ ��� ���������." << std::endl;
            }
            break;
        case 3:
            Administrator::AuditIn("��������� �����");
            std::cout << "�����..." << std::endl;
            return;
        default:
            std::cout << "������������ �����. ����������, ��������� �������." << std::endl;
            break;
        }
    }
}
