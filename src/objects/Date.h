//
// Created by Yarni on 2/26/2026.
//

#ifndef MEETING_PLANNER_DATE_H
#define MEETING_PLANNER_DATE_H
#include <iostream>
#include <chrono>


class Date
{
public:
    /**
     * @brief Creates the date class.
     * The date class has a year, month and day attribute.
     * When the date class constructor gets called without parameters, the date that gets created will be the current date.
     *
     * @post Date creation failed. Date validity check did not pass.
     * @post Date was not properly initialized.
     * @post Day was not set.
     * @post Month was not set.
     * @post Year was not set.
     */
    Date();

    /**
     * @brief Copies a Date object.
     * It copies all important values but makes sure the 'properlyInitialized' test still passes.
     *
     * @pre Tried to copy a date which was not properly initialized by the constructor.
     *
     * @param d The Date to be copied.
     *
     * @post Date was not properly initialized.
     * @post Day was not set.
     * @post Month was not set.
     * @post Year was not set.
     */
    Date(const Date& d);

    /**
     *
     *@pre Tried to copy a date which was not properly initialized by the constructor.
     *
     *
     *@post Date was not properly initialized.
     *@post Day was not set.
     *@post Month was not set.
     *@post Year was not set.
     */
    Date& operator=(const Date& d);

    /**
     * @brief Creates the date class.
     * The date class has a year, month and day attribute.
     * When the date class constructor gets called with parameters, all parameters are required and the date will be initialized with the given data.
     *
     * @pre Year can not be negative! %i
     * @pre Month can not be negative! %i
     * @pre Day can not be negative! %i
     * @pre Invalid date provided. Please check if this date really exists!
     *
     * @param year
     * @param month
     * @param day
     *
     * @post Date creation failed. Date validity check did not pass.
     * @post Date was not properly initialized.
     * @post Day was not set.
     * @post Month was not set.
     * @post Year was not set.
     */
    Date(int year, int month, int day);

    /**
     * @brief Creates the date class.
     * The date class has a year, month and day attribute.
     * When the date class constructor gets called with parameters, all parameters are required and the date will be initialized with the given data.
     *
     * @pre Invalid date provided. Please check if this date really exists!
     *
     * @param year_month_day date to initialize the Date class with.
     *
     * @post Date creation failed. Date validity check did not pass.
     * @post Date was not properly initialized.
     * @post Day was not set.
     * @post Month was not set.
     * @post Year was not set.
     */
    Date(std::chrono::year_month_day year_month_day);

    /**
     * @brief Checks whether this Participation was properly initialized by the constructor.
     * @return bool indicating result
     */
    bool isProperlyInitialized() const;

    /**
     * @brief Year getter.
     *
     * @post Failed to get Year. Date must be properly initialized with the constructor!
     *
     * @return the date's year
     */
    int getYear() const;

    /**
     * @brief Month getter.
     *
     * @post Failed to get Month. Date must be properly initialized with the constructor!
     *
     * @return the date's month
     */
    unsigned int getMonth() const;

    /**
     * @brief Day getter.
     *
     * @post Failed to get Day. Date must be properly initialized with the constructor!
     *
     * @return the date's day
     */
    unsigned int getDay() const;

    /**
     * @brief Weekday getter.
     * @return the date's weekday
     */
    std::string getWeekDay() const;

    /**
     * @brief Converts the date class to a readable string format.
     * The date's string format is "YYYY-MM-DD"
     *
     * @post Failed to convert date to string. Date must be properly initialized with the constructor!
     *
     * @return the date represented as a string.
     */
    std::string toString() const;

    /**
     * @brief Standard stream operator using the toString function to append the date to the stream.
     * @param os stream to write on
     * @param date to write on the stream
     * @return the stream
     */
    friend std::ostream& operator<<(std::ostream& os, const Date& date);

    /**
     * @brief Compares 2 dates with each other.
     * Returns "True" if they are the same.
     * Returns "False" if they are NOT the same
     * @param other date to compare with
     * @return whether the dates are the same
     */
    bool operator==(const Date& other) const;

    /**
     * @brief Compares 2 dates with each other.
     * Returns "True" if the date before the operator is more in the past than the date after the operator.
     * Returns "False" if the date before the operator is NOT more in the past than the date after the operator.
     * @param other date to compare with
     * @return whether the date is lesser than the other date
     */
    bool operator<(const Date& other) const;

    /**
     * @brief Compares 2 dates with each other.
     * Returns "True" if the date before the operator is more recent than the date after the operator.
     * Returns "False" if the date before the operator is NOT more recent than the date after the operator.
     * @param other date to compare with
     * @return whether the date is greater than the other date
     */
    bool operator>(const Date& other) const;

    /**
     * @brief Compares 2 dates with each other.
     * Returns "True" if the date before the operator is more in the past or the same as the date after the operator.
     * Returns "False" if the date before the operator is NOT more in the past or the same as the date after the operator.
     * @param other date to compare with
     * @return whether the date is lesser or equal to the other date
     */
    bool operator<=(const Date& other) const;

    /**
     * @brief Compares 2 dates with each other.
     * Returns "True" if the date before the operator is more in recent or the same as the date after the operator.
     * Returns "False" if the date before the operator is NOT more recent or the same as the date after the operator.
     * @param other date to compare with
     * @return whether the date is greater or equal to the other date
     */
    bool operator>=(const Date& other) const;

    /**
     * @brief Default destructor.
     */
    ~Date();

protected:
    int year;
    unsigned month;
    unsigned day;
    void* init_test_this_ptr;
};


#endif //MEETING_PLANNER_DATE_H
