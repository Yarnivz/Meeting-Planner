//
// Created by lucas on 4/5/26.
//

#ifndef MEETING_PLANNER_TYPEDEFS_H
#define MEETING_PLANNER_TYPEDEFS_H

#include <map>
#include <string>
#include <unordered_map>

class User;
class Room;
class Building;
class Campus;
class Date;

using Campuses = std::unordered_map<std::string, Campus*>;
using Buildings = std::unordered_map<std::string, Building*>;
using Rooms = std::unordered_map<std::string, Room*>;
using Users = std::map<std::string, User*>;

using Renovation = std::pair<const Date, Date>;
using Renovations = std::map<Date, Date>;

#endif //MEETING_PLANNER_TYPEDEFS_H
