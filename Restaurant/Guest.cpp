#include "Guest.h"
#include "Order.h"
#include <fstream>
#include "Accountant.h"
#include "Administrator.h"

Guest::Guest() {}

Guest::Guest(const std::map<int, std::shared_ptr<MenuItem>>& menuItems)
    : menuItems(menuItems) {}

void Guest::Run() {
    std::cout << "����� ����������, �����!" << std::endl;

    MenuItem::LoadFromFile("menu_items.txt", menuItems);
    std::map<std::string, std::shared_ptr<Product>> products; // ���������� ���������� "products"

    while (true) {
        std::cout << std::endl;
        std::cout << "1. ������� ����" << std::endl;
        std::cout << "2. ������� �����" << std::endl;
        std::cout << "3. ���������� ������ ������" << std::endl;
        std::cout << "4. �����" << std::endl;

        int choice;
        std::cout << "����������� ���� �����: ";
        std::cin >> choice;

        if (choice == 1) {
            Administrator::AuditIn("����������� ���� ������� � �����");
            DisplayMenu();
        }
        else if (choice == 2) {
            Administrator::AuditIn("����� ������ �����");
            MakeOrder(products); // �������� ���������� "products" � ������� MakeOrder
        }
        else if (choice == 3) {
            Administrator::AuditIn("����� ������� ������ ������");
            TrackOrderStatus();
        }
        else if (choice == 4) {
            break;
        }
        else {
            std::cout << "������������ �����";
        }
    }
    Administrator::AuditIn("����� ����");
    std::cout << "������� �� �����! ��������!" << std::endl;
}

void Guest::MakeOrder(std::map<std::string, std::shared_ptr<Product>>& products) {
    DisplayMenu();
    int itemId, quantity;

    std::cout << "������� ID ������, ������� �� ������ �������� (-1 ��� ����������): ";
    std::cin >> itemId;

    Accountant accountant; // �������� ���������� ������ Accountant
    std::map<int, std::shared_ptr<MenuItem>> Items;

    while (itemId != -1) {
        if (menuItems.find(itemId) != menuItems.end()) {
            std::cout << "������� ����������: ";
            std::cin >> quantity;

            if (quantity > 0 && quantity <= menuItems.at(itemId)->GetQuantity()) {
                std::shared_ptr<MenuItem> selectedItem = menuItems.at(itemId);
                selectedItem->SetQuantity(selectedItem->GetQuantity() - quantity);

                for (int i = 0; i < quantity; ++i) {
                    currentOrder.AddItem(selectedItem);
                }
                std::cout << "����� �������� � �����." << std::endl;
                Administrator::AuditIn("������ ����� " + selectedItem->GetName() + " �� ����������" + std::to_string(selectedItem->GetQuantity()));

                // ���������� ���������� ��������� � products
                UpdateProductQuantities(products, selectedItem, quantity);
                UpdateProductFile(products);
                Items.emplace(itemId, selectedItem);
                accountant.GenerateSalesReport(Items);  // ����� ������ GenerateSalesReport ������ Accountant
                accountant.GetSoldItemsReport();
            }
            else {
                std::cout << "������������� ����������. ����������, �������� ������� ����������." << std::endl;
            }
        }
        else {
            std::cout << "�������� ID ������. ����������, ���������� �����." << std::endl;
        }

        std::cout << "������� ID ������, ������� �� ������ �������� (-1 ��� ����������): ";
        std::cin >> itemId;
    }

    if (!currentOrder.GetBasket().empty()) {
        if (currentOrder.ConfirmOrder()) {
            std::cout << "����� ����������� � �������. ����� ������� ���� ������� � ������ �������." << std::endl;
            Administrator::AuditIn("����� ����������� � ������� ������");

            currentOrder.SetStatus(OrderStatus::PAID_AND_SENT_TO_KITCHEN);
            Order::SaveOrderStatus(OrderStatus::PAID_AND_SENT_TO_KITCHEN);

            // Write order details to receipt.txt
            std::ofstream outputFile("receipt.txt");
            outputFile.clear();
            if (outputFile.is_open()) {
                outputFile << "����� ������:" << std::endl;
                for (const auto& item : currentOrder.GetBasket()) {
                    outputFile << "��������: " << item->GetName() << std::endl;
                    outputFile << "����: $" << item->GetPrice() << std::endl;
                    outputFile << "����������: " << item->GetQuantity() << std::endl;
                    outputFile << "------------------------" << std::endl;
                }
                outputFile.close();
                std::cout << "�������� ������ ���� �������� � ���� receipt.txt" << std::endl;
            }
            else {
                std::cout << "�� ������� ������� ���� receipt.txt ��� ������ �������� �������." << std::endl;
            }

        }
        else {
            std::cout << "����� �������. ������������ ������� �� ����� �������." << std::endl;
            // �������������� ���������� ��������� � ������ ������ ������
            RestoreProductQuantities(products);
            currentOrder.ClearBasket();
        }
    }
}

void Guest::UpdateProductQuantities(std::map<std::string, std::shared_ptr<Product>>& products, const std::shared_ptr<MenuItem>& item, int quantity) {
    Administrator::AuditIn("�������� ��������� �������� ���� �� " + std::to_string(quantity));

    for (const auto& product : products) {
        if (product.second->GetName() == item->GetName()) {
            product.second->SetQuantity(product.second->GetQuantity() - quantity);
            break;
        }
    }
}
//���������� ���������� ���������
void Guest::RestoreProductQuantities(std::map<std::string, std::shared_ptr<Product>>& products) {
    for (const auto& item : currentOrder.GetBasket()) {
        std::string itemName = item->GetName();
        if (products.find(itemName) != products.end()) {
            products[itemName]->SetQuantity(products[itemName]->GetQuantity() + 1);
        }
    }
}
//��������� ����� ���������
void Guest::UpdateProductFile(const std::map<std::string, std::shared_ptr<Product>>& products) {
    std::ofstream outputFile("products.txt");
    if (outputFile.is_open()) {
        for (const auto& product : products) {
            outputFile << product.second->GetId() << " "
                << product.second->GetName() << " "
                << product.second->GetCost() << " "
                << product.second->GetQuantity() << std::endl;
        }
        outputFile.close();
    }
    else {
        std::cout << "���� �� ��������" << std::endl;
    }
}
void Guest::DisplayMenu() const {
    std::cout << "Menu:" << std::endl;
    for (const auto& pair : menuItems) {
        std::cout << pair.first << ". " << pair.second->GetName() << " - $"
            << pair.second->GetPrice() << " (����������: " << pair.second->GetQuantity()
            << ")" << std::endl;
    }
    std::cout << "-------------------------------------" << std::endl;
}

void Guest::TrackOrderStatus() const {
    std::cout << "������ ������: " << Order::GetStatusString(Order::LoadOrderStatus()) << std::endl;
    Order::SaveOrderStatus(currentOrder.GetStatus());
}
