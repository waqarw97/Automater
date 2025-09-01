#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <string>
#include <vector>
#include "shift.h"

class Schedule {
private:
    int id;
    std::string name;
    std::string start_date;  // Format: "YYYY-MM-DD"
    std::string end_date;    // Format: "YYYY-MM-DD"
    std::string status;      // "draft", "active", or "archived"
    std::string created_at;  // Format: "YYYY-MM-DD HH:MM:SS"
    std::string last_modified; // Format: "YYYY-MM-DD HH:MM:SS"
    std::vector<Shift> shifts;

public:
    // Constructor
    Schedule(int id, const std::string& name, 
             const std::string& start_date, const std::string& end_date);
    
    // Getters
    int getId() const;
    std::string getName() const;
    std::string getStartDate() const;
    std::string getEndDate() const;
    std::string getStatus() const;
    std::string getCreatedAt() const;
    std::string getLastModified() const;
    const std::vector<Shift>& getShifts() const;
    std::vector<Shift>& getShifts();
    
    // Setters
    void setName(const std::string& name);
    void setStatus(const std::string& status);
    void updateLastModified();
    
    // Shift management
    void addShift(const Shift& shift);
    void removeShift(int shift_id);
    Shift* getShift(int shift_id);
    
    // Validation
    bool isValid() const;
    std::vector<std::string> getValidationErrors() const;
};

#endif // SCHEDULE_H
