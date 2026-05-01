//
// Created by lucas on 4/26/26.
//

#ifndef MEETING_PLANNER_CATERING_H
#define MEETING_PLANNER_CATERING_H

class Campus;


class Catering {
public:
    Catering(Campus* campus, float co2);
    Catering(const Catering&) = delete;

    bool isProperlyInitialized() const;

    float getEmissions() const;
    Campus* getCampus() const;
private:
    float emission;
    Campus* campus;
    void* init_check_this_ptr;
};



#endif //MEETING_PLANNER_CATERING_H
