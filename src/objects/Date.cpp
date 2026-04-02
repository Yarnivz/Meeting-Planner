//
// Created by Yarni on 2/26/2026.
//

#include "Date.h"
#include <string>
#include <chrono>
#include <stdexcept>

#include "helper/DesignByContract.h"


Date::Date()
{
    const std::chrono::time_point now = std::chrono::system_clock::now();
    const std::chrono::year_month_day current_date = std::chrono::floor<std::chrono::days>(now);
    year = int(current_date.year());
    month = unsigned(current_date.month());
    day = unsigned(current_date.day());

    init_test_this_ptr = this;

    ENSURE(current_date.ok(), "Date creation failed. Date validity check did not pass.");
}

Date::Date(const Date& d)
{
    REQUIRE(d.isProperlyInitialized(), "Tried to copy a date which was not properly initialized by the constructor.");

    year = d.year;
    month = d.month;
    day = d.day;

    init_test_this_ptr = this; //Reset this pointer, do not copy it from the other date.
}


Date::Date(int year, int month, int day)
{
    //REQUIRE(year > 0, "Year can not be negative!");
    REQUIRE(month > 0, "Month can not be negative!");
    REQUIRE(day > 0, "Day can not be negative!");

    std::chrono::year_month_day date{std::chrono::year(year), std::chrono::month(month), std::chrono::day(day)};

    REQUIRE(date.ok(), "Invalid date provided. Please check if this date really exists!");
    this->year = year;
    this->month = month;
    this->day = day;

    init_test_this_ptr = this;

    ENSURE(date.ok(), "Date creation failed. Date validity check did not pass.");
}

Date::Date(std::chrono::year_month_day year_month_day)
{
    REQUIRE(year_month_day.ok(), "Invalid date provided. Please check if this date really exists!");

    this->year = int(year_month_day.year());
    this->month = unsigned(year_month_day.month());
    this->day = unsigned(year_month_day.day());

    init_test_this_ptr = this;
    ENSURE(year_month_day.ok(), "Date creation failed. Date validity check did not pass.");
}

bool Date::isProperlyInitialized() const
{
    return init_test_this_ptr == this;
}


int Date::getYear() const
{
    ENSURE(isProperlyInitialized(), "Failed to get Year. Date must be properly initialized with the constructor!");

    return static_cast<int>(year);
}

unsigned Date::getMonth() const
{
    ENSURE(isProperlyInitialized(), "Failed to get Month. Date must be properly initialized with the constructor!");

    return month;
}

unsigned Date::getDay() const
{
    ENSURE(isProperlyInitialized(), "Failed to get Day. Date must be properly initialized with the constructor!");
    return day;
}

std::string Date::getWeekDay() const
{
    std::chrono::year_month_day date{std::chrono::year(year), std::chrono::month(month), std::chrono::day(day)};
    return std::format("{:%A}", std::chrono::weekday(date));
}

std::string Date::toString() const
{
    ENSURE(isProperlyInitialized(), "Failed to convert date to string. Date must be properly initialized with the constructor!");

    std::chrono::year_month_day date{std::chrono::year(year), std::chrono::month(month), std::chrono::day(day)};
    return std::format("{:%d/%m/%Y}", date);
}

std::ostream& operator<<(std::ostream& os, const Date& date)
{
    os << date.toString();
    return os;
}

bool Date::operator==(const Date& other) const
{
    return (other.day == day && other.month == month && other.year == year);
}

bool Date::operator<(const Date& other) const
{
    if (year == other.year)
    {
        if (month == other.month)
        {
            return day < other.day;
        }
        return month < other.month;
    }
    return year < other.year;
}

bool Date::operator>(const Date& other) const
{
    if (year == other.year)
    {
        if (month == other.month)
        {
            return day > other.day;
        }
        return month > other.month;
    }
    return year > other.year;
}

bool Date::operator<=(const Date& other) const
{
    return *this == other || *this < other;
}

bool Date::operator>=(const Date& other) const
{
    return *this == other || *this > other;
}

Date::~Date() = default;
