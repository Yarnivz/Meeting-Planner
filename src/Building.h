//
// Created by User on 3/29/2026.
//

#ifndef MEETING_PLANNER_BUILDING_H
#define MEETING_PLANNER_BUILDING_H
#include <string>


class Building {
public:
    /**
     * @brief Creates the Building class.
     * This is the object where rooms will be inside.
     * A building has to be initialized using its name, id and the campus where this building can be found.
     *
     * @param name the name of this building
     * @param id the unique identifier of this building that can be used to call this object from the app.
     * @param campus the campus that houses this building
     */
    Building(const std::string& name, const std::string& id, const std::string& campus);

    /**
     * @brief Checks whether this Room was properly initialized by the constructor.
     * @return bool indicating result
     */
    bool isProperlyInitialized() const;

    /**
     * @brief Id getter.
     *
     * @return the identifier of this building
     */
    const std::string& getId() const;

    /**
     * @brief Campus id getter.
     *
     * @return the identifier of the campus this building is located on
     */
    const std::string& getCampusId() const;

    /**
     * @brief Converts this building object into a readable string format.
     * This is represented by its name.
     *
     * @return the building in string format
     */
    const std::string& toString() const;

    /**
     * @brief Standard stream operator using the toString function to append the building to the stream.
     * @param os stream to write on
     * @param building object to write on the stream
     * @return the stream
     */
    friend std::ostream& operator <<(std::ostream& os, const Building& building);

    /**
     * @brief Default destructor.
     */
    ~Building();


private:
    std::string name;
    std::string id;
    std::string campus;

    void* init_check_this_ptr;
};


#endif //MEETING_PLANNER_BUILDING_H