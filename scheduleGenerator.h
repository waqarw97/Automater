#ifndef SCHEDULE_GENERATOR_H
#define SCHEDULE_GENERATOR_H

#include <vector>
#include "employee.h"
#include "availability.h"
#include "shift.h"
#include "schedule.h"
#include "TimeUtility.h"

class ScheduleGenerator {
public:
    // Constructor takes the data needed for scheduling
    ScheduleGenerator(const std::vector<Employee>& employees, 
                      const std::vector<Availability>& availabilities);
    
    // Main method to generate a schedule - fills in shifts with employees
    bool generateSchedule(Schedule& schedule);
    
private:
    // Store references to the data
    const std::vector<Employee>& employees;
    const std::vector<Availability>& availabilities;
    
    // Track assigned hours per employee during scheduling
    std::map<int, int> employee_assigned_hours;
    
    // Helper method to find the best employee for a shift
    int findBestEmployeeForShift(const Shift& shift);
    
    // Initialize tracking data before generation
    void initializeTracking();
    
    // Update employee hours when assigned to a shift
    void updateEmployeeHours(int employee_id, const Shift& shift);
    
    // Calculate shift duration in hours
    int calculateShiftHours(const Shift& shift);
};

#endif // SCHEDULE_GENERATOR_H
