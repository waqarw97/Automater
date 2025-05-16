#include "schedule.h"
#include <ctime>

Schedule::Schedule(int id, const std::string& name, 
                  const std::string& start_date, const std::string& end_date)
    : id(id), name(name), start_date(start_date), end_date(end_date),
      status("draft") {
    
    // Set created_at and last_modified to current time
    time_t now = time(0);
    char time_str[30];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&now));
    
    created_at = time_str;
    last_modified = time_str;
}

int Schedule::getId() const {
    return id;
}

std::string Schedule::getName() const {
    return name;
}

std::string Schedule::getStartDate() const {
    return start_date;
}

std::string Schedule::getEndDate() const {
    return end_date;
}

std::string Schedule::getStatus() const {
    return status;
}

std::string Schedule::getCreatedAt() const {
    return created_at;
}

std::string Schedule::getLastModified() const {
    return last_modified;
}

const std::vector<Shift>& Schedule::getShifts() const {
    return shifts;
}

void Schedule::setName(const std::string& name) {
    this->name = name;
    updateLastModified();
}

void Schedule::setStatus(const std::string& status) {
    this->status = status;
    updateLastModified();
}

void Schedule::updateLastModified() {
    time_t now = time(0);
    char time_str[30];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&now));
    
    last_modified = time_str;
}

void Schedule::addShift(const Shift& shift) {
    shifts.push_back(shift);
    updateLastModified();
}

void Schedule::removeShift(int shift_id) {
    for (auto it = shifts.begin(); it != shifts.end(); ++it) {
        if (it->getId() == shift_id) {
            shifts.erase(it);
            updateLastModified();
            return;
        }
    }
}

Shift* Schedule::getShift(int shift_id) {
    for (auto& shift : shifts) {
        if (shift.getId() == shift_id) {
            return &shift;
        }
    }
    return nullptr;
}

bool Schedule::isValid() const {
    // For now, just a simple check that end date is after start date
    return end_date >= start_date;
}

std::vector<std::string> Schedule::getValidationErrors() const {
    std::vector<std::string> errors;
    
    if (end_date < start_date) {
        errors.push_back("End date must be after start date");
    }
    
    return errors;
}
