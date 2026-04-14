//
// Created by Yarni on 2/26/2026.
//

#ifndef MEETING_PLANNER_ROOM_H
#define MEETING_PLANNER_ROOM_H
#include <iostream>
#include <string>
#include "Building.h"
#include "Campus.h"


class Room
{
public:
    /**
     * @brief Creates the Room class.
     * This is the place where a meeting can take place.
     * A room can only be initialized using the name, id and capacity parameters as these are required.
     *
     * @param name the name of this room
     * @param id the identifier of this room
     * @param capacity the amount of people that fit in this room
     * @param building the building in which this room can be found
     */
    Room(const std::string& name, const std::string& id, unsigned capacity, Building* building);

    /**
     * @brief Copies a Room object.
     * It copies all important values but makes sure the 'properlyInitialized' test still passes.
     * @param r Room to be copied
     */
    Room(const Room& r);


    /**
     * @brief Checks whether this Room was properly initialized by the constructor.
     * @return bool indicating result
     */
    bool isProperlyInitialized() const;


    /**
     * @brief Id getter.
     *
     * @return the identifier of the room
     */
    const std::string& getId() const;

    /**
     * @brief Converts the room class to a readable string format.
     *
     * @return the room in string format
     */
    const std::string& toString() const;

    /**
     * @brief Capacity getter.
     * @return the maximum capacity of this room
     */
    unsigned getCapacity() const;

    /**
     * @brief Standard stream operator using the toString function to append the room to the stream.
     * @param os stream to write on
     * @param room to write on the stream
     * @return the stream
     */
    friend std::ostream& operator <<(std::ostream& os, const Room& room);

    /**
     * @brief Default destructor
     */
    ~Room();

private:
    std::string name;
    std::string id;
    unsigned capacity;
    unsigned occupancy;
    Campus* campus;
    Building* building;
    void* init_check_this_ptr;
};


#endif //MEETING_PLANNER_ROOM_H
