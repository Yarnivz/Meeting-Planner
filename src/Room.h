//
// Created by User on 2/26/2026.
//

#ifndef MEETING_PLANNER_ROOM_H
#define MEETING_PLANNER_ROOM_H
#include <string>


class Room {
public:
    Room(const std::string &name, const std::string &id, unsigned capacity);
    const std::string& getId();
    const std::string& toString();
    ~Room();
private:
    std::string name;
    std::string id;
    unsigned capacity;
};


#endif //MEETING_PLANNER_ROOM_H