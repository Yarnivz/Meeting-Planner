//
// Created by Lucas on 2026-03-12.
//
#pragma once

#include "gtest/gtest.h"

#include "objects/Meeting.h"
#include "objects/Room.h"

class TestMeeting : public ::testing::Test
{
protected:
};


TEST_F(TestMeeting, HappyDay)
{
    Campus* c = new Campus ("Middelheim", "M");
    ASSERT_TRUE(c->isProperlyInitialized());
    Building* b = new Building("Bib", "G", c);
    ASSERT_TRUE(b->isProperlyInitialized());
    Room* r = new Room("label", "some_room", 123, b);
    ASSERT_TRUE(r->isProperlyInitialized());

    DateTime dt = DateTime(2025, 2, 2, 0);
    ASSERT_TRUE(dt.isProperlyInitialized());

    Meeting meeting = Meeting("My Meeting", "meeting0", r, dt);

    ASSERT_TRUE(meeting.isProperlyInitialized());
    EXPECT_EQ("My Meeting", meeting.toString());
    EXPECT_EQ("meeting0", meeting.getId());
    EXPECT_EQ(r, meeting.getRoom());


    const DateTime& fetched_dt = meeting.getDateTime();
    EXPECT_TRUE(fetched_dt.isProperlyInitialized());
    EXPECT_EQ(dt, fetched_dt);

    delete c;
    delete b;
    delete r;
}

TEST_F(TestMeeting, ContractViolation)
{
    EXPECT_DEATH(Meeting("hello", "", nullptr, DateTime(1,1,1,1)), "");
    EXPECT_DEATH(Meeting("world", "some_id", nullptr, DateTime(1,1,1,1)), "");
}