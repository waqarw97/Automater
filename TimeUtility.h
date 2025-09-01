#ifndef TIME_UTILITY_H
#define TIME_UTILITY_H

#include <string>
#include <vector>
#include "employee.h"
#include "availability.h"
#include "shift.h"

class TimeUtility {
public:
    // Check if an employee can work a specific shift based on their availabilities
    static bool canEmployeeWorkShift(int employee_id, const Shift& shift, 
                             const std::vector<Availability>& availabilities);
    
    // Get the day of week (0-6) from a date string (YYYY-MM-DD)
    static int getDayOfWeekFromDate(const std::string& date_str);
    
    // Compare two time strings (HH:MM format)
    // Returns -1 if time1 < time2, 0 if time1 == time2, 1 if time1 > time2
    static int compareTimeStrings(const std::string& time1, const std::string& time2);
    
    // Check if a time range covers another time range
    static bool timeRangeCovers(const std::string& cover_start, const std::string& cover_end,
                        const std::string& target_start, const std::string& target_end);
    
    // Convert minutes since midnight to HH:MM format
    static std::string minutesToTimeString(int minutes);
    
    // Convert HH:MM format to minutes since midnight
    static int timeStringToMinutes(const std::string& time_str);

    // Increment a date string by one day
    static std::string incrementDate(const std::string& date_str);
};

#endif // TIME_UTILITY_H
