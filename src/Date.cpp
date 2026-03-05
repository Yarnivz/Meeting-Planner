//
// Created by User on 2/26/2026.
//

#include "Date.h"
#include <string>
#include <chrono>
#include <stdexcept>

Date::Date() {
    const std::chrono::time_point now = std::chrono::system_clock::now();
    const std::chrono::year_month_day current_date = std::chrono::floor<std::chrono::days>(now);
    year = current_date.year();
    month = current_date.month();
    day = current_date.day();
}

Date::Date(int year, int month, int day) {
    std::chrono::year_month_day date{std::chrono::year(year),std::chrono::month(month),std::chrono::day(day)};
    if (date.ok()) {
        this->year = std::chrono::year(year);
        this->month = std::chrono::month(month);
        this->day = std::chrono::day(day);
    } else {
        throw std::invalid_argument("Invalid date provided. Please check if this date really exists!");
    }
}

int Date::getYear() { return static_cast<int>(year); }

int Date::getMonth() { return static_cast<unsigned>(month); }

int Date::getDay() { return static_cast<unsigned>(day); }

std::string Date::toString() const {
    std::chrono::year_month_day date{std::chrono::year(year),std::chrono::month(month),std::chrono::day(day)};
    return std::format("{:%Y-%m-%d}", date);
}

Date::~Date() = default;
