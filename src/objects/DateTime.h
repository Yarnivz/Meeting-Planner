//
// Created by Lucas on 4/2/2026.
//

#ifndef MEETING_PLANNER_DATETIME_H
#define MEETING_PLANNER_DATETIME_H
#include "Date.h"

#include <chrono>

class DateTime : public Date
{
public:
    /**
     * @brief Creates the DateTime class.
     * The DateTime class has a year, month, day and hour attribute.
     * When the DateTime class constructor gets called without parameters, the DateTime that gets created will be the current DateTime.
     *
     * @post ENSURE(hour < 24, "DateTime creation failed. Hour needs to be >= 0 and < 24")
     * @post ENSURE(getYear() == year, "DateTime creation failed. Year was not set correctly.")
     * @post ENSURE(getMonth() == month, "DateTime creation failed. Month was not set correctly.")
     * @post ENSURE(getDay() == day, "DateTime creation failed. Day was not set correctly.")
     * @post ENSURE(getHour() == hour, "DateTime creation failed. Hour was not set correctly.")
     * @post ENSURE(isProperlyInitialized(), "DateTime creation failed. Constructor did not properly initialize.")
     */
    DateTime();

    /**
     * @brief Copies a DateTime object.
     * It copies all important values but makes sure the 'properlyInitialized' test still passes.
     *
     * @pre REQUIRE(d.isProperlyInitialized(), "Tried to copy a DateTime which was not properly initialized by the constructor.")
     *
     * @param d The DateTime to be copied.
     *
     *
     * @post ENSURE(getYear() == d.year, "DateTime creation failed. Year was not set correctly.")
     * @post ENSURE(getMonth() == d.month, "DateTime creation failed. Month was not set correctly.")
     * @post ENSURE(getDay() == d.day, "DateTime creation failed. Day was not set correctly.")
     * @post ENSURE(getHour() == d.hour, "DateTime creation failed. Hour was not set correctly.")
     * @post ENSURE(isProperlyInitialized(), "DateTime creation failed. Constructor did not properly initialize.")
     */
    DateTime(const DateTime& d);

    /**
     *
     * @pre REQUIRE(d.isProperlyInitialized(), "Tried to copy a DateTime which was not properly initialized by the constructor.")
     *
     *
     * @post ENSURE(getYear() == d.year, "DateTime creation failed. Year was not set correctly.")
     * @post ENSURE(getMonth() == d.month, "DateTime creation failed. Month was not set correctly.")
     * @post ENSURE(getDay() == d.day, "DateTime creation failed. Day was not set correctly.")
     * @post ENSURE(getHour() == d.hour, "DateTime creation failed. Hour was not set correctly.")
     * @post ENSURE(isProperlyInitialized(), "DateTime creation failed. Constructor did not properly initialize.")
     */
    DateTime& operator=(const DateTime& d);

    /**
     * @brief Creates the DateTime class.
     * The DateTime class has a year, month, day and hour attribute.
     * When the DateTime class constructor gets called with parameters, all parameters are required and the DateTime will be initialized with the given data.
     * 
     * @pre REQUIRE(hour >= 0, "Hour can't be negative! %i", hour)
     * @pre REQUIRE(hour < 24, "Hour must be less than 24! %i", hour)
     * 
     * @param year
     * @param month
     * @param day
     * @param hour
     * 
     * @post ENSURE(getYear() == year, "Year must be set correctly in order to create DateTime.")
     * @post ENSURE(getMonth() == umonth, "Month must be set correctly in order to create DateTime.")
     * @post ENSURE(getDay() == uday, "Day must be set correctly in order to create DateTime.")
     * @post ENSURE(getHour() == uhour, "Hour must be set correctly in order to create DateTime.")
     * @post ENSURE(isProperlyInitialized(), "Constructor must be properly initialized in order to create DateTime.")
     */
    DateTime(int year, int month, int day, int hour = 0);

