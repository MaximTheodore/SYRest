#include "Cook.h"
#include <iostream>
#include <fstream>
#include "Product.h"
#include "MenuItem.h"
#include <sstream>
#include "Warehouse.h"
#include "Administrator.h"

void Cook::Run() {
    Administrator::AuditIn("���������� ���� �� ������");
    std::cout << "����� ����������, �����!" << std::endl;

    OrderStatus status = Order::LoadOrderStatus();
    if (status == OrderStatus::PAID_AND_SENT_TO_KITCHEN) {
        std::ifstream receiptFile("receipt.txt");
        if (receiptFile.is_open()) {
            std::string line;
            while (std::getline(receiptFile, line)) {
                std::cout << line << std::endl;
            }
            receiptFile.close();

            int ch;
            std::cout << "1. ������ ��������\n";
            std::cout << "��������: ";
            std::cin >> ch;

            switch (ch) {
            case 1: {
                std::cout << "���� �����. ������ ��������? (y/n): ";
                char choice;
                std::cin >> choice;
                if (choice == 'y' || choice == 'Y') {
                    Administrator::AuditIn("����� ����� ��������");
                    std::ifstream menuItemsFile("menu_items.txt");
                    if (menuItemsFile.is_open()) {
                        std::map<std::string, std::shared_ptr<MenuItem>> menuItems;
                        MenuItem::LoadFromFileForCook("menu_items.txt", menuItems);

                        for (const auto& pair : menuItems) {
                            std::cout << pair.second->GetName() << " - " << "����������� ��� �������������\n"
                                << MenuItem::GetItemsIngredients(pair.second->GetIngredients()) << std::endl;
                        }

                        std::cout << "������� �������� ������� ����, ����� ����������� �� (��������� �������� ���������): ";
                        std::string menuItemsNames;
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::getline(std::cin, menuItemsNames);

                        std::stringstream ss("menu_items.txt");
                        std::string menuItemName;
                        bool allMenuItemsAvailable = true;
                        while (ss >> menuItemName) {
                            menuItemName = menuItemsNames;
                            auto it = menuItems.find(menuItemName);
                            if (it != menuItems.end()) {
                                std::shared_ptr<MenuItem> menuItem = it->second;
                                if (menuItem->GetQuantity() > 0) {
                                    menuItem->SetQuantity(menuItem->GetQuantity() - 1);
                                    std::vector<std::shared_ptr<Product>> itemIngredients = menuItem->GetIngredients();

                                    for (const auto& ingredient : itemIngredients) {
                                        
                                        Warehouse::RemoveProduct(ingredient->GetName(), 1);
                                    }
                                }
                                else {
                                    std::cout << "����� ���� � ��������� \"" << menuItemName << "\" ����������." << std::endl;
                                    allMenuItemsAvailable = false;
                                    break;
                                }
                            }
                            else {
                                std::cout << "����� ���� � ��������� \"" << menuItemName << "\" �� ������." << std::endl;
                                allMenuItemsAvailable = false;
                                break;
                            }
                        }

                        if (allMenuItemsAvailable) {
                            std::cout << "������� �������������..." << std::endl;

                            std::cout << "��� ������ ������ ������������. ����� ������� ��������� �� ������." << std::endl;
                            Administrator::AuditIn("����� ��� ����������");
                            status = OrderStatus::READY_FOR_DELIVERY;
                            Order::SaveOrderStatus(status);
                        }

                        MenuItem::SaveToFileForCook("menu_items.txt", menuItems);
                    }
                    else {
                        std::cout << "������ � �������� �����: menu_items.txt" << std::endl;
                    }
                }
                break;
            }
            default:
                status = OrderStatus::IN_PROCESS;
                Order::SaveOrderStatus(status);
                break;
            }
        }
        else {
            Administrator::AuditIn("������ � �������� �����: receipt.txt");

            std::cout << "������ � �������� �����: receipt.txt" << std::endl;
        }
    }
    else {
        std::cout << "��� ������� ��� ��������." << std::endl;
    }
    Administrator::AuditIn("����� ������� �������");

    std::cout << "��������, �����!" << std::endl;
}
