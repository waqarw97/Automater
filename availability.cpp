#include "availability.h"
#include <stdexcept>

Availability::Availability(int id, int employee_id, int day_of_week,
                           const std::string& start_time, const std::string& end_time)
    : id(id), employee_id(employee_id), day_of_week(day_of_week),
      start_time(start_time), end_time(end_time) {
    
    // Basic validation
    if (day_of_week < 0 || day_of_week > 6) {
        throw std::invalid_argument("Day of week must be between 0 and 6");
    }
    
    // We could add more validation for time formats here
}

int Availability::getId() const {
    return id;
}

int Availability::getEmployeeId() const {
    return employee_id;
}

int Availability::getDayOfWeek() const {
    return day_of_week;
}

std::string Availability::getStartTime() const {
    return start_time;
}

std::string Availability::getEndTime() const {
    return end_time;
}

void Availability::setDayOfWeek(int day_of_week) {
    if (day_of_week < 0 || day_of_week > 6) {
        throw std::invalid_argument("Day of week must be between 0 and 6");
    }
    this->day_of_week = day_of_week;
}

void Availability::setStartTime(const std::string& start_time) {
    // Could add validation here
    this->start_time = start_time;
}

void Availability::setEndTime(const std::string& end_time) {
    // Could add validation here
    this->end_time = end_time;
}

bool Availability::coversTimeRange(int day, const std::string& start, const std::string& end) const {
    // For now, just check if the day matches and do a simple string comparison
    // A more robust implementation would parse and compare actual times
    if (day != day_of_week) {
        return false;
    }
    
    return (this->start_time <= start && this->end_time >= end);
}