    /**
     * @brief Creates the DateTime class.
     * The DateTime class has a year, month, day and hour attribute.
     * When the DateTime class constructor gets called with parameters, all parameters are required and the DateTime will be initialized with the given data.
     * 
     * @pre REQUIRE(date.isProperlyInitialized(), "The given date must be properly initialized")
     * @pre REQUIRE(hour >= 0, "Hour can't be negative! %i", hour)
     * @pre REQUIRE(hour < 24, "Hour must be less than 24! %i", hour)
     * 
     * @param date
     * @param hour
     * 
     * @post ENSURE(getYear() == date.getYear(), "Year must be set correctly in order to create DateTime.")
     * @post ENSURE(getMonth() == date.getMonth(), "Month must be set correctly in order to create DateTime.")
     * @post ENSURE(getDay() == date.getDay(), "Day must be set correctly in order to create DateTime.")
     * @post ENSURE(getHour() == uhour, "Hour must be set correctly in order to create DateTime.")
     * @post ENSURE(isProperlyInitialized(), "Constructor must be properly initialized in order to create DateTime.")
     */
    DateTime(const Date& date, int hour = 0);

    /**
     * @brief Hour getter
     *
     * @post ENSURE(isProperlyInitialized(), "DateTime must be properly initialized with constructor in order to get Hour.")
     * @return the DateTime's hour
     */
    unsigned getHour() const;

    /**
     * @brief Converts the DateTime class to a readable string format.
     *
     * @post ENSURE(isProperlyInitialized(), "DateTime must be properly initialized with constructor in order to convert DateTime into string.")
     * @return the DateTime represented as a string.
     * The DateTime's string format is "YYYY-MM-DD, Hh00"
     */
    std::string toString() const;

    /**
     * @brief Standard stream operator using the toString function to append the DateTime to the stream.
     * @param os stream to write on
     * @param DateTime to write on the stream
     * @return the stream
     */
    friend std::ostream& operator<<(std::ostream& os, const DateTime& DateTime);

    /**
     * @brief Compares 2 DateTimes with each other.
     * Returns "True" if they are the same.
     * Returns "False" if they are NOT the same
     * @param other DateTime to compare with
     * @return whether the DateTimes are the same
     */
    bool operator==(const DateTime& other) const;

    /**
     * @brief Compares 2 DateTimes with each other.
     * Returns "True" if the DateTime before the operator is more in the past than the DateTime after the operator.
     * Returns "False" if the DateTime before the operator is NOT more in the past than the DateTime after the operator.
     * @param other DateTime to compare with
     * @return whether the DateTime is lesser than the other DateTime
     */
    bool operator<(const DateTime& other) const;

    /**
     * @brief Compares 2 DateTimes with each other.
     * Returns "True" if the DateTime before the operator is more recent than the DateTime after the operator.
     * Returns "False" if the DateTime before the operator is NOT more recent than the DateTime after the operator.
     * @param other DateTime to compare with
     * @return whether the DateTime is greater than the other DateTime
     */
    bool operator>(const DateTime& other) const;

    /**
     * @brief Compares 2 DateTimes with each other.
     * Returns "True" if the DateTime before the operator is more in the past or the same as the DateTime after the operator.
     * Returns "False" if the DateTime before the operator is NOT more in the past or the same as the DateTime after the operator.
     * @param other DateTime to compare with
     * @return whether the DateTime is lesser or equal to the other DateTime
     */
    bool operator<=(const DateTime& other) const;

    /**
     * @brief Compares 2 DateTimes with each other.
     * Returns "True" if the DateTime before the operator is more in recent or the same as the DateTime after the operator.
     * Returns "False" if the DateTime before the operator is NOT more recent or the same as the DateTime after the operator.
     * @param other DateTime to compare with
     * @return whether the DateTime is greater or equal to the other DateTime
     */
    bool operator>=(const DateTime& other) const;

    /**
     * @brief Default destructor.
     */
    ~DateTime();

private:
    unsigned hour;
};


#endif //MEETING_PLANNER_DateTime_H
