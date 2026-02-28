//
// Created by User on 2/26/2026.
//

#ifndef MEETING_PLANNER_DATE_H
#define MEETING_PLANNER_DATE_H


class Date {
public:
    Date();
    Date(int year, int month, int day);
    ~Date();
private:
    int year;
    int month;
    int day;
};


#endif //MEETING_PLANNER_DATE_H