#include "Order.h"
#include <iostream>
#include <fstream>
#include "Administrator.h"

void Order::AddToBasket(std::shared_ptr<MenuItem> item) {
    basket.push_back(item);
}

void Order::AddItem(const std::shared_ptr<MenuItem>& item) {
    basket.push_back(item);
}

double Order::CalculateTotal() const {
    double total = 0.0;
    for (const auto& item : basket) {
        total += item->GetPrice();
    }
    return total;
}

bool Order::ConfirmOrder() const {
    std::cout << "Подтверждение заказа..." << std::endl;
    std::cout << "Успешная оплата!" << std::endl;
    return true;
}

void Order::TrackOrderStatus() const {
    std::cout << "Статус заказа: " << GetStatusString(status) << std::endl;
}

void Order::GenerateReceipt(const std::string& filename) const {
    std::ofstream receiptFile(filename);
    if (receiptFile.is_open()) {
        receiptFile << "Receipt" << std::endl;
        receiptFile << "-----------------" << std::endl;
        for (const auto& item : basket) {
            receiptFile << item->GetName() << " - $" << item->GetPrice() << std::endl;
        }
        receiptFile << "-----------------" << std::endl;
        receiptFile << "Total: $" << CalculateTotal() << std::endl;
        receiptFile.close();
        std::cout << "Заказ сохранен в " << filename << std::endl;
        Administrator::AuditIn("Заказ сохранен в " + filename);
    }
    else {
        std::cout << "Ошибка в открытии файла." << std::endl;
    }
}

const std::vector<std::shared_ptr<MenuItem>>& Order::GetBasket() const {
    return basket;
}

void Order::ClearBasket() {
    basket.clear();
}

void Order::SetStatus(OrderStatus status) {
    this->status = status;
}

void Order::SaveOrder(const std::string& filename) const {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << basket.size() << std::endl;
        for (const auto& item : basket) {
            file << item->GetName() << std::endl;
            file << item->GetQuantity() << std::endl;
            file << item->GetPrice() << std::endl;
        }
        file.close();
        std::cout << "Заказ сохранен в файл " << filename << std::endl;
        Administrator::AuditIn("Заказ сохранен в " + filename);
    }
    else {
        std::cout << "Ошибка в открытии файла." << std::endl;
    }
}

std::string Order::GetStatusString(OrderStatus status) {
    switch (status) {
    case OrderStatus::PAID_AND_SENT_TO_KITCHEN:
        return "Оплачен и отправлен на кухню";
    case OrderStatus::IN_PROCESS:
        return "В процессе";
    case OrderStatus::READY_FOR_DELIVERY:
        return "Готов к отправке";
    case OrderStatus::COMPLETED:
        return "Завершен";
    case OrderStatus::UNKNOWN:
        return "Неизвестен";
    }
}

OrderStatus Order::GetStatus() const {
    return status;
}
///

void Order::SaveOrderStatus(OrderStatus status) {
    std::ofstream file("status.txt");
    file.clear();
    if (file.is_open()) {
        file << static_cast<int>(status) << std::endl;
        file.close();
    }
    else {
        std::cout << "Ошибка в открытии файла." << std::endl;
    }
}

OrderStatus Order::LoadOrderStatus() {
    std::ifstream file("status.txt");
    if (file.is_open()) {
        int status;
        file >> status;
        file.close();
        return static_cast<OrderStatus>(status);
    }
    else {
        std::cout << "Ошибка в открытии файла." << std::endl;
        return OrderStatus::UNKNOWN;
    }
}
///