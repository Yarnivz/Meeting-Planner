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
     * @brief Creates the date class.
     * The date class has a year, month and day attribute.
     * When the date class constructor gets called without parameters, the date that gets created will be the current date.
     */
    Date();
    Date(const Date& d);



    /**
     * @brief Creates the date class.
     * The date class has a year, month and day attribute.
     * When the date class constructor gets called with parameters, all parameters are required and the date will be initialized with the given data.
     * @param year
     * @param month
     * @param day
     */
    Date(int year, int month, int day);

    /**
     * @brief Creates the date class.
     * The date class has a year, month and day attribute.
     * When the date class constructor gets called with parameters, all parameters are required and the date will be initialized with the given data.
     * @param year_month_day date to initialize the Date class with.
     */
    Date(std::chrono::year_month_day year_month_day);


    bool isProperlyInitialized() const;


    /**
     * @brief Year getter.
     * @return the date's year
     */
    int getYear();

    /**
     * @brief Month getter.
     * @return the date's month
     */
    int getMonth();

    /**
     * @brief Day getter.
     * @return the date's day
     */
    int getDay();


    std::string getWeekDay();

    /**
     * @brief Converts the date class to a readable string format.
     * @return the date represented as a string.
     * The date's string format is "YYYY-MM-DD"
     */
    std::string toString() const;

    /**
     * @brief Standard stream operator using the toString function to append the date to the stream.
     * @param os stream to write on
     * @param date to write on the stream
     * @return the stream
     */
    friend std::ostream& operator<<(std::ostream &os, Date& date);

    /**
     * @brief Compares 2 dates with each other.
     * Returns "True" if they are the same.
     * Returns "False" if they are NOT the same
     * @param other date to compare with
     * @return whether the dates are the same
     */
    bool operator==(const Date &other) const;

    /**
     * @brief Compares 2 dates with each other.
     * Returns "True" if the date before the operator is more in the past than the date after the operator.
     * Returns "False" if the date before the operator is NOT more in the past than the date after the operator.
     * @param other date to compare with
     * @return whether the date is lesser than the other date
     */
    bool operator<(const Date &other) const;

    /**
     * @brief Compares 2 dates with each other.
     * Returns "True" if the date before the operator is more recent than the date after the operator.
     * Returns "False" if the date before the operator is NOT more recent than the date after the operator.
     * @param other date to compare with
     * @return whether the date is greater than the other date
     */
    bool operator>(const Date &other) const;

    /**
     * @brief Compares 2 dates with each other.
     * Returns "True" if the date before the operator is more in the past or the same as the date after the operator.
     * Returns "False" if the date before the operator is NOT more in the past or the same as the date after the operator.
     * @param other date to compare with
     * @return whether the date is lesser or equal to the other date
     */
    bool operator<=(const Date &other) const;

    /**
     * @brief Compares 2 dates with each other.
     * Returns "True" if the date before the operator is more in recent or the same as the date after the operator.
     * Returns "False" if the date before the operator is NOT more recent or the same as the date after the operator.
     * @param other date to compare with
     * @return whether the date is greater or equal to the other date
     */
    bool operator>=(const Date &other) const;

    /**
     * @brief Default destructor.
     */
    ~Date();
private:
    std::chrono::year year{};
    std::chrono::month month{};
    std::chrono::day day{};

    void* init_test_this_ptr;
};


#endif //MEETING_PLANNER_DATE_H