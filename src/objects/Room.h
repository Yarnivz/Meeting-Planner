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
     *
     * @pre name must not be empty
     * @pre id must not be empty
     * @pre capacity must be greater than 0
     * @pre The given building must exist
     * @pre The given building must be properly initialized
     *
     * @param name the name of this room
     * @param id the identifier of this room
     * @param capacity the amount of people that fit in this room
     * @param building the building in which this room can be found
     *
     * @post     // IDEE: ENSURE(isProperlyInitialized)
     */
    Room(const std::string& name, const std::string& id, unsigned capacity, Building* building = nullptr);

    /**
     * @brief Copies a Room object.
     * It copies all important values but makes sure the 'properlyInitialized' test still passes.
     *
     * @pre Tried to copy a room which was not properly initialized by the constructor.
     *
     * @param r Room to be copied
     */
    Room(const Room& r);
    Room& operator=(const Room& r);

    /**
     * @brief Checks whether this Room was properly initialized by the constructor.
     * @return bool indicating result
     */
    bool isProperlyInitialized() const;

    /**
    * @brief Gets the rooms building
    * @return the building itself
    */
    Building* getBuilding() const;

    /**
    * @brief Gets the rooms campus
    * @return the campus itself
    */
    Campus* getCampus() const;

    /**
     * @brief Id getter.
     *
     *
     * @pre id must not be empty
     *
     * @return the identifier of the room
     */
    const std::string& getId() const;

    /**
     * @brief Converts the room class to a readable string format.
     *
     *
     * @pre name must not be empty
     *
     * @return the room in string format
     */
    const std::string& toString() const;

    /**
     * @brief Capacity getter.
     *
     * @pre Capacity must be greater than 0
     *
     * @return the maximum capacity of this room
     */
    unsigned getCapacity() const;

    /**
     * @brief Standard stream operator using the toString function to append the room to the stream.
     *
     * @pre room must be properly initialized
     *
     * @param os stream to write on
     * @param room to write on the stream
     *
     * @post ostream variable is not usable
     * @return the stream
     */
    friend std::ostream& operator<<(std::ostream& os, const Room& room);

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
