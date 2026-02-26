//
// Created by User on 2/26/2026.
//

#ifndef MEETING_PLANNER_ROOM_H
#define MEETING_PLANNER_ROOM_H
#include <string>


class Room {
public:
    Room(const std::string &name, const std::string &id, unsigned int capacity);
    const std::string& getId();
    ~Room();
private:
    std::string name;
    std::string id;
    unsigned int capacity;
};


#endif //MEETING_PLANNER_ROOM_H