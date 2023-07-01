#include "Employee.h"
#include <fstream>
#include <iostream>

Employee::Employee(const std::string& role, const std::string& lastName, const std::string& firstName,
    const std::string& middleName, const std::string& login, int password)
    : role(role), lastName(lastName), firstName(firstName), middleName(middleName),
    login(login), password(password) {}
std::string Employee::GetRole() const {
    return role;
}
std::string Employee::GetFullName() const {
    return lastName + " " + firstName + " " + middleName;
}
std::string Employee::GetLogin() const {
    return login;
}
int Employee::GetPassword() const {
    return password;
}
void Employee::SetLogin(const std::string& Login) {
    login = Login;
}
void Employee::SetRole(const std::string& Role) {
    role = Role;
}
void Employee::SetLastName(const std::string& LastName) {
    lastName = LastName;
}
void Employee::SetFirstName(const std::string& FirstName) {
    firstName = FirstName;
}
void Employee::SetMiddleName(const std::string& MiddleName) {
    middleName = MiddleName;
}
void Employee::SetPassword(int Password) {
    password = Password;
}
void Employee::LoadFromFile(const std::string& filename, std::map<std::string, std::shared_ptr<Employee>,
    std::less<std::string>, std::allocator<std::pair<const std::string, std::shared_ptr<Employee>>>>& employees) {
    std::ifstream file(filename);
    if (file.is_open()) {
        employees.clear();
        std::string role, lastName, firstName, middleName, login; int password;

        while (file >> role >> lastName >> firstName >> middleName >> login >> password) {
            std::shared_ptr<Employee> employee = std::make_shared<Employee>(role, lastName, firstName,
                middleName, login, password);
            employees.insert(std::make_pair(login, employee));
        }

        file.close();
    }
    else {
        std::cout << "Ошибка в открытии файла: " << filename << std::endl;
    }
}

void Employee::SaveToFile(const std::string& filename, const std::map<std::string, std::shared_ptr<Employee>,
    std::less<std::string>, std::allocator<std::pair<const std::string, std::shared_ptr<Employee>>>>& employees) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (const auto& pair : employees) {
            file << pair.second->GetRole() << " " << pair.second->GetFullName() << " "
                << pair.second->GetLogin() << " " << pair.second->GetPassword() << std::endl;
        }

        file.close();
    }
    else {
        std::cout << "Ошибка в открытии файла: " << filename << std::endl;
    }
}
