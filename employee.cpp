#include "employee.h"

Employee::Employee(int id, const std::string& name, int min_hours, int max_hours)
    : id(id), name(name), min_hours(min_hours), max_hours(max_hours) {
}

int Employee::getId() const {
    return id;
}

std::string Employee::getName() const {
    return name;
}

int Employee::getMinHours() const {
    return min_hours;
}

int Employee::getMaxHours() const {
    return max_hours;
}

void Employee::setName(const std::string& name) {
    this->name = name;
}

void Employee::setMinHours(int min_hours) {
    this->min_hours = min_hours;
}

void Employee::setMaxHours(int max_hours) {
    this->max_hours = max_hours;
}
