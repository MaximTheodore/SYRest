
#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <map>
#include <string>
#include <memory>

class Employee {
public:
    Employee(const std::string& role, const std::string& lastName, const std::string& firstName,
        const std::string& middleName, const std::string& login, int password);

    static void LoadFromFile(const std::string& filename, std::map<std::string, std::shared_ptr<Employee>>& employees);
    static void SaveToFile(const std::string& filename, const std::map<std::string, std::shared_ptr<Employee>>& employees);

    std::string GetRole() const;
    std::string GetFullName() const;
    std::string GetLogin() const;
    int GetPassword() const;


    void SetRole(const std::string& role);
    void SetLastName(const std::string& lastName);
    void SetFirstName(const std::string& firstName);
    void SetMiddleName(const std::string& middleName);
    void SetLogin(const std::string& Login);
    void SetPassword(int password);

private:
    std::string role;
    std::string lastName;
    std::string firstName;
    std::string middleName;
    std::string login;
    int password;
};

#endif // EMPLOYEE_H
