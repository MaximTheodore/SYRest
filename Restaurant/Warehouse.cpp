#include "Warehouse.h"
#include "Administrator.h"
#include <iostream>
#include <iomanip>
#include "Provider.h"
#include "Product.h"
#include "Accountant.h"
std::map<std::string, int> Warehouse::inventory; 
double Warehouse::GetProductPrice(const std::string& productName) const {
    if (prices.find(productName) != prices.end()) {
        return prices.at(productName);
    }
    else {
        return -1.0;
    }
}
void Warehouse::RemoveProduct(const std::string& productName, int quantity) {
    inventory[productName] += quantity;

    std::map<int, std::shared_ptr<Product>> productMap;

    // Загрузка данных из файла "products.txt" в productMap
    Product::LoadFromFile("products.txt", productMap);

    // Обновление количества товара в productMap
    for (auto& pair : productMap) {
        if (pair.second->GetName() == productName) {
            pair.second->SetQuantity(pair.second->GetQuantity() - quantity);
            break; // Прекратить поиск, если товар найден
        }
    }

    // Сохранение обновленных данных в файл "products.txt"
    Product::SaveToFile("products.txt", productMap);
   
}
int Warehouse::GetProductId(const std::string& productName) {
    for (const auto& pair : inventory) {
        if (pair.first == productName) {
            return pair.second;
        }
    }
    return -1;  // Возвращаем -1, если продукт не найден
}
double Warehouse::GetProductCost(const std::string& productName) {
    for (const auto& pair : inventory) {
        if (pair.first == productName) {
            return pair.second;
        }
    }
    return 0.0;  // Возвращаем 0.0, если продукт не найден
}




void Warehouse::AddProduct(const std::string& productName, int quantity) {
    inventory[productName] += quantity;

    std::map<int, std::shared_ptr<Product>> productMap;

    // Загрузка данных из файла "products.txt" в productMap
    Product::LoadFromFile("products.txt", productMap);

    // Обновление количества товара в productMap
    for (auto& pair : productMap) {
        if (pair.second->GetName() == productName) {
            pair.second->SetQuantity(pair.second->GetQuantity() + quantity);
            break; // Прекратить поиск, если товар найден
        }
    }
    // Сохранение обновленных данных в файл "products.txt"
    Product::SaveToFile("products.txt", productMap);
}
void Warehouse::DisplayInventory() const {
    std::cout << "Складские запасы:\n";
    for (const auto& item : inventory) {
        std::cout << item.first << ": " << item.second << std::endl;
    }
}
bool Warehouse::CanSendOrder(const std::map<std::string, int>& order, double balance) const {
    double totalCost = 0.0;
    for (const auto& item : order) {
        const std::string& productName = item.first;
        int quantity = item.second;

        if (inventory.find(productName) == inventory.end()) {
            std::cout << "Продукт \"" << productName << "\" отсутствует на складе.\n";
            return false;
        }

        int availableQuantity = inventory.at(productName);
        if (quantity > availableQuantity) {
            std::cout << "Недостаточное количество продукта \"" << productName << "\" на складе.\n";
            return false;
        }

        if (prices.find(productName) == prices.end()) {
            std::cout << "Цена продукта \"" << productName << "\" не найдена.\n";
            return false;
        }

        double productPrice = prices.at(productName);
        totalCost += productPrice * quantity;
    }

    if (balance < totalCost) {
        std::cout << "Недостаточный баланс денежных средств для отправки заявки.\n";
        return false;
    }

    return true;
}

void Warehouse::LoadProducts(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cout << "Ошибка при открытии файла: " << filename << std::endl;
        return;
    }

    inventory.clear();
    prices.clear();
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        int id, cost, quantity;
        std::string name;
        if (iss >> id >> name >> cost >> quantity) {
            inventory[name] = quantity;
            prices[name] = cost;
        }
    }

    file.close();
}

void Warehouse::SaveProducts(const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        std::cout << "Ошибка при открытии файла для записи: " << filename << std::endl;
        return;
    }

    for (const auto& item : inventory) {
        const std::string& productName = item.first;
        int quantity = item.second;
        if (prices.find(productName) != prices.end()) {
            double price = prices[productName];
            file << productName << " " << price << " " << quantity << std::endl;
        }
    }

    file.close();
}


