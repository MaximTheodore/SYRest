#include "Waiter.h"
#include "Order.h"
#include <iostream>
#include "Administrator.h"
void Waiter::Run() {
    Administrator::AuditIn("Произведен вход за официанта");
    std::cout << "Добро пожаловать, официант!" << std::endl;

    OrderStatus status = Order::LoadOrderStatus();
    if (status == OrderStatus::READY_FOR_DELIVERY) {
        std::cout << "Заказ, готовый для отправки. Пометить его как завершенный? (y/n): ";
        char choice;
        std::cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            std::cout << "Потметка заказа как завершенный..." << std::endl;
            Administrator::AuditIn("Потметка заказа как завершенный официантом");
            status = OrderStatus::COMPLETED;
            Order::SaveOrderStatus(status);
            std::cout << "Заказ помечен как завершен" << std::endl;
            Administrator::AuditIn("Заказ помечен как завершен официантом");
        }
    }
    else {
        std::cout << "Нет готовых заказов для отправки." << std::endl;
    }
    Administrator::AuditIn("Официант вышел");

    std::cout << "Пока, официант!" << std::endl;
}