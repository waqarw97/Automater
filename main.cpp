#include <iostream>
#include <vector>
#include <string> // Include string header

// Include our class headers
#include "employee.h"
#include "availability.h"
#include "shift.h"
#include "schedule.h"
#include "TimeUtility.h"

int main() {
    // Step 1: Create sample employees
    std::cout << "Creating employees...\n";
    std::vector<Employee> employees;
    employees.push_back(Employee(1, "John Doe", 20, 40));
    employees.push_back(Employee(2, "Jane Smith", 15, 30));
    employees.push_back(Employee(3, "Bob Johnson", 10, 25));

    std::cout << "Created " << employees.size() << " employees.\n\n"; // Added newline for spacing

    // Step 2: Create employee availabilities
    std::cout << "Creating availabilities...\n";
    std::vector<Availability> availabilities;
    // John (ID 1) is available Monday 9AM-5PM
    availabilities.push_back(Availability(10, 1, 1, "09:00", "17:00")); // Availability ID 10, Employee ID 1, Day 1 (Mon)
    // Jane (ID 2) is available Monday 12PM-8PM
    availabilities.push_back(Availability(11, 2, 1, "12:00", "20:00")); // Availability ID 11, Employee ID 2, Day 1 (Mon)
    // Bob (ID 3) is available Monday 8AM-4PM
    availabilities.push_back(Availability(12, 3, 1, "08:00", "16:00")); // Availability ID 12, Employee ID 3, Day 1 (Mon)
    // John (ID 1) is also available Tuesday 10AM-2PM
    availabilities.push_back(Availability(13, 1, 2, "10:00", "14:00")); // Availability ID 13, Employee ID 1, Day 2 (Tue)

    std::cout << "Created " << availabilities.size() << " availability slots.\n\n"; // Added newline

    // Step 3: Create a schedule and shifts
    std::cout << "Creating schedule and shifts...\n";
    // Create a schedule for the week of April 1st, 2024 (example date)
    Schedule main_schedule(1, "Week of 2024-04-01", "2024-04-01", "2024-04-07"); // Schedule ID 1

    // Create some shifts for Monday, April 1st (example date)
    // Assume Schedule ID is 1 for all these shifts
    Shift shift1(101, 1, "2024-04-01", "09:00", "17:00", 1); // Shift ID 101, Schedule 1, 9am-5pm, needs 1 person
    Shift shift2(102, 1, "2024-04-01", "12:00", "20:00", 1); // Shift ID 102, Schedule 1, 12pm-8pm, needs 1 person
    Shift shift3(103, 1, "2024-04-01", "10:00", "14:00", 1); // Shift ID 103, Schedule 1, 10am-2pm, needs 1 person

    // Add shifts to the schedule
    main_schedule.addShift(shift1);
    main_schedule.addShift(shift2);
    main_schedule.addShift(shift3);

    std::cout << "Created schedule '" << main_schedule.getName() << "' with "
              << main_schedule.getShifts().size() << " shifts.\n";

    // Step 4: Test using our TimeUtility functions
    std::cout << "Running tests with TimeUtility...\n";

    // Get the first shift from the schedule
    const auto& shifts = main_schedule.getShifts();
    if (!shifts.empty()) {
        // Test each employee with each shift
        for (const auto& shift : shifts) {
            std::cout << "Shift ID " << shift.getId() << " (" 
                      << shift.getDate() << " " << shift.getStartTime() 
                      << "-" << shift.getEndTime() << "):\n";
                      
            for (const auto& emp : employees) {
                bool can_work = TimeUtility::canEmployeeWorkShift(emp.getId(), shift, availabilities);
                
                std::cout << "  " << emp.getName() << " (ID: " << emp.getId()
                          << ") can work this shift? " 
                          << (can_work ? "Yes" : "No") << "\n";
            }
            std::cout << "\n";
        }
    } else {
        std::cout << "No shifts in schedule to test.\n";
    }

    std::cout << "TimeUtility tests complete.\n\n";

    return 0;
}
