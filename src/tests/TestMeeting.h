//
// Created by Lucas on 2026-03-12.
//
#pragma once

#include "gtest/gtest.h"
#include "objects/Catering.h"

#include "objects/Meeting.h"
#include "objects/Room.h"
#include "objects/User.h"

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

TEST_F(TestMeeting, Emmissions)
{
    { // External + Internal
        Campus* c = new Campus("campus", "c");
        Building* b = new Building("building", "b", c);
        Room* r = new Room("room", "r", 100, b);
        Meeting* m = new Meeting("meeting", "m", r, DateTime(2026,1,2,3), false, true, false);

        User* u1 = new User("Peter Selie", true);
        User* u2 = new User("John Doe", false);
        User* u3 = new User("Jane Doe", true);

        m->addParticipant(u1);
        EXPECT_EQ(1200.0f,m->getEmissions());
        m->addParticipant(u2);
        EXPECT_EQ(1320.0f,m->getEmissions());
        m->addParticipant(u3);
        EXPECT_EQ(2520.0f,m->getEmissions());

        delete c;
        delete b;
        delete r;
        delete m;
        delete u1;
        delete u2;
        delete u3;
    }

    { // External + Internal + Catering
        Campus* c = new Campus("campus", "c");
        Catering* cat = new Catering(c, 10.0);
        Building* b = new Building("building", "b", c);
        Room* r = new Room("room", "r", 100, b);
        Meeting* m = new Meeting("meeting", "m", r, DateTime(2026,1,2,3), false, true, true);

        User* u1 = new User("Peter Selie", true);
        User* u2 = new User("John Doe", false);
        User* u3 = new User("Jane Doe", true);

        m->addParticipant(u1);
        EXPECT_EQ(1200.0f + 10.0f,m->getEmissions());
        m->addParticipant(u2);
        EXPECT_EQ(1320.0f + 20.0f,m->getEmissions());
        m->addParticipant(u3);
        EXPECT_EQ(2520.0f + 30.0f,m->getEmissions());

        delete c;
        delete cat;
        delete b;
        delete r;
        delete m;
        delete u1;
        delete u2;
        delete u3;
    }

    { // Empty
        Campus* c = new Campus("campus", "c");
        Catering* cat = new Catering(c, 10.0);
        Building* b = new Building("building", "b", c);
        Room* r = new Room("room", "r", 100, b);
        Meeting* m = new Meeting("meeting", "m", r, DateTime(2026,1,2,3), true, false, false);


        EXPECT_EQ(0.0f, m->getEmissions());

        delete c;
        delete cat;
        delete b;
        delete r;
        delete m;
    }

    { // Online
        Campus* c = new Campus("campus", "c");
        Building* b = new Building("building", "b", c);
        Room* r = new Room("room", "r", 100, b);
        Meeting* m = new Meeting("meeting", "m", r, DateTime(2026,1,2,3), true, false, false);

        User* u1 = new User("Peter Selie", false);
        User* u2 = new User("John Doe", false);
        User* u3 = new User("Jane Doe", false);

        m->addParticipant(u1);
        EXPECT_EQ(30.0f,m->getEmissions());
        m->addParticipant(u2);
        EXPECT_EQ(60.0f,m->getEmissions());
        m->addParticipant(u3);
        EXPECT_EQ(90.0f,m->getEmissions());

        delete c;
        delete b;
        delete r;
        delete m;
        delete u1;
        delete u2;
        delete u3;
    }

}

TEST_F(TestMeeting, ContractViolation)
{
    EXPECT_DEATH(Meeting("hello", "", nullptr, DateTime(1,1,1,1)), "");
    EXPECT_DEATH(Meeting("world", "some_id", nullptr, DateTime(1,1,1,1)), "");
}