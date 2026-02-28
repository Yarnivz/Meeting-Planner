//
// Created by User on 2/26/2026.
//

#ifndef MEETING_PLANNER_MEETING_H
#define MEETING_PLANNER_MEETING_H
#include <string>
#include "Date.h"

class Meeting {
public:
    Meeting(const std::string &label, const std::string &id, const std::string &room, const Date &date);
    const std::string& getId();
    void process();
    ~Meeting();
private:
    std::string label;
    std::string id;
    std::string room;
    Date date;
};


#endif //MEETING_PLANNER_MEETING_H