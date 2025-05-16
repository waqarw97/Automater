#ifndef SHIFT_H
#define SHIFT_H

#include <string>
#include <vector>

class Shift {
private:
    int id;
    int schedule_id;
    std::string date;        // Format: "YYYY-MM-DD"
    std::string start_time;  // Format: "HH:MM"
    std::string end_time;    // Format: "HH:MM" 
    int required_employees;
    std::vector<int> assigned_employee_ids;
    std::string status;      // "unfilled" or "filled"

public:
    // Constructor
    Shift(int id, int schedule_id, const std::string& date,
          const std::string& start_time, const std::string& end_time,
          int required_employees);
    
    // Getters
    int getId() const;
    int getScheduleId() const;
    std::string getDate() const;
    std::string getStartTime() const;
    std::string getEndTime() const;
    int getRequiredEmployees() const;
    const std::vector<int>& getAssignedEmployeeIds() const;
    std::string getStatus() const;
    
    // Methods for managing employees
    void assignEmployee(int employee_id);
    void removeEmployee(int employee_id);
    bool isEmployeeAssigned(int employee_id) const;
    bool isFilled() const;
    
    // Update status based on employee assignments
    void updateStatus();
};

#endif // SHIFT_H
