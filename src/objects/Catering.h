//
// Created by lucas on 4/26/26.
//

#ifndef MEETING_PLANNER_CATERING_H
#define MEETING_PLANNER_CATERING_H

class Campus;


class Catering {
public:
    Catering(Campus* campus, float co2);


    /**
     * @briefprevents the Catering object from being copied
     */
    Catering(const Catering&) = delete;
    Catering& operator=(const Catering&) = delete;

    /**
     * @brief Checks whether this Catering was properly initialized by the constructor.
     * @return bool indicating result
     */
    bool isProperlyInitialized() const;

    /**
     * @brief gets the caterings co2 emission amount
     * @return the emission amount itself
     */
    float getEmissions() const;
    /**
     * @brief Gets the caterings campus
     * @return the campus itself
     */
    Campus* getCampus() const;
private:
    float emission;
    Campus* campus;
    void* init_check_this_ptr;
};



#endif //MEETING_PLANNER_CATERING_H
