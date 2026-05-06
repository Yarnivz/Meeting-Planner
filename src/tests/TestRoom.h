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

    delete building1;
    delete campus1;
}

TEST_F(TestRoom, ContractViolation)
{
    Campus* campus1 = new Campus ("Middelheim", "M");
    Building* building1 = new Building("Bib", "G", campus1);

    EXPECT_DEATH(Room("Room With Empty Id", "", 20, building1), "");
    EXPECT_DEATH(Room("Room With No Capacity", "a_id", 0, building1), "");
    EXPECT_DEATH(Room("Room With No Building", "a_id", 20, nullptr), "");

    delete building1;
    delete campus1;
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

    delete building;
    delete campus;
}

TEST_F(TestRoom, Renovations)
{
    Campus* campus = new Campus ("Middelheim", "M");
    Building* building = new Building("Gebouw G", "G", campus);

    Room r1("A Room", "room21", 10, building);
    Room r2("Another Room", "room22", 15, building);
    Room r3("Yet another Room", "room23", 20, building);

    r1.addRenovation(Date(2026,05,31), Date(2026,06,06));
    r2.addRenovation(Date(2026,04,20), Date(2026,05,01));
    r3.addRenovation(Date(2026,06,06), Date(2026,06,26));

    //Check if renovations can be found with their start date
    EXPECT_TRUE(r1.getRenovation(Date(2026,05,31)));
    EXPECT_TRUE(r2.getRenovation(Date(2026,04,20)));
    EXPECT_TRUE(r3.getRenovation(Date(2026,06,06)));

    //Check if renovations can be found with their end date
    EXPECT_TRUE(r1.getRenovation(Date(2026,06,06)));
    EXPECT_TRUE(r2.getRenovation(Date(2026,05,01)));
    EXPECT_TRUE(r3.getRenovation(Date(2026,06,26)));

    //Check if renovations can be found with a date in between
    EXPECT_TRUE(r1.getRenovation(Date(2026,06,01)));
    EXPECT_TRUE(r2.getRenovation(Date(2026,04,24)));
    EXPECT_TRUE(r3.getRenovation(Date(2026,06,18)));

    //Check if renovations cannot be found by giving wrong dates
    EXPECT_FALSE(r1.getRenovation(Date(2026,07,04)));
    EXPECT_FALSE(r2.getRenovation(Date(2026,06,20)));
    EXPECT_FALSE(r3.getRenovation(Date(2026,05,01)));

    delete building;
    delete campus;
}

#endif //MEETING_PLANNER_ROOMTEST_H