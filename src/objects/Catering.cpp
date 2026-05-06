//
// Created by lucas on 4/26/26.
//

#include "Catering.h"

#include "Campus.h"
#include "helper/DesignByContract.h"

Catering::Catering(Campus* campus, float co2)
{
    REQUIRE(co2 >= 0.0f, "The emission level can't be negative: %f", co2);
    REQUIRE(campus != nullptr, "Campus can't be null");

    this->campus = campus;
    this->emission = co2;

    this->init_check_this_ptr = this;


    ENSURE(getCampus() == campus, "Campus must be set correctly.");
    ENSURE(getEmissions() == co2, "Emissions must be set correctly.");
    ENSURE(isProperlyInitialized(), "Catering must be properly initialized.");

    campus->_addCatering(this);
    ENSURE(campus->getCaterings().back() == this, "Catering must be added to the campus");
}

bool Catering::isProperlyInitialized() const
{
    return init_check_this_ptr == this;
}

float Catering::getEmissions() const
{
    REQUIRE(emission > 0, "emissions cant be negative");
    return emission;
}

Campus* Catering::getCampus() const
{
    return campus;
}
