//
// Created by User on 2/26/2026.
//

#ifndef MEETING_PLANNER_PARTICIPATION_H
#define MEETING_PLANNER_PARTICIPATION_H
#include <string>

class Participation {
public:
    Participation(std::string user, std::string meeting);
    ~Participation();
private:
    std::string user, meeting;
};


#endif //MEETING_PLANNER_PARTICIPATION_H