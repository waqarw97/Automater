#include "TimeUtility.h"
#include <sstream>
#include <ctime>
#include <iomanip>

int TimeUtility::timeStringToMinutes(const std::string& time_str) {
    int hours = 0, minutes = 0;
    sscanf(time_str.c_str(), "%d:%d", &hours, &minutes);
    
    return hours * 60 + minutes;
}

std::string TimeUtility::minutesToTimeString(int minutes) {
    int hours = minutes / 60;
    int mins = minutes % 60;
    
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << hours << ":"
       << std::setfill('0') << std::setw(2) << mins;
    
    return ss.str();
}

bool TimeUtility::timeRangeCovers(const std::string& cover_start, const std::string& cover_end,
                                const std::string& target_start, const std::string& target_end) {
    // Convert to integers for proper numerical comparison
    int cover_start_mins = timeStringToMinutes(cover_start);
    int cover_end_mins = timeStringToMinutes(cover_end);
    int target_start_mins = timeStringToMinutes(target_start);
    int target_end_mins = timeStringToMinutes(target_end);
    
    // Check if cover range fully contains target range
    return (cover_start_mins <= target_start_mins && cover_end_mins >= target_end_mins);
}

bool TimeUtility::canEmployeeWorkShift(int employee_id, const Shift& shift, 
                                     const std::vector<Availability>& availabilities) {
    // For now, use a simplified approach - assuming all shifts are on Monday (day 1)
    // Later we can implement getDayOfWeekFromDate to get the actual day
    int day_of_week = getDayOfWeekFromDate(shift.getDate());
    
    // Check all availabilities for this employee
    for (const auto& avail : availabilities) {
        if (avail.getEmployeeId() == employee_id && 
            avail.getDayOfWeek() == day_of_week && 
            timeRangeCovers(avail.getStartTime(), avail.getEndTime(), 
                           shift.getStartTime(), shift.getEndTime())) {
            return true;
        }
    }
    return false;
}

int TimeUtility::getDayOfWeekFromDate(const std::string& date_str) {
    // Parse YYYY-MM-DD format
    struct tm tm_date = {};
    int year, month, day;
    sscanf(date_str.c_str(), "%d-%d-%d", &year, &month, &day);
    
    tm_date.tm_year = year - 1900;  // Years since 1900
    tm_date.tm_mon = month - 1;     // Months are 0-11
    tm_date.tm_mday = day;          // Days are 1-31
    
    // Set the time to noon to avoid DST issues
    tm_date.tm_hour = 12;
    
    // Calculate time_t value
    time_t time_value = mktime(&tm_date);
    
    // Convert back to tm struct to get day of week
    struct tm* result_tm = localtime(&time_value);
    
    // Get day of week (0 = Sunday, 1 = Monday, etc.)
    return result_tm->tm_wday;
}



std::string TimeUtility::incrementDate(const std::string& date_str) {
    struct tm tm_date = {};
    int year, month, day;
    sscanf(date_str.c_str(), "%d-%d-%d", &year, &month, &day);
    tm_date.tm_year = year - 1900;
    tm_date.tm_mon = month - 1;
    tm_date.tm_mday = day;
    tm_date.tm_hour = 12; // avoid DST issues
    time_t t = mktime(&tm_date) + 60 * 60 * 24;
    struct tm* next_tm = localtime(&t);
    char buf[11];
    strftime(buf, sizeof(buf), "%Y-%m-%d", next_tm);
    return std::string(buf);
}