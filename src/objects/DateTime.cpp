//
// Created by Lucas on 4/2/2026.
//

#include "DateTime.h"
#include <string>
#include <chrono>

#include "helper/DesignByContract.h"


DateTime::DateTime() : Date()
{
    const std::chrono::time_point now = std::chrono::system_clock::now();
    const std::chrono::time_point<std::chrono::system_clock, std::chrono::days> tpoint = std::chrono::floor<std::chrono::days>(now);
    const std::chrono::hh_mm_ss<std::chrono::nanoseconds> time = std::chrono::hh_mm_ss(now - tpoint);

    hour = unsigned(time.hours().count());

    //init_test_this_ptr = this;

    ENSURE(hour < 24, "DateTime creation failed. Hour needs to be >= 0 and < 24");
    ENSURE(getYear() == year, "DateTime creation failed. Year was not set correctly.");
    ENSURE(getMonth() == month, "DateTime creation failed. Month was not set correctly.");
    ENSURE(getDay() == day, "DateTime creation failed. Day was not set correctly.");
    ENSURE(getHour() == hour, "DateTime creation failed. Hour was not set correctly.");
    ENSURE(isProperlyInitialized(), "DateTime creation failed. Constructor did not properly initialize.");
}

DateTime::DateTime(const DateTime& d) : Date(d) {
    REQUIRE(d.isProperlyInitialized(), "Tried to copy a DateTime which was not properly initialized by the constructor.");

    hour = d.hour;

    //init_test_this_ptr = this; //Reset this pointer, do not copy it from the other DateTime.

    ENSURE(getYear() == d.year, "DateTime creation failed. Year was not set correctly.");
    ENSURE(getMonth() == d.month, "DateTime creation failed. Month was not set correctly.");
    ENSURE(getDay() == d.day, "DateTime creation failed. Day was not set correctly.");
    ENSURE(getHour() == d.hour, "DateTime creation failed. Hour was not set correctly.");
    ENSURE(isProperlyInitialized(), "DateTime creation failed. Constructor did not properly initialize.");
}

DateTime& DateTime::operator=(const DateTime& d)
{
    if (this == &d) return *this;

    REQUIRE(d.isProperlyInitialized(), "Tried to copy a DateTime which was not properly initialized by the constructor.");

    *(Date*)this = (Date&)d; // Copy base_class
    hour = d.hour;

    //init_test_this_ptr = this; //Reset this pointer, do not copy it from the other DateTime.

    ENSURE(getYear() == d.year, "DateTime creation failed. Year was not set correctly.");
    ENSURE(getMonth() == d.month, "DateTime creation failed. Month was not set correctly.");
    ENSURE(getDay() == d.day, "DateTime creation failed. Day was not set correctly.");
    ENSURE(getHour() == d.hour, "DateTime creation failed. Hour was not set correctly.");
    ENSURE(isProperlyInitialized(), "DateTime creation failed. Constructor did not properly initialize.");

    return *this;
}


DateTime::DateTime(int year, int month, int day, int hour) : Date(year, month, day)
{
    REQUIRE(hour >= 0, "Hour can't be negative! %i", hour);
    REQUIRE(hour < 24, "Hour must be less than 24! %i", hour);

    //Cast to unsigned
    unsigned int umonth = month;
    unsigned int uday = day;
    unsigned int uhour = hour;

    this->hour = uhour;

    //init_test_this_ptr = this;

    ENSURE(getYear() == year, "Year must be set correctly in order to create DateTime.");
    ENSURE(getMonth() == umonth, "Month must be set correctly in order to create DateTime.");
    ENSURE(getDay() == uday, "Day must be set correctly in order to create DateTime.");
    ENSURE(getHour() == uhour, "Hour must be set correctly in order to create DateTime.");
    ENSURE(isProperlyInitialized(), "Constructor must be properly initialized in order to create DateTime.");
}

DateTime::DateTime(const Date& date, int hour) : Date(date)
{
    REQUIRE(date.isProperlyInitialized(), "The given date must be properly initialized");
    REQUIRE(hour >= 0, "Hour can't be negative! %i", hour);
    REQUIRE(hour < 24, "Hour must be less than 24! %i", hour);

    unsigned uhour = hour;
    this->hour = uhour;


    //init_test_this_ptr = this;

    ENSURE(getYear() == date.getYear(), "Year must be set correctly in order to create DateTime.");
    ENSURE(getMonth() == date.getMonth(), "Month must be set correctly in order to create DateTime.");
    ENSURE(getDay() == date.getDay(), "Day must be set correctly in order to create DateTime.");
    ENSURE(getHour() == uhour, "Hour must be set correctly in order to create DateTime.");
    ENSURE(isProperlyInitialized(), "Constructor must be properly initialized in order to create DateTime.");
}

unsigned DateTime::getHour() const
{
    ENSURE(isProperlyInitialized(), "DateTime must be properly initialized with constructor in order to get Hour.");
    return hour;
}


std::string DateTime::toString() const
{
    ENSURE(isProperlyInitialized(), "DateTime must be properly initialized with constructor in order to convert DateTime into string.");

    std::chrono::year_month_day date{std::chrono::year(year), std::chrono::month(month), std::chrono::day(day)};
    std::stringstream ss;
    ss << std::format("{:%d/%m/%Y}", date);
    ss << ", " << hour << "h00";
    return ss.str();
}


std::ostream& operator<<(std::ostream& os, const DateTime& DateTime)
{
    os << DateTime.toString();
    return os;
}

bool DateTime::operator==(const DateTime& other) const
{
    return (Date::operator==(other) && other.hour == hour);
}

bool DateTime::operator<(const DateTime& other) const
{
    if (Date::operator<(other)) return true;
    return hour < other.hour;
}

bool DateTime::operator>(const DateTime& other) const
{
    if (Date::operator>(other)) return true;
    return hour > other.hour;
}

bool DateTime::operator<=(const DateTime& other) const
{
    return operator==(other) || operator<(other);
}

bool DateTime::operator>=(const DateTime& other) const
{
    return operator==(other) || operator>(other);
}

DateTime::~DateTime() = default;
