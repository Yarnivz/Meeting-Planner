//
// Created by User on 2/26/2026.
//

#ifndef MEETING_PLANNER_ROOM_H
#define MEETING_PLANNER_ROOM_H
#include <string>


class Room {
public:
    Room(std::string name, std::string id, unsigned int capacity);
    ~Room();
private:
    std::string name, id;
    unsigned int capacity;
};


#endif //MEETING_PLANNER_ROOM_H