void Warehouse::Run() {
        Administrator::AuditIn("Произведен вход за кладовщика");
        std::map<int, std::shared_ptr<Product>> products;
        std::map<std::string, std::shared_ptr<Product>> pro;
        std::map<std::string, std::shared_ptr<Employee>> employees;
        std::shared_ptr<MenuItem> menu = std::make_shared<MenuItem>();
        Administrator admin{ products, employees, menu };
        Accountant accountant;
     

        for (const auto& pair : products) {
            int key = pair.first;
            std::shared_ptr<Product> value = pair.second;
            pro[std::to_string(key)] = value;
        }
        std::cout << "Warehouse Management System" << std::endl;

        while (true) {
          
            std::cout << "1. Добавить продукт" << std::endl;
            std::cout << "2. Удалить продукт" << std::endl;
            std::cout << "3. Изменить количество продуктов" << std::endl;
            std::cout << "4. Посмотреть склад" << std::endl;
            std::cout << "5. Отправить заявку на продукты" << std::endl;
            std::cout << "6. Выход" << std::endl;
            std::cout << "Выбор: ";
            int choice;
            std::cin >> choice;

            switch (choice) {
            case 1: {
                int id;
                std::string name;
                double cost;
                int quantity;

                std::cout << "Введите ID продукта: ";
                std::cin >> id;
                std::cout << "Введите имя продукта : ";
                std::cin.ignore();
                std::getline(std::cin, name);
                std::cout << "Введите стоимость продукта: ";
                std::cin >> cost;
                std::cout << "Введите количество продукта: ";
                std::cin >> quantity;

                admin.AddProduct(products, id, name, cost, quantity);
                break;
            }
            case 2: {
                int id;
                std::cout << "Введите ID продукта: ";
                std::cin >> id;

                admin.RemoveProduct(id);
                std::cout << "Продукт успешно удален." << std::endl;
                Administrator::AuditIn("Продукт по ID " + std::to_string(id) + " успешно удален");
                break;
            }
            case 3: {
                int id;
                std::cout << "Введите ID продукта: ";
                std::cin >> id;

                admin.UpdateProduct(id);
                std::cout << "Продукт успешно изменен." << std::endl;
                Administrator::AuditIn("Продукт по ID " + std::to_string(id) + " успешно обновлен");
                break;
            }
            case 4: {
                std::map<int, std::shared_ptr<Product>> products1;

                Product::LoadFromFile("products.txt", products1);
                admin.ShowProducts(products1);
                break;
            }
            case 5: {
                std::map<std::string, int> order;
                double balance;
                LoadProducts("products.txt");

                std::string orderFilename = "OrderProduct.txt";
                std::ofstream orderFile(orderFilename);
                if (!orderFile) {
                    std::cout << "Ошибка в открытии order файла." << std::endl;
                    break;
                }

                std::cout << "Введите детали заказа:" << std::endl;
                std::cout << "Имя продукта (-1 to stop): ";
                std::string productName;
                std::cin >> productName;

                while (productName != "-1") {
                    int quantity;
                    std::cout << "Количество: ";
                    std::cin >> quantity;

                    order[productName] = quantity;

                    orderFile << std::setw(15) << std::left << productName << quantity << std::endl;

                    std::cout << "Имя продукта (-1 to stop): ";
                    std::cin >> productName;
                    Administrator::AuditIn("Заявка на продукт " + productName + " в количесте " + std::to_string(quantity));
                }

                orderFile.close();

                std::cout << "Введите балансе: ";
                std::cin >> balance;
                if (CanSendOrder(order, balance)) {
                    std::cout << "Заявка на продукты может быть отправлена." << std::endl;
                    Administrator::AuditIn("Заявка на продукты может быть отправлена.");

                    Provider provider;
                    provider.ProcessOrder(order, balance);
                    provider.ViewOrder(orderFilename);
                }
                accountant.GenerateFinancialReport(pro);
                break;
            }
            case 6: {
                SaveProducts("products.txt");
                Administrator::AuditIn("Выход кладовщика");
                std::cout << "Выход..." << std::endl;
                return;
            }
            default: {
                std::cout << "Неправильный выбор." << std::endl;
                break;
            }
            }
        }
    }



std::map<std::string, std::shared_ptr<Product>> Warehouse::GetProducts()  {
      std::map<std::string, std::shared_ptr<Product>> products;

      std::map<int, std::shared_ptr<Product>> productMap;
        Product::LoadFromFile("products.txt", productMap);

    for (const auto& pair : productMap) {
            int id = pair.first;
            std::shared_ptr<Product> product = pair.second;
            products[std::to_string(id)] = product;
     }

    return products;
}
