//
// Created by Yarni on 2/26/2026.
//

#ifndef MEETING_PLANNER_ROOM_H
#define MEETING_PLANNER_ROOM_H
#include <iostream>
#include <map>
#include "../TypeDefs.h"
#include "Building.h"
#include "Date.h"


class Room
{
public:
    /**
     * @brief Creates the Room class.
     * This is the place where a meeting can take place.
     * A room can only be initialized using the name, id and capacity parameters as these are required.
     *
     *     * @pre name must not be empty
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
     * @post Room creation failed. Object was not properly initialized.
     * @post Room creation failed. Capacity was not set correctly.
     * @post Room creation failed. Id was not set correctly.
     */
    Room(const std::string& name, const std::string& id, unsigned capacity, Building* building = nullptr);

    /**
     * @brief Copies a Room object.
     * It copies all important values but makes sure the 'properlyInitialized' test still passes.
     *
     * @pre Tried to copy a room which was not properly initialized by the constructor.
     *
     * @param r Room to be copied
     *
     * @post Room creation failed. Object was not properly initialized.
     * @post Room creation failed. Capacity was not set correctly.
     * @post Room creation failed. Id was not set correctly.
     * @post Room creation failed. Campus was not set correctly.
     * @post Room creation failed. Building was not set correctly.
     */
    Room(const Room& r);

    /**
     *
     * @pre Tried to copy a room which was not properly initialized by the constructor.
     *
     *
     * @post Room creation failed. Object was not properly initialized.
     * @post Room creation failed. Capacity was not set correctly.
     * @post Room creation failed. Id was not set correctly.
     * @post Room creation failed. Campus was not set correctly.
     * @post Room creation failed. Building was not set correctly.
     */
    Room& operator=(const Room& r);

    /**
     * @brief Checks whether this Room was properly initialized by the constructor.
     * @return bool indicating result
     */
    bool isProperlyInitialized() const;

    /**
    * @brief Gets the rooms building
     *
     * @post Building cannot be null.
    * @return the building itself
    */
    Building* getBuilding() const;

    /**
    * @brief Gets the rooms campus
     *
     * @post Campus cannot be null.
    * @return the campus itself
    */
    Campus* getCampus() const;

    /**
     * @brief Id getter.
     *
     * @post id must not be empty
     *
     * @return the identifier of the room
     */
    const std::string& getId() const;

    /**
     * @brief Converts the room class to a readable string format.
     *
     * @post name must not be empty
     *
     * @return the room in string format
     */
    const std::string& toString() const;

    /**
     * @brief Capacity getter.
     *
     *
     *
     * @post Capacity must be greater than 0
     *
     * @return the maximum capacity of this room
     */
    unsigned getCapacity() const;

    /**
     * @brief Adds a renovation object to the Room using the start and end date
     *
     *
     * @pre Start date must be properly initialized.
     * @pre End date must be properly initialized.
     *
     * @param start Date of the renovation
     * @param end Date of the renovation
     *
     *
     * @post Renovation must have been added to the Room.
     */
    void addRenovation(const Date& start, const Date& end);

    /**
     * @brief Gets the renovation if the current date is between the start and end date
     *
     * @param date The date of the renovation
     *
     * @return The renovation object itself
     */
    const Renovation* getRenovation(const Date& date) const;

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
    Renovations renovations;
    void* init_check_this_ptr;
};


#endif //MEETING_PLANNER_ROOM_H
