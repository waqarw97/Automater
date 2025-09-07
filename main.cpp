#include <iostream>
#include <vector>
#include <string> // Include string header
#include <iomanip> // for std::setw
#include <ctime>
#include <sstream>

// Include our class headers
#include "employee.h"
#include "availability.h"
#include "shift.h"
#include "schedule.h"
#include "TimeUtility.h"
#include "scheduleGenerator.h"

// Helper: increment a date string ("YYYY-MM-DD") by one day
std::string to12Hour(const std::string& time24) {
    int hour, minute;
    char colon;
    std::istringstream iss(time24);
    iss >> hour >> colon >> minute;
    std::string ampm = (hour >= 12) ? "PM" : "AM";
    int hour12 = hour % 12;
    if (hour12 == 0) hour12 = 12;
    char buf[8];
    snprintf(buf, sizeof(buf), "%d:%02d %s", hour12, minute, ampm.c_str());
    return std::string(buf);
}

// Helper function to determine required employees based on schedule type and shift time
int getRequiredEmployees(int schedule_id, const std::string& start_time) {
    if (schedule_id == 0) {
        // Simple store: always 1 person
        return 1;
    }
    else if (schedule_id == 1) {
        // Complex store: 1 for mornings, 2 for evenings
        if (start_time == "08:00") return 1;  // Morning shift
        if (start_time == "15:00") return 2;  // Evening shift
    }
    // Default fallback
    return 1;
}

// Helper function to determine week number from date
auto getWeekNumber = [](const std::string& date) {
    int day;
    sscanf(date.c_str(), "2024-04-%d", &day);
    return (day - 1) / 7; // Week 0: days 1-7, Week 1: days 8-14
};

