#include "Guest.h"
#include "Order.h"
#include <fstream>
#include "Accountant.h"
#include "Administrator.h"

Guest::Guest() {}

Guest::Guest(const std::map<int, std::shared_ptr<MenuItem>>& menuItems)
    : menuItems(menuItems) {}

void Guest::Run() {
    std::cout << "Добро пожаловать, гость!" << std::endl;

    MenuItem::LoadFromFile("menu_items.txt", menuItems);
    std::map<std::string, std::shared_ptr<Product>> products; // Объявление переменной "products"

    while (true) {
        std::cout << std::endl;
        std::cout << "1. Открыть меню" << std::endl;
        std::cout << "2. Сделать заказ" << std::endl;
        std::cout << "3. Посмотреть статус заказа" << std::endl;
        std::cout << "4. Выход" << std::endl;

        int choice;
        std::cout << "Подтвердите свой выбор: ";
        std::cin >> choice;

        if (choice == 1) {
            Administrator::AuditIn("Открывается меню заказов у гостя");
            DisplayMenu();
        }
        else if (choice == 2) {
            Administrator::AuditIn("Гость делает заказ");
            MakeOrder(products); // Передача переменной "products" в функцию MakeOrder
        }
        else if (choice == 3) {
            Administrator::AuditIn("Гость смотрит статус заказа");
            TrackOrderStatus();
        }
        else if (choice == 4) {
            break;
        }
        else {
            std::cout << "Неправильный выбор";
        }
    }
    Administrator::AuditIn("Гость ушел");
    std::cout << "Спасибо за визит! прощайте!" << std::endl;
}

void Guest::MakeOrder(std::map<std::string, std::shared_ptr<Product>>& products) {
    DisplayMenu();
    int itemId, quantity;

    std::cout << "Введите ID товара, который вы хотите заказать (-1 для завершения): ";
    std::cin >> itemId;

    Accountant accountant; // Создание экземпляра класса Accountant
    std::map<int, std::shared_ptr<MenuItem>> Items;

    while (itemId != -1) {
        if (menuItems.find(itemId) != menuItems.end()) {
            std::cout << "Введите количество: ";
            std::cin >> quantity;

            if (quantity > 0 && quantity <= menuItems.at(itemId)->GetQuantity()) {
                std::shared_ptr<MenuItem> selectedItem = menuItems.at(itemId);
                selectedItem->SetQuantity(selectedItem->GetQuantity() - quantity);

                for (int i = 0; i < quantity; ++i) {
                    currentOrder.AddItem(selectedItem);
                }
                std::cout << "Товар добавлен в заказ." << std::endl;
                Administrator::AuditIn("Сделан заказ " + selectedItem->GetName() + " на количество" + std::to_string(selectedItem->GetQuantity()));

                // Обновление количества продуктов в products
                UpdateProductQuantities(products, selectedItem, quantity);
                UpdateProductFile(products);
                Items.emplace(itemId, selectedItem);
                accountant.GenerateSalesReport(Items);  // Вызов метода GenerateSalesReport класса Accountant
                accountant.GetSoldItemsReport();
            }
            else {
                std::cout << "Недостаточное количество. Пожалуйста, выберите меньшее количество." << std::endl;
            }
        }
        else {
            std::cout << "Неверный ID товара. Пожалуйста, попробуйте снова." << std::endl;
        }

        std::cout << "Введите ID товара, который вы хотите заказать (-1 для завершения): ";
        std::cin >> itemId;
    }

    if (!currentOrder.GetBasket().empty()) {
        if (currentOrder.ConfirmOrder()) {
            std::cout << "Заказ подтвержден и оплачен. Сумма платежа была списана с вашего баланса." << std::endl;
            Administrator::AuditIn("Заказ подтвержден и оплачен гостем");

            currentOrder.SetStatus(OrderStatus::PAID_AND_SENT_TO_KITCHEN);
            Order::SaveOrderStatus(OrderStatus::PAID_AND_SENT_TO_KITCHEN);

            // Write order details to receipt.txt
            std::ofstream outputFile("receipt.txt");
            outputFile.clear();
            if (outputFile.is_open()) {
                outputFile << "Заказ детали:" << std::endl;
                for (const auto& item : currentOrder.GetBasket()) {
                    outputFile << "Название: " << item->GetName() << std::endl;
                    outputFile << "Цена: $" << item->GetPrice() << std::endl;
                    outputFile << "Количество: " << item->GetQuantity() << std::endl;
                    outputFile << "------------------------" << std::endl;
                }
                outputFile.close();
                std::cout << "Заказные детали были записаны в файл receipt.txt" << std::endl;
            }
            else {
                std::cout << "Не удалось открыть файл receipt.txt для записи заказных деталей." << std::endl;
            }

        }
        else {
            std::cout << "Заказ отменен. Недостаточно средств на вашем балансе." << std::endl;
            // Восстановление количества продуктов в случае отмены заказа
            RestoreProductQuantities(products);
            currentOrder.ClearBasket();
        }
    }
}

void Guest::UpdateProductQuantities(std::map<std::string, std::shared_ptr<Product>>& products, const std::shared_ptr<MenuItem>& item, int quantity) {
    Administrator::AuditIn("Изменено колчество элемента меню на " + std::to_string(quantity));

    for (const auto& product : products) {
        if (product.second->GetName() == item->GetName()) {
            product.second->SetQuantity(product.second->GetQuantity() - quantity);
            break;
        }
    }
}
//перезапись количества продуктов
void Guest::RestoreProductQuantities(std::map<std::string, std::shared_ptr<Product>>& products) {
    for (const auto& item : currentOrder.GetBasket()) {
        std::string itemName = item->GetName();
        if (products.find(itemName) != products.end()) {
            products[itemName]->SetQuantity(products[itemName]->GetQuantity() + 1);
        }
    }
}
//изменение файла продуктов
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
        std::cout << "Файл не доступен" << std::endl;
    }
}
void Guest::DisplayMenu() const {
    std::cout << "Menu:" << std::endl;
    for (const auto& pair : menuItems) {
        std::cout << pair.first << ". " << pair.second->GetName() << " - $"
            << pair.second->GetPrice() << " (Количество: " << pair.second->GetQuantity()
            << ")" << std::endl;
    }
    std::cout << "-------------------------------------" << std::endl;
}

void Guest::TrackOrderStatus() const {
    std::cout << "Статус заказа: " << Order::GetStatusString(Order::LoadOrderStatus()) << std::endl;
    Order::SaveOrderStatus(currentOrder.GetStatus());
}
