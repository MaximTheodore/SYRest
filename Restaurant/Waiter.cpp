#include "Waiter.h"
#include "Order.h"
#include <iostream>
#include "Administrator.h"
void Waiter::Run() {
    Administrator::AuditIn("���������� ���� �� ���������");
    std::cout << "����� ����������, ��������!" << std::endl;

    OrderStatus status = Order::LoadOrderStatus();
    if (status == OrderStatus::READY_FOR_DELIVERY) {
        std::cout << "�����, ������� ��� ��������. �������� ��� ��� �����������? (y/n): ";
        char choice;
        std::cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            std::cout << "�������� ������ ��� �����������..." << std::endl;
            Administrator::AuditIn("�������� ������ ��� ����������� ����������");
            status = OrderStatus::COMPLETED;
            Order::SaveOrderStatus(status);
            std::cout << "����� ������� ��� ��������" << std::endl;
            Administrator::AuditIn("����� ������� ��� �������� ����������");
        }
    }
    else {
        std::cout << "��� ������� ������� ��� ��������." << std::endl;
    }
    Administrator::AuditIn("�������� �����");

    std::cout << "����, ��������!" << std::endl;
}