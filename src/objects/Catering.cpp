//
// Created by lucas on 4/26/26.
//

#include "Catering.h"

#include "helper/DesignByContract.h"

Catering::Catering(Campus* campus, float co2)
{
    REQUIRE(co2 >= 0.0f, "The emission level can't be negative: %f", co2);
    this->campus = campus;
    this->emission = co2;

    this->init_check_this_ptr = this;
    ENSURE(getCampus() == campus, "Something went wrong. Campus was not set correctly.");
    ENSURE(getEmissions() == co2, "Something went wrong. Emissions were not set correctly.");
    ENSURE(isProperlyInitialized(), "Something went wrong. Catering was not properly initialized.");
}

bool Catering::isProperlyInitialized() const
{
    return init_check_this_ptr == this;
}

float Catering::getEmissions() const
{
    return emission;
}

Campus* Catering::getCampus() const
{
    return campus;
}
