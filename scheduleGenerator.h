#ifndef SCHEDULE_GENERATOR_H
#define SCHEDULE_GENERATOR_H

#include <vector>
#include <map>
#include "employee.h"
#include "shift.h"
#include "schedule.h"
#include "TimeUtility.h"

struct DaySchedule {
    int day;                    // 0-6
    std::string preference;     // "anytime", "morning", "evening", "off"
    int shift_modifier;         // +/- hours from standard 3pm handoff
};

class ScheduleGenerator {
public:
    ScheduleGenerator(const std::vector<Employee>& employees, 
                      const std::map<int, std::vector<DaySchedule>>& employee_preferences);
    
    bool generateSchedule(Schedule& schedule);
    
private:
    const std::vector<Employee>& employees;
    const std::map<int, std::vector<DaySchedule>>& employee_preferences;
    
    int calculateShiftHours(const Shift& shift);
    bool doesEmployeeHaveOverlappingShift(int employee_id, const Shift& candidate_shift, const Schedule& schedule);
};

#endif
