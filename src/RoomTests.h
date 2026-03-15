//
// Created by Lucas on 2026-03-12.
//

#ifndef MEETING_PLANNER_ROOMTEST_H
#define MEETING_PLANNER_ROOMTEST_H

#include "Room.h"
#include "gtest/gtest.h"

class RoomTests : public ::testing::Test {
protected:
};

TEST_F(RoomTests, HappyDay) {
    Room r("A Room", "a_room_id", 20);

    EXPECT_TRUE(r.isProperlyInitialized());
    EXPECT_EQ(20u, r.getCapacity());
    EXPECT_EQ("A Room", r.toString());
    EXPECT_EQ("a_room_id", r.getId());
}

TEST_F(RoomTests, ContractViolation) {
    EXPECT_DEATH(Room("Room With Empty Id", "", 20), "");
    EXPECT_DEATH(Room("Room With No Capacity", "a_id", 0), "");
}

TEST_F(RoomTests, CopyConstructor) {
    Room r1("A Room", "room20", 13);
    Room r2 = r1;

    EXPECT_TRUE(r1.isProperlyInitialized());
    EXPECT_TRUE(r2.isProperlyInitialized());
    EXPECT_EQ("A Room", r1.toString());
    EXPECT_EQ("A Room", r2.toString());
    EXPECT_EQ("room20", r1.getId());
    EXPECT_EQ("room20", r2.getId());
    EXPECT_EQ(13u, r1.getCapacity());
    EXPECT_EQ(13u, r2.getCapacity());
}

#endif //MEETING_PLANNER_ROOMTEST_H