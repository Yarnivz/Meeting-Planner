//
// Created by User on 2/26/2026.
//

#ifndef MEETING_PLANNER_MEETING_H
#define MEETING_PLANNER_MEETING_H
#include <string>
#include "Date.h"

class Meeting {
public:
    Meeting(std::string label, std::string id, std::string room, Date date);
    void process();
    ~Meeting();
private:
    std::string label, id, room;
    Date date;
};


#endif //MEETING_PLANNER_MEETING_H