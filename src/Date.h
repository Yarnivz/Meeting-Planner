//
// Created by User on 2/26/2026.
//

#ifndef MEETING_PLANNER_DATE_H
#define MEETING_PLANNER_DATE_H
#include <iostream>
#include <chrono>

class Date {
public:
    Date();
    Date(int year, int month, int day);
    int getYear();
    int getMonth();
    int getDay();
    std::string toString() const;
    friend std::ostream& operator<<(std::ostream &os, Date& date);
    ~Date();
private:
    std::chrono::year year{};
    std::chrono::month month{};
    std::chrono::day day{};
};


#endif //MEETING_PLANNER_DATE_H