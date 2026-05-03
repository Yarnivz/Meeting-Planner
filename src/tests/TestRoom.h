//
// Created by Lucas on 2026-03-12.
//

#ifndef MEETING_PLANNER_ROOMTEST_H
#define MEETING_PLANNER_ROOMTEST_H

#include "objects/Room.h"
#include "gtest/gtest.h"

class TestRoom : public ::testing::Test
{
protected:
};

TEST_F(TestRoom, HappyDay)
{
    Campus* campus1 = new Campus ("Middelheim", "M");
    Building* building1 = new Building("Bib", "G", campus1);

    Room r("A Room", "a_room_id", 20, building1);

    EXPECT_TRUE(r.isProperlyInitialized());
    EXPECT_EQ(20u, r.getCapacity());
    EXPECT_EQ("A Room", r.toString());
    EXPECT_EQ("a_room_id", r.getId());
    EXPECT_EQ(building1, r.getBuilding());
    EXPECT_EQ(campus1, r.getCampus());
}

TEST_F(TestRoom, ContractViolation)
{
    Campus* campus1 = new Campus ("Middelheim", "M");
    Building* building1 = new Building("Bib", "G", campus1);

    EXPECT_DEATH(Room("Room With Empty Id", "", 20, building1), "");
    EXPECT_DEATH(Room("Room With No Capacity", "a_id", 0, building1), "");
    EXPECT_DEATH(Room("Room With No Building", "a_id", 20, nullptr), "");

    delete campus1;
    delete building1;
}

TEST_F(TestRoom, CopyConstructor)
{
    Campus* campus = new Campus ("Middelheim", "M");
    Building* building = new Building("Bib", "G", campus);

    Room r1("A Room", "room20", 13, building);
    Room r2 = r1;

    EXPECT_TRUE(r1.isProperlyInitialized());
    EXPECT_TRUE(r2.isProperlyInitialized());
    EXPECT_EQ("A Room", r1.toString());
    EXPECT_EQ("A Room", r2.toString());
    EXPECT_EQ("room20", r1.getId());
    EXPECT_EQ("room20", r2.getId());
    EXPECT_EQ(13u, r1.getCapacity());
    EXPECT_EQ(13u, r2.getCapacity());
    EXPECT_EQ(building, r1.getBuilding());
    EXPECT_EQ(building, r2.getBuilding());
    EXPECT_EQ(campus, r1.getCampus());
    EXPECT_EQ(campus, r2.getCampus());

    delete campus;
    delete building;

}

#endif //MEETING_PLANNER_ROOMTEST_H