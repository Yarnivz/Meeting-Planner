//
// Created by lucas on 4/5/26.
//

#ifndef MEETING_PLANNER_CONTAINERS_H
#define MEETING_PLANNER_CONTAINERS_H

#include <map>
#include <string>
#include <unordered_map>
class User;
class Room;

using Rooms = std::unordered_map<std::string, Room*>;
using Users = std::map<std::string, User*>;

#endif //MEETING_PLANNER_CONTAINERS_H
