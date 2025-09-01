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
    std::vector<int> allowed_store_ids;  // Which stores this employee can work at

public:
    // Constructor
    Employee(int id, const std::string& name, int min_hours, int max_hours, const std::vector<int>& allowed_store_ids);
    
    // Getters
    int getId() const;
    std::string getName() const;
    int getMinHours() const;
    int getMaxHours() const;
    const std::vector<int>& getAllowedStoreIds() const;
    
    // Setters
    void setName(const std::string& name);
    void setMinHours(int min_hours);
    void setMaxHours(int max_hours);
    void setAllowedStoreIds(const std::vector<int>& allowed_store_ids);
    
    // Utility methods
    bool canWorkAtStore(int store_id) const;
};

#endif // EMPLOYEE_H
