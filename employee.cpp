#include "employee.h"
#include <algorithm>

Employee::Employee(int id, const std::string& name, int min_hours, int max_hours, const std::vector<int>& allowed_store_ids)
    : id(id), name(name), min_hours(min_hours), max_hours(max_hours), allowed_store_ids(allowed_store_ids) {
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

const std::vector<int>& Employee::getAllowedStoreIds() const {
    return allowed_store_ids;
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

void Employee::setAllowedStoreIds(const std::vector<int>& allowed_store_ids) {
    this->allowed_store_ids = allowed_store_ids;
}

bool Employee::canWorkAtStore(int store_id) const {
    return std::find(allowed_store_ids.begin(), allowed_store_ids.end(), store_id) != allowed_store_ids.end();
}
