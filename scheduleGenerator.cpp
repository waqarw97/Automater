#include "scheduleGenerator.h"
#include "employee.h"
#include "shift.h"
#include "schedule.h"
#include "TimeUtility.h"
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <set> // Added for std::set

ScheduleGenerator::ScheduleGenerator(const std::vector<Employee>& employees,
                                     const std::map<int, std::vector<DaySchedule>>& employee_preferences)
    : employees(employees), employee_preferences(employee_preferences) {}

std::pair<std::string, std::string> getStoreHours(int day_of_week) {
    if (day_of_week == 0) return {"09:00", "21:00"}; // Sunday: 9–9
    return {"08:00", "22:00"}; // Mon–Sat: 8–10
}

bool ScheduleGenerator::generateSchedule(Schedule& schedule) {
    // Track weekly hours for each employee
    std::map<int, int> employeeWeeklyHours;
    
    // Helper function to determine week number from date
    auto getWeekNumber = [](const std::string& date) {
        int day;
        sscanf(date.c_str(), "2024-04-%d", &day);
        return (day - 1) / 7; // Week 0: days 1-7, Week 1: days 8-14
    };
    
    int currentWeek = -1;
    
    // Get the date range from the schedule
    std::string start_date = schedule.getStartDate();
    std::string end_date = schedule.getEndDate();
    
    // Iterate through each date in the schedule
    std::string current_date = start_date;
    while (current_date <= end_date) {
        // Get day of week for this date (0=Sunday, 1=Monday, etc.)
        int day_of_week = TimeUtility::getDayOfWeekFromDate(current_date);
        
        // Check if we've moved to a new week and reset hours
        int week_number = getWeekNumber(current_date);
        if (week_number != currentWeek) {
            currentWeek = week_number;
            employeeWeeklyHours.clear(); // Reset all weekly hours
            std::cout << "  *** Starting Week " << (currentWeek + 1) << " - Resetting hours ***" << std::endl;
        }
        
        // TODO: New preference-based logic will go here
        
        // For each employee, check their preference for this day
        std::vector<int> available_employees;
        for (const auto& [employee_id, preferences] : employee_preferences) {
            // Find the DaySchedule for this day_of_week
            for (const auto& day_schedule : preferences) {
                if (day_schedule.day == day_of_week) {
                    std::cout << "  Employee " << employee_id << " prefers '" 
                              << day_schedule.preference << "' on day " << day_of_week << std::endl;
                    
                    // If they're not off, add them to available list
                    if (day_schedule.preference != "off") {
                        available_employees.push_back(employee_id);
                    }
                    break;
                }
            }
        }
        
        std::cout << "  Available employees for day " << day_of_week << ": ";
        for (int emp_id : available_employees) {
            std::cout << "Employee " << emp_id << " ";
        }
        std::cout << std::endl;
        
        // Sort by who is furthest below their minimum hours
        std::sort(available_employees.begin(), available_employees.end(), 
            [&](int emp_a, int emp_b) {
                // Find employee A
                const Employee* emp_a_obj = nullptr;
                for (const auto& employee : employees) {
                    if (employee.getId() == emp_a) {
                        emp_a_obj = &employee;
                        break;
                    }
                }
                
                // Find employee B  
                const Employee* emp_b_obj = nullptr;
                for (const auto& employee : employees) {
                    if (employee.getId() == emp_b) {
                        emp_b_obj = &employee;
                        break;
                    }
                }
                
                // Calculate deficits and compare
                int emp_a_deficit = emp_a_obj->getMinHours() - employeeWeeklyHours[emp_a];
                int emp_b_deficit = emp_b_obj->getMinHours() - employeeWeeklyHours[emp_b];
                
                return emp_a_deficit > emp_b_deficit;
            });
        
        // Only create shifts if we have available employees
        if (!available_employees.empty()) {
            auto store_hours = getStoreHours(day_of_week);
            std::cout << "  Store hours: " << store_hours.first << "-" << store_hours.second << std::endl;
            
            // Track coverage for this day
            bool has_morning_coverage = false;
            bool has_evening_coverage = false;
            int employees_assigned_today = 0;
            static int shift_id_counter = 100;
            
            // Process morning preferences first
            for (int employee_id : available_employees) {
                if (employees_assigned_today >= 2) break; // Daily limit reached
                
                // Find this employee's DaySchedule for today
                for (const auto& [emp_id, preferences] : employee_preferences) {
                    if (emp_id == employee_id) {
                        for (const auto& day_schedule : preferences) {
                            if (day_schedule.day == day_of_week) {
                                if (day_schedule.preference == "morning" && !has_morning_coverage) {
                                    std::cout << "    Employee " << employee_id 
                                              << ": preference='morning', modifier=" << day_schedule.shift_modifier << std::endl;
                                    
                                    // Calculate shift times
                                    std::string base_start = store_hours.first;  // 08:00
                                    std::string base_end = "15:00";              // 3pm handoff
                                    
                                    // Apply shift modifier (modifier adjusts end time)
                                    int end_hour = 15 + day_schedule.shift_modifier;
                                    char modified_end[6];
                                    snprintf(modified_end, sizeof(modified_end), "%02d:00", end_hour);
                                    
                                    // Constrain within store hours
                                    std::string shift_start = std::max(base_start, store_hours.first);
                                    std::string shift_end = std::min(std::string(modified_end), store_hours.second);
                                    
                                    // Create and assign shift
                                    Shift new_shift(shift_id_counter++, schedule.getId(), current_date, 
                                                  shift_start, shift_end, 1);
                                    schedule.addShift(new_shift);
                                    
                                    // Check max hours before assigning
                                    int shift_hours = calculateShiftHours(new_shift);
                                    
                                    // Find employee to get max hours
                                    const Employee* emp = nullptr;
                                    for (const auto& employee : employees) {
                                        if (employee.getId() == employee_id) {
                                            emp = &employee;
                                            break;
                                        }
                                    }
                                    
                                    if (emp && (employeeWeeklyHours[employee_id] + shift_hours <= emp->getMaxHours())) {
                                        // Find the shift we just added and assign employee
                                        for (auto& shift : schedule.getShifts()) {
                                            if (shift.getId() == new_shift.getId()) {
                                                shift.assignEmployee(employee_id);
                                                break;
                                            }
                                        }
                                        
                                        employeeWeeklyHours[employee_id] += shift_hours;
                                        std::cout << "      Created and assigned morning shift: " << shift_start << "-" << shift_end << std::endl;
                                        has_morning_coverage = true;
                                        employees_assigned_today++;
                                    } else {
                                        std::cout << "      Employee " << employee_id << " would exceed max hours - skipping" << std::endl;
                                    }
                                }
                                break;
                            }
                        }
                        break;
                    }
                }
            }
            // Process evening preferences second
            for (int employee_id : available_employees) {
                if (employees_assigned_today >= 2) break; // Daily limit reached
                
                // Find this employee's DaySchedule for today
                for (const auto& [emp_id, preferences] : employee_preferences) {
                    if (emp_id == employee_id) {
                        for (const auto& day_schedule : preferences) {
                            if (day_schedule.day == day_of_week) {
                                if (day_schedule.preference == "evening" && !has_evening_coverage) {
                                    std::cout << "    Employee " << employee_id 
                                              << ": preference='evening', modifier=" << day_schedule.shift_modifier << std::endl;
                                    
                                    // Calculate shift times
                                    std::string base_start = "15:00";              // 3pm handoff
                                    std::string base_end = store_hours.second;     // store close
                                    
                                    // Apply shift modifier (modifier adjusts start time)
                                    int start_hour = 15 + day_schedule.shift_modifier;
                                    char modified_start[6];
                                    snprintf(modified_start, sizeof(modified_start), "%02d:00", start_hour);
                                    
                                    // Constrain within store hours
                                    std::string shift_start = std::max(std::string(modified_start), store_hours.first);
                                    std::string shift_end = std::min(base_end, store_hours.second);
                                    
                                    // Create and assign shift
                                    Shift new_shift(shift_id_counter++, schedule.getId(), current_date, 
                                                  shift_start, shift_end, 1);
                                    schedule.addShift(new_shift);
                                    
                                    // Check max hours before assigning
                                    int shift_hours = calculateShiftHours(new_shift);
                                    
                                    // Find employee to get max hours
                                    const Employee* emp = nullptr;
                                    for (const auto& employee : employees) {
                                        if (employee.getId() == employee_id) {
                                            emp = &employee;
                                            break;
                                        }
                                    }
                                    
                                    if (emp && (employeeWeeklyHours[employee_id] + shift_hours <= emp->getMaxHours())) {
                                        // Find the shift we just added and assign employee
                                        for (auto& shift : schedule.getShifts()) {
                                            if (shift.getId() == new_shift.getId()) {
                                                shift.assignEmployee(employee_id);
                                                break;
                                            }
                                        }
                                        
                                        employeeWeeklyHours[employee_id] += shift_hours;
                                        std::cout << "      Created and assigned evening shift: " << shift_start << "-" << shift_end << std::endl;
                                        has_evening_coverage = true;
                                        employees_assigned_today++;
                                    } else {
                                        std::cout << "      Employee " << employee_id << " would exceed max hours - skipping" << std::endl;
                                    }
                                }
                                break;
                            }
                        }
                        break;
                    }
                }
            }
            // Process anytime preferences last
            for (int employee_id : available_employees) {
                if (employees_assigned_today >= 2) break; // Daily limit reached
                
                // Find this employee's DaySchedule for today
                for (const auto& [emp_id, preferences] : employee_preferences) {
                    if (emp_id == employee_id) {
                        for (const auto& day_schedule : preferences) {
                            if (day_schedule.day == day_of_week) {
                                if (day_schedule.preference == "anytime") {
                                    std::cout << "    Employee " << employee_id 
                                              << ": preference='anytime', modifier=" << day_schedule.shift_modifier << std::endl;
                                    
                                    // Check what coverage we need and assign accordingly
                                    if (!has_morning_coverage) {
                                        // Calculate morning shift times
                                        std::string base_start = store_hours.first;
                                        std::string base_end = "15:00";
                                        
                                        int end_hour = 15 + day_schedule.shift_modifier;
                                        char modified_end[6];
                                        snprintf(modified_end, sizeof(modified_end), "%02d:00", end_hour);
                                        
                                        std::string shift_start = std::max(base_start, store_hours.first);
                                        std::string shift_end = std::min(std::string(modified_end), store_hours.second);
                                        
                                        // Create and assign shift
                                        Shift new_shift(shift_id_counter++, schedule.getId(), current_date, 
                                                      shift_start, shift_end, 1);
                                        schedule.addShift(new_shift);
                                        
                                        // Check max hours before assigning
                                        int shift_hours = calculateShiftHours(new_shift);
                                        
                                        // Find employee to get max hours
                                        const Employee* emp = nullptr;
                                        for (const auto& employee : employees) {
                                            if (employee.getId() == employee_id) {
                                                emp = &employee;
                                                break;
                                            }
                                        }
                                        
                                        if (emp && (employeeWeeklyHours[employee_id] + shift_hours <= emp->getMaxHours())) {
                                            // Find the shift we just added and assign employee
                                            for (auto& shift : schedule.getShifts()) {
                                                if (shift.getId() == new_shift.getId()) {
                                                    shift.assignEmployee(employee_id);
                                                    break;
                                                }
                                            }
                                            
                                            employeeWeeklyHours[employee_id] += shift_hours;
                                            std::cout << "      Created and assigned anytime->morning shift: " << shift_start << "-" << shift_end << std::endl;
                                            has_morning_coverage = true;
                                            employees_assigned_today++;
                                        } else {
                                            std::cout << "      Employee " << employee_id << " would exceed max hours - skipping" << std::endl;
                                        }
                                    } else if (!has_evening_coverage) {
                                        // Calculate evening shift times
                                        std::string base_start = "15:00";
                                        std::string base_end = store_hours.second;
                                        
                                        int start_hour = 15 + day_schedule.shift_modifier;
                                        char modified_start[6];
                                        snprintf(modified_start, sizeof(modified_start), "%02d:00", start_hour);
                                        
                                        std::string shift_start = std::max(std::string(modified_start), store_hours.first);
                                        std::string shift_end = std::min(base_end, store_hours.second);
                                        
                                        // Create and assign shift
                                        Shift new_shift(shift_id_counter++, schedule.getId(), current_date, 
                                                      shift_start, shift_end, 1);
                                        schedule.addShift(new_shift);
                                        
                                        // Check max hours before assigning
                                        int shift_hours = calculateShiftHours(new_shift);
                                        
                                        // Find employee to get max hours
                                        const Employee* emp = nullptr;
                                        for (const auto& employee : employees) {
                                            if (employee.getId() == employee_id) {
                                                emp = &employee;
                                                break;
                                            }
                                        }
                                        
                                        if (emp && (employeeWeeklyHours[employee_id] + shift_hours <= emp->getMaxHours())) {
                                            // Find the shift we just added and assign employee
                                            for (auto& shift : schedule.getShifts()) {
                                                if (shift.getId() == new_shift.getId()) {
                                                    shift.assignEmployee(employee_id);
                                                    break;
                                                }
                                            }
                                            
                                            employeeWeeklyHours[employee_id] += shift_hours;
                                            std::cout << "      Created and assigned anytime->evening shift: " << shift_start << "-" << shift_end << std::endl;
                                            has_evening_coverage = true;
                                            employees_assigned_today++;
                                        } else {
                                            std::cout << "      Employee " << employee_id << " would exceed max hours - skipping" << std::endl;
                                        }
                                    } else {
                                        std::cout << "      Both shifts covered - skipping anytime employee" << std::endl;
                                    }
                                }
                                break;
                            }
                        }
                        break;
                    }
                }
            }
        }
        
        current_date = TimeUtility::incrementDate(current_date);
    }

    return true;
}

int ScheduleGenerator::calculateShiftHours(const Shift& shift) {
    int start = TimeUtility::timeStringToMinutes(shift.getStartTime());
    int end = TimeUtility::timeStringToMinutes(shift.getEndTime());
    return (end - start) / 60; // returns hours as int
}

bool ScheduleGenerator::doesEmployeeHaveOverlappingShift(
    int employee_id,
    const Shift& candidate_shift,
    const Schedule& schedule
) {
    for (const Shift& shift : schedule.getShifts()) {
        //continue here because we are not checking to see if a shift conflicts with itself
        if (shift.getId() == candidate_shift.getId()) continue;
        //skip shifts where employee is not assigned
        if (!shift.isEmployeeAssigned(employee_id)) continue;
        //skip shifts on different dates
        if (shift.getDate() != candidate_shift.getDate()) continue;

        // If we get here, employee already has a shift on this date
        // Prevent any same-day assignments (not just time overlaps)
        return true;
    }
    return false;
}
