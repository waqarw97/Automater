// employee.h
#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <string>
#include <vector>

class Employee {
private:
    int id;
    std::string name;
    int min_hours;
    int max_hours;

public:
    // Constructor
    Employee(int id, const std::string& name, int min_hours, int max_hours);
    
    // Getters
    int getId() const;
    std::string getName() const;
    int getMinHours() const;
    int getMaxHours() const;
    
    // Setters
    void setName(const std::string& name);
    void setMinHours(int min_hours);
    void setMaxHours(int max_hours);
};

#endif // EMPLOYEE_H