// Function to print the schedule in a table format
void printScheduleTable(const Schedule& schedule, const std::vector<Employee>& employees) {
    // Print the title using the schedule's name and date range
    std::cout << schedule.getName() << " - "
              << schedule.getStartDate() << " to " << schedule.getEndDate() << std::endl;

    // Print the column headers
    std::cout << std::setw(12) << "Date"
              << std::setw(8) << "Day";
    for (const auto& emp : employees) {
        std::cout << std::setw(18) << emp.getName();
    }
    std::cout << std::endl;

    std::string current_date = schedule.getStartDate();
    std::string end_date = schedule.getEndDate();

    while (current_date <= end_date) {
        std::cout << std::setw(12) << current_date;

        // Print the day of the week
        int day_of_week = TimeUtility::getDayOfWeekFromDate(current_date);
        static const char* day_names[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
        std::cout << std::setw(8) << day_names[day_of_week];

        for (const auto& emp : employees) {
            std::string shift_str = "Off";
            for (const auto& shift : schedule.getShifts()) {
                if (shift.getDate() == current_date && shift.isEmployeeAssigned(emp.getId())) {
                    shift_str = to12Hour(shift.getStartTime()) + "-" + to12Hour(shift.getEndTime());
                    break; // Stop after finding the first shift for this employee on this date
                }
            }
            std::cout << std::setw(18) << shift_str;
        }
        std::cout << std::endl;
        current_date = TimeUtility::incrementDate(current_date);
    }

    // 3. For each day in the 2-week period:
    //    - Print the date and day of week
    //    - For each employee, print their shift (or "Off")
    
    // (Implementation will go here step by step)
}

int main() {
    // Step 1: Create sample employees
    std::cout << "Creating employees...\n";
    std::vector<Employee> employees;
    
    // Old employees (commented out)
    // employees.push_back(Employee(1, "John Doe", 35, 45, {1}));      // 40 hrs/week avg, morning shifts
    // employees.push_back(Employee(2, "Jane Smith", 25, 35, {1}));    // 30 hrs/week avg, flexible
    // employees.push_back(Employee(3, "Bob Johnson", 15, 30, {1}));   // 20 hrs/week avg, evening shifts
    
    // New employees
    employees.push_back(Employee(1, "Sierra", 35, 40, {1}));     // 35 min, 40 max hours per week
    employees.push_back(Employee(2, "Susi", 40, 40, {1}));       // 40 min, 40 max hours per week  
    employees.push_back(Employee(3, "Tanisha", 35, 40, {1}));    // 35 min, 40 max hours per week
    employees.push_back(Employee(4, "Samantha", 0, 40, {1}));    // Only works Sundays

    std::cout << "Created " << employees.size() << " employees.\n\n"; // Added newline for spacing

    // Step 2: Create employee availabilities
    std::cout << "Creating availabilities...\n";
    std::vector<Availability> availabilities;
    
    // Old schedules (commented out)
    // John (ID 1) - Morning person, off Fri/Sat, works 80 hrs per 2 weeks
    // Jane (ID 2) - Flexible, off Mon/Tue, works 60 hrs per 2 weeks  
    // Bob (ID 3) - Evening person, off Wed/Thu, works 40 hrs per 2 weeks
    
    // Sierra (ID 1) - Anytime preference
    std::vector<DaySchedule> sierra_schedule = {
        {0, "anytime", 0},    // Sunday - anytime
        {1, "anytime", 0},    // Monday - anytime
        {2, "anytime", 0},    // Tuesday - anytime
        {3, "anytime", 0},    // Wednesday - anytime
        {4, "anytime", 0},    // Thursday - anytime
        {5, "anytime", 0},    // Friday - anytime
        {6, "anytime", 0}     // Saturday - anytime
    };
    
    // Susi (ID 2) - Morning shifts only, off Friday and Sunday
    std::vector<DaySchedule> susi_schedule = {
        {0, "off", 0},        // Sunday off
        {1, "morning", 0},    // Monday morning
        {2, "morning", 0},    // Tuesday morning
        {3, "morning", 0},    // Wednesday morning
        {4, "morning", 0},    // Thursday morning
        {5, "off", 0},        // Friday off
        {6, "morning", 0}     // Saturday morning
    };
    
    // Tanisha (ID 3) - Anytime preference, needs Sep 10-12 off (will add as override)
    std::vector<DaySchedule> tanisha_schedule = {
        {0, "anytime", 0},    // Sunday - anytime
        {1, "anytime", 0},    // Monday - anytime
        {2, "anytime", 0},    // Tuesday - anytime
        {3, "anytime", 0},    // Wednesday - anytime
        {4, "anytime", 0},    // Thursday - anytime
        {5, "anytime", 0},    // Friday - anytime
        {6, "anytime", 0}     // Saturday - anytime
    };
    
    // Samantha (ID 4) - Only works Sunday mornings
    std::vector<DaySchedule> samantha_schedule = {
        {0, "morning", 0},    // Sunday morning only
        {1, "off", 0},        // Monday off
        {2, "off", 0},        // Tuesday off
        {3, "off", 0},        // Wednesday off
        {4, "off", 0},        // Thursday off
        {5, "off", 0},        // Friday off
        {6, "off", 0}         // Saturday off
    };

    // Create the employee preferences map
    std::map<int, std::vector<DaySchedule>> employee_preferences;
    employee_preferences[1] = sierra_schedule;
    employee_preferences[2] = susi_schedule;
    employee_preferences[3] = tanisha_schedule;
    employee_preferences[4] = samantha_schedule;

    // Per-date overrides: employee_id -> { "YYYY-MM-DD" -> (preference, modifier) }
    std::map<int, std::map<std::string, std::pair<std::string,int>>> per_date_overrides;
    
    // Sierra overrides: Sep 5-8 off and Sep 13 off
    per_date_overrides[1]["2024-09-05"] = {"off", 0};  // Thursday off
    per_date_overrides[1]["2024-09-06"] = {"off", 0};  // Friday off
    per_date_overrides[1]["2024-09-07"] = {"off", 0};  // Saturday off
    per_date_overrides[1]["2024-09-08"] = {"off", 0};  // Sunday off
    per_date_overrides[1]["2024-09-13"] = {"off", 0};  // Friday off
    
    // Susi overrides: Doesn't start until Sep 5 (Sep 1-4 off)
    per_date_overrides[2]["2024-09-01"] = {"off", 0};  // Sunday off
    per_date_overrides[2]["2024-09-02"] = {"off", 0};  // Monday off
    per_date_overrides[2]["2024-09-03"] = {"off", 0};  // Tuesday off
    per_date_overrides[2]["2024-09-04"] = {"off", 0};  // Wednesday off
    
    // Tanisha overrides: Sep 10-12 off (made this up as requested)
    per_date_overrides[3]["2024-09-10"] = {"off", 0};  // Tuesday off
    per_date_overrides[3]["2024-09-11"] = {"off", 0};  // Wednesday off
    per_date_overrides[3]["2024-09-12"] = {"off", 0};  // Thursday off

    // Create the schedule
    Schedule tobacco_plus(1, "Tobacco Plus Schedule", "2024-09-01", "2024-09-14");

    // The ScheduleGenerator will be responsible for creating shifts dynamically.
    // We just need to pass the schedule and the shifts to the generator.
    ScheduleGenerator generator(employees, employee_preferences, per_date_overrides);
    generator.generateSchedule(tobacco_plus);

    // Print the schedule table
    printScheduleTable(tobacco_plus, employees);

    return 0;
}
