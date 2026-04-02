//
// Created by Lucas on 4/2/2026.
//

#include "DateTime.h"
#include <string>
#include <chrono>

#include "helper/DesignByContract.h"


DateTime::DateTime()
{
    const std::chrono::time_point now = std::chrono::system_clock::now();
    const std::chrono::time_point<std::chrono::system_clock, std::chrono::days> tpoint = std::chrono::floor<std::chrono::days>(now);
    const std::chrono::year_month_day ymd = tpoint;
    const std::chrono::hh_mm_ss<std::chrono::nanoseconds> time = std::chrono::hh_mm_ss(now - tpoint);

    year = int(ymd.year());
    month = unsigned(ymd.month());
    day = unsigned(ymd.day());
    hour = unsigned(time.hours().count());

    init_test_this_ptr = this;

    ENSURE(ymd.ok(), "DateTime creation failed. Date validity check did not pass.");
    ENSURE(hour < 24, "DateTime creation failed. Hour needs to be >= 0 and < 24");
}

DateTime::DateTime(const DateTime& d)
{
    REQUIRE(d.isProperlyInitialized(), "Tried to copy a DateTime which was not properly initialized by the constructor.");

    year = d.year;
    month = d.month;
    day = d.day;

    init_test_this_ptr = this; //Reset this pointer, do not copy it from the other DateTime.
}


DateTime::DateTime(int year, int month, int day, int hour)
{
    REQUIRE(year > 0, "Year can not be negative!");
    REQUIRE(month > 0, "Month can not be negative!");
    REQUIRE(day > 0, "Day can not be negative!");
    REQUIRE(hour >= 0, "Hour can't be negative");
    REQUIRE(hour < 24, "Hour must be less than 24");

    std::chrono::year_month_day date{std::chrono::year(year), std::chrono::month(month), std::chrono::day(day)};

    REQUIRE(date.ok(), "Invalid date provided. Please check if this date really exists!");

    //Cast to unsigned
    unsigned umonth = month;
    unsigned uday = day;
    unsigned uhour = hour;

    this->year = year;
    this->month = umonth;
    this->day = uday;
    this->hour = uhour;

    init_test_this_ptr = this;

    ENSURE(year == getYear() && umonth == getMonth() && uday == getDay() && uhour == getHour(), "DateTime creation failed. Values were not correctly assigned.");
}

// DateTime::DateTime(std::chrono::year_month_day year_month_day)
// {
//     REQUIRE(year_month_day.ok(), "Invalid DateTime provided. Please check if this DateTime really exists!");
//
//     this->year = year_month_day.year();
//     this->month = year_month_day.month();
//     this->day = year_month_day.day();
//
//     init_test_this_ptr = this;
//     ENSURE(year_month_day.ok(), "DateTime creation failed. DateTime validity check did not pass.");
// }

bool DateTime::isProperlyInitialized() const
{
    return init_test_this_ptr == this;
}


int DateTime::getYear() const
{
    ENSURE(isProperlyInitialized(), "Failed to get Year. DateTime must be properly initialized with the constructor!");
    return year;
}

unsigned DateTime::getMonth() const
{
    ENSURE(isProperlyInitialized(), "Failed to get Month. DateTime must be properly initialized with the constructor!");
    return month;
}

unsigned DateTime::getDay() const
{
    ENSURE(isProperlyInitialized(), "Failed to get Day. DateTime must be properly initialized with the constructor!");
    return day;
}

unsigned DateTime::getHour() const
{
    ENSURE(isProperlyInitialized(), "Failed to get Hour. DateTime must be properly initialized with the constructor!");
    return hour;
}


std::string DateTime::getWeekDay() const
{
    std::chrono::year_month_day date{std::chrono::year(year), std::chrono::month(month), std::chrono::day(day)};
    return std::format("{:%A}", std::chrono::weekday(date));
}

std::string DateTime::toString() const
{
    ENSURE(isProperlyInitialized(), "Failed to convert DateTime to string. DateTime must be properly initialized with the constructor!");

    std::chrono::year_month_day date{std::chrono::year(year), std::chrono::month(month), std::chrono::day(day)};
    return std::format("{:%d/%m/%Y}", date);
}



std::ostream& operator<<(std::ostream& os, const DateTime& DateTime)
{
    os << DateTime.toString();
    return os;
}

bool DateTime::operator==(const DateTime& other) const
{
    return (other.day == day && other.month == month && other.year == year && other.hour == hour);
}

bool DateTime::operator<(const DateTime& other) const
{
    if (year != other.year) return year < other.year;
    if (month != other.month) return month < other.month;
    if (day != other.day) return day < other.day;
    if (hour != other.hour) return hour < other.hour;
    return false;
}

bool DateTime::operator>(const DateTime& other) const
{
    if (year != other.year) return year > other.year;
    if (month != other.month) return month > other.month;
    if (day != other.day) return day > other.day;
    if (hour != other.hour) return hour > other.hour;
    return false;
}

bool DateTime::operator<=(const DateTime& other) const
{
    if (year != other.year) return year < other.year;
    if (month != other.month) return month < other.month;
    if (day != other.day) return day < other.day;
    if (hour != other.hour) return hour < other.hour;
    return true;
}

bool DateTime::operator>=(const DateTime& other) const
{
    if (year != other.year) return year > other.year;
    if (month != other.month) return month > other.month;
    if (day != other.day) return day > other.day;
    if (hour != other.hour) return hour > other.hour;
    return true;
}

DateTime::~DateTime() = default;
