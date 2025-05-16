#include "shift.h"
#include <algorithm>

Shift::Shift(int id, int schedule_id, const std::string& date,
             const std::string& start_time, const std::string& end_time,
             int required_employees)
    : id(id), schedule_id(schedule_id), date(date),
      start_time(start_time), end_time(end_time),
      required_employees(required_employees), status("unfilled") {
}

int Shift::getId() const {
    return id;
}

int Shift::getScheduleId() const {
    return schedule_id;
}

std::string Shift::getDate() const {
    return date;
}

std::string Shift::getStartTime() const {
    return start_time;
}

std::string Shift::getEndTime() const {
    return end_time;
}

int Shift::getRequiredEmployees() const {
    return required_employees;
}

const std::vector<int>& Shift::getAssignedEmployeeIds() const {
    return assigned_employee_ids;
}

std::string Shift::getStatus() const {
    return status;
}

void Shift::assignEmployee(int employee_id) {
    // Check if employee is already assigned
    if (!isEmployeeAssigned(employee_id)) {
        assigned_employee_ids.push_back(employee_id);
        updateStatus();
    }
}

void Shift::removeEmployee(int employee_id) {
    // Find the employee in the vector
    auto it = std::find(assigned_employee_ids.begin(), assigned_employee_ids.end(), employee_id);
    
    // If found, remove them
    if (it != assigned_employee_ids.end()) {
        assigned_employee_ids.erase(it);
    }
    
    // Update the shift status
    updateStatus();
}

bool Shift::isEmployeeAssigned(int employee_id) const {
    return std::find(assigned_employee_ids.begin(), assigned_employee_ids.end(), employee_id) 
           != assigned_employee_ids.end();
}

bool Shift::isFilled() const {
    // Cast required_employees to the vector's size type for safe comparison
    return assigned_employee_ids.size() >= static_cast<std::vector<int>::size_type>(required_employees);
}

void Shift::updateStatus() {
    status = isFilled() ? "filled" : "unfilled";
}
