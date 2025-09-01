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
    employees.push_back(Employee(1, "Wesley", 35, 50, {1}));      // 70 hrs/2 weeks = 35/week min
    employees.push_back(Employee(2, "Crystal", 40, 50, {1}));    // 80 hrs/2 weeks = 40/week min  
    employees.push_back(Employee(3, "Jay", 40, 50, {1}));        // 80 hrs/2 weeks = 40/week min

    std::cout << "Created " << employees.size() << " employees.\n\n"; // Added newline for spacing

    // Step 2: Create employee availabilities
    std::cout << "Creating availabilities...\n";
    std::vector<Availability> availabilities;
    
    // Old schedules (commented out)
    // John (ID 1) - Morning person, off Fri/Sat, works 80 hrs per 2 weeks
    // Jane (ID 2) - Flexible, off Mon/Tue, works 60 hrs per 2 weeks  
    // Bob (ID 3) - Evening person, off Wed/Thu, works 40 hrs per 2 weeks
    
    // Wesley (ID 1) - Mornings, off Wed/Thu/Sun, needs 70 hrs per 2 weeks
    std::vector<DaySchedule> wesley_schedule = {
        {0, "off", 0},        // Sunday off
        {1, "morning", 0},    // Monday morning
        {2, "morning", +1},   // Tuesday morning, stay late
        {3, "off", 0},        // Wednesday off
        {4, "off", 0},        // Thursday off
        {5, "morning", +2},   // Friday morning, stay extra late
        {6, "morning", +1}    // Saturday morning, stay late
    };
    
    // Crystal (ID 2) - Evenings, off Fri/Sat, needs 80 hrs per 2 weeks
    std::vector<DaySchedule> crystal_schedule = {
        {0, "evening", 0},    // Sunday evening
        {1, "evening", 0},    // Monday evening
        {2, "evening", 0},    // Tuesday evening
        {3, "evening", 0},    // Wednesday evening, standard time
        {4, "evening", 0},    // Thursday evening, standard time
        {5, "off", 0},        // Friday off
        {6, "off", 0}         // Saturday off
    };
    
    // Jay (ID 3) - Flexible, off Mon/Tue, needs 80 hrs per 2 weeks
    std::vector<DaySchedule> jay_schedule = {
        {0, "anytime", 0},    // Sunday - flexible
        {1, "off", 0},        // Monday off
        {2, "off", 0},        // Tuesday off
        {3, "anytime", 0},    // Wednesday - flexible
        {4, "anytime", 0},    // Thursday - flexible
        {5, "anytime", 0},    // Friday - flexible
        {6, "anytime", 0}     // Saturday - flexible
    };

    // Create the employee preferences map
    std::map<int, std::vector<DaySchedule>> employee_preferences;
    employee_preferences[1] = wesley_schedule;   // Wesley's ID is 1
    employee_preferences[2] = crystal_schedule;  // Crystal's ID is 2
    employee_preferences[3] = jay_schedule;      // Jay's ID is 3

    std::cout << "Created " << availabilities.size() << " availability slots.\n\n"; // Added newline

    // Step 3: Create a schedule and shifts
    std::cout << "Creating schedule and shifts...\n";
    // Create a schedule for 2 weeks starting April 1st, 2024
    // Create a schedule for 2 weeks starting September 1st, 2024
    Schedule tobacco_plus(1, "Tobacco Plus", "2025-09-01", "2025-09-14"); // Schedule ID 1
    // The ScheduleGenerator will be responsible for creating shifts dynamically.
    // We just need to pass the schedule and the shifts to the generator.
    ScheduleGenerator generator(employees, employee_preferences);
    generator.generateSchedule(tobacco_plus);

    // Print the schedule table
    printScheduleTable(tobacco_plus, employees);

    return 0;
}
