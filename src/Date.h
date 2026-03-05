//
// Created by User on 2/26/2026.
//

#ifndef MEETING_PLANNER_DATE_H
#define MEETING_PLANNER_DATE_H
#include <iostream>
#include <chrono>

class Date {
public:
    /**
     * Creates the date class.
     * The date class has a year, month and day attribute.
     * When the date class constructor gets called without parameters, the date that gets created will be the current date.
     */
    Date();

    /**
     * Creates the date class.
     * The date class has a year, month and day attribute.
     * When the date class constructor gets called with parameters, all parameters are required and the date will be initialized with the given data.
     * @param year
     * @param month
     * @param day
     */
    Date(int year, int month, int day);

    /**
     *
     * @return the date's year
     */
    int getYear();

    /**
     *
     * @return the date's month
     */
    int getMonth();

    /**
     *
     * @return the date's day
     */
    int getDay();

    /**
     *
     * @return the date represented as a string.
     * The date's string format is "YYYY-MM-DD"
     */
    std::string toString() const;

    /**
     *
     * @param os
     * @param date
     * @return
     */
    friend std::ostream& operator<<(std::ostream &os, Date& date);

    /**
     * Default destructor
     */
    ~Date();
private:
    std::chrono::year year{};
    std::chrono::month month{};
    std::chrono::day day{};
};


#endif //MEETING_PLANNER_DATE_H