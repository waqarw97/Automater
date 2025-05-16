#ifndef AVAILABILITY_H
#define AVAILABILITY_H

#include <string>

class Availability {
private:
    int id;
    int employee_id;
    int day_of_week;  // 0-6 for Monday-Sunday
    std::string start_time;  // Format: "HH:MM"
    std::string end_time;    // Format: "HH:MM"

public:
    // Constructor
    Availability(int id, int employee_id, int day_of_week, 
                 const std::string& start_time, const std::string& end_time);
    
    // Getters
    int getId() const;
    int getEmployeeId() const;
    int getDayOfWeek() const;
    std::string getStartTime() const;
    std::string getEndTime() const;
    
    // Setters
    void setDayOfWeek(int day_of_week);
    void setStartTime(const std::string& start_time);
    void setEndTime(const std::string& end_time);
    
    // For now, a simple check if this availability covers a specific time
    bool coversTimeRange(int day, const std::string& start, const std::string& end) const;
};

#endif // AVAILABILITY_H
