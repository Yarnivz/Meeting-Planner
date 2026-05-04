//
// Created by Yarni on 3/13/2026.
//

#ifndef MEETING_PLANNER_PARSEFILETESTS_H
#define MEETING_PLANNER_PARSEFILETESTS_H
#include <complex>
#include <fstream>
#include <gtest/gtest.h>

#include "FileUtils.h"
#include "App.h"
#include "parser/XmlParser.h"

class TestParseFile : public ::testing::Test
{
};

TEST_F(TestParseFile, HappyDay1)
{
    const std::string error = "./test-files/TestParseFile.HappyDay1-errors.txt";
    std::ofstream errLog(error);

    App app = App(new XmlParser(), nullptr);
    ASSERT_TRUE(app.isProperlyInitialized());

    app.parseFile("./test-files/HappyDay1.xml", errLog);

    Campus* c = app.getCampus("M");
    ASSERT_NE(nullptr, c);
    EXPECT_TRUE(c->isProperlyInitialized());
    EXPECT_EQ("Campus Middelheim", c->toString());
    Building* b = app.getBuilding("M.G");
    ASSERT_NE(nullptr, b);
    EXPECT_TRUE(b->isProperlyInitialized());
    EXPECT_EQ("Gebouw G", b->toString());
    EXPECT_EQ(c, b->getCampus());

    //Test room #1
    Room* room = app.getRoom("M.G.025");
    ASSERT_NE(nullptr, room);
    EXPECT_TRUE(room->isProperlyInitialized());
    EXPECT_EQ("M.G.025", room->toString());
    EXPECT_EQ(55u, room->getCapacity());
    EXPECT_EQ(b, room->getBuilding());
    EXPECT_EQ(c, room->getCampus());

    //Test room #2
    room = app.getRoom("Room98732");
    ASSERT_NE(nullptr, room);
    EXPECT_TRUE(room->isProperlyInitialized());
    EXPECT_EQ("M.G.023", room->toString());
    EXPECT_EQ(10u, room->getCapacity());
    EXPECT_EQ(b, room->getBuilding());
    EXPECT_EQ(c, room->getCampus());

    //Test meeting
    const std::string meeting_id = "Meeting_478463";
    const Meeting* meeting = app.getMeetingById(meeting_id);
    ASSERT_NE(nullptr, meeting);
    EXPECT_TRUE(meeting->isProperlyInitialized());
    EXPECT_EQ("Weekly meeting", meeting->toString());
    EXPECT_EQ(room, meeting->getRoom());
    EXPECT_EQ(DateTime(2026, 5, 22, 13), meeting->getDateTime());
    EXPECT_FALSE(meeting->cateringNeeded());
    EXPECT_FALSE(meeting->externalsAllowed());
    EXPECT_FALSE(meeting->isOnline());

    //Test participation
    const User* peter = app.getUser("Peter Selie");
    ASSERT_NE(nullptr, peter);
    EXPECT_FALSE(peter->isExternal());
    EXPECT_EQ(meeting, peter->getMeetingById(meeting_id));
    EXPECT_EQ(peter, meeting->getParticipant("Peter Selie"));

    EXPECT_TRUE(file_is_empty(error));
}


TEST_F(TestParseFile, HappyDay2)
{
    const std::string errors = "./test-files/TestParseFile.HappyDay2-errors.txt";
    std::ofstream errLog(errors);


    App app = App(new XmlParser(), nullptr);

    ASSERT_TRUE(app.isProperlyInitialized());
    app.parseFile("./test-files/HappyDay2.xml", errLog);

    Campus* c = app.getCampus("M");
    ASSERT_NE(nullptr, c);
    EXPECT_TRUE(c->isProperlyInitialized());
    EXPECT_EQ("Campus Middelheim", c->toString());
    Building* bG = app.getBuilding("M.G");
    ASSERT_NE(nullptr, bG);
    EXPECT_TRUE(bG->isProperlyInitialized());
    EXPECT_EQ("Gebouw G", bG->toString());
    EXPECT_EQ(c, bG->getCampus());
    Building* bA = app.getBuilding("M.A");
    ASSERT_NE(nullptr, bA);
    EXPECT_TRUE(bA->isProperlyInitialized());
    EXPECT_EQ("Gebouw A", bA->toString());
    EXPECT_EQ(c, bA->getCampus());

    //Test room #1
    Room* room = app.getRoom("id_026");
    ASSERT_NE(nullptr, room);
    EXPECT_EQ(room->toString(), "M.G.026");
    EXPECT_EQ(room->getCapacity(), 30u);
    EXPECT_EQ(bG, room->getBuilding());
    EXPECT_EQ(c, room->getCampus());

    //Test room #2
    Room* room2 = app.getRoom("Room98732");
    ASSERT_NE(nullptr, room2);
    EXPECT_EQ("M.G.023", room2->toString());
    EXPECT_EQ(10u, room2->getCapacity());
    EXPECT_EQ(bG, room2->getBuilding());
    EXPECT_EQ(c, room2->getCampus());

    //Test room #3
    Room* room3 = app.getRoom("Aula23");
    ASSERT_NE(nullptr, room3);
    EXPECT_EQ("M.A.043", room3->toString());
    EXPECT_EQ(46u, room3->getCapacity());
    EXPECT_EQ(bA, room3->getBuilding());
    EXPECT_EQ(c, room3->getCampus());

    //Test meeting #1
    const std::string meeting_id = "Meeting_478463";
    Meeting* meeting = app.getMeetingById(meeting_id);
    ASSERT_NE(nullptr, meeting);
    EXPECT_EQ("Weekly Meeting", meeting->toString());
    EXPECT_EQ(room3, meeting->getRoom());
    EXPECT_EQ(DateTime(2026, 5, 22, 13), meeting->getDateTime());
    EXPECT_FALSE(meeting->cateringNeeded());
    EXPECT_TRUE(meeting->externalsAllowed());
    EXPECT_FALSE(meeting->isOnline());

    //Test meeting #2
    const std::string meeting2_id = "Meeting_465831";
    Meeting* meeting2 = app.getMeetingById(meeting2_id);
    ASSERT_NE(nullptr, meeting2);
    EXPECT_EQ("Sales Report 2025", meeting2->toString());
    EXPECT_EQ("Room98732", meeting2->getRoom()->getId());
    EXPECT_EQ(DateTime(2026, 1, 3, 21), meeting2->getDateTime());
    EXPECT_FALSE(meeting2->cateringNeeded());
    EXPECT_FALSE(meeting2->externalsAllowed());
    EXPECT_FALSE(meeting2->isOnline());

    //Test participations meeting #1
    User* peter = app.getUser("Peter Selie");
    User* john = app.getUser("John Cena");
    User* arnold = app.getUser("Arnold Schwarzenegger");
    User* mick = app.getUser("Mick Turner");
    ASSERT_NE(nullptr, peter);
    ASSERT_NE(nullptr, john);
    ASSERT_NE(nullptr, arnold);
    ASSERT_NE(nullptr, mick);
    EXPECT_TRUE(peter->isProperlyInitialized());
    EXPECT_TRUE(john->isProperlyInitialized());
    EXPECT_TRUE(arnold->isProperlyInitialized());
    EXPECT_TRUE(mick->isProperlyInitialized());

    EXPECT_FALSE(peter->isExternal());
    EXPECT_TRUE(john->isExternal());
    EXPECT_TRUE(arnold->isExternal());
    EXPECT_FALSE(mick->isExternal());

    EXPECT_EQ(peter, meeting->getParticipant("Peter Selie"));
    EXPECT_EQ(john, meeting->getParticipant("John Cena"));
    EXPECT_EQ(arnold, meeting->getParticipant("Arnold Schwarzenegger"));
    EXPECT_EQ(mick, meeting2->getParticipant("Mick Turner"));

    EXPECT_EQ(meeting, peter->getMeetingById(meeting_id));
    EXPECT_EQ(meeting, john->getMeetingById(meeting_id));
    EXPECT_EQ(meeting, arnold->getMeetingById(meeting_id));
    EXPECT_EQ(meeting2, mick->getMeetingById(meeting2_id));

    EXPECT_TRUE(file_is_empty(errors));
}

TEST_F(TestParseFile, Catering)
{
    const std::string errors = "./test-files/TestParseFile.Catering-errors.txt";
    std::ofstream errLog(errors);


    App app = App(new XmlParser(), nullptr);
    ASSERT_TRUE(app.isProperlyInitialized());
    app.parseFile("./test-files/Catering.xml", errLog);

    Campus* c1 = app.getCampus("C1");
    Campus* c2 = app.getCampus("C2");
    Campus* c3 = app.getCampus("C3");
    Campus* c4 = app.getCampus("C4");
    ASSERT_NE(nullptr, c1);
    ASSERT_NE(nullptr, c2);
    ASSERT_NE(nullptr, c3);
    ASSERT_NE(nullptr, c4);

    ASSERT_FALSE(c1->getCaterings().empty());
    ASSERT_FALSE(c2->getCaterings().empty());
    ASSERT_FALSE(c3->getCaterings().empty());
    EXPECT_TRUE(c4->getCaterings().empty());

    Catering* cat1 = c1->getCaterings().front();
    Catering* cat2 = c2->getCaterings().front();
    Catering* cat3 = c3->getCaterings().front();
    ASSERT_NE(nullptr, cat1);
    ASSERT_NE(nullptr, cat2);
    ASSERT_NE(nullptr, cat3);

    EXPECT_EQ(c1, cat1->getCampus());
    EXPECT_EQ(c2, cat2->getCampus());
    EXPECT_EQ(c3, cat3->getCampus());

    EXPECT_EQ(100.0, cat1->getEmissions());
    EXPECT_EQ(200.0, cat2->getEmissions());
    EXPECT_EQ(300.0, cat3->getEmissions());

    EXPECT_TRUE(file_is_empty(errors));
}

TEST_F(TestParseFile, InvalidData1)
{
    const std::string actual = "./test-files/TestParseFile.InvalidData1-errors-actual.txt";
    const std::string expected = "./test-files/TestParseFile.InvalidData1-errors-expected.txt";

    std::ofstream errLog(actual);

    App app = App(new XmlParser(errLog), nullptr);

    ASSERT_TRUE(app.isProperlyInitialized());
    app.parseFile("./test-files/InvalidData1.xml", errLog);

    // Correctly parsed
    Campus* c = app.getCampus("c");
    ASSERT_NE(nullptr, c);
    EXPECT_EQ("campus", c->toString());

    Building* b = app.getBuilding("b");
    ASSERT_NE(nullptr, b);
    EXPECT_EQ("building", b->toString());
    EXPECT_EQ(c, b->getCampus());


    Room* room = app.getRoom("mg025");
    ASSERT_NE(nullptr, room);
    EXPECT_EQ("M.G.025", room->toString());
    EXPECT_EQ(55u, room->getCapacity());
    EXPECT_EQ(b, room->getBuilding());
    EXPECT_EQ(c, room->getCampus());


    // Failed
    EXPECT_EQ(nullptr, app.getRoom("Room98732"));
    EXPECT_EQ(nullptr, app.getMeetingById("Meeting_478463"));
    EXPECT_EQ(nullptr, app.getMeetingById("Meeting_514203"));
    EXPECT_EQ(nullptr, app.getUser("Peter Selie"));

    //Test errors
    ASSERT_TRUE(file_exists(actual));
    ASSERT_TRUE(file_exists(expected));
    EXPECT_TRUE(file_compare(actual, expected));
}

TEST_F(TestParseFile, InvalidData2)
{
    const std::string actual = "./test-files/TestParseFile.InvalidData2-errors-actual.txt";
    const std::string expected = "./test-files/TestParseFile.InvalidData2-errors-expected.txt";

    std::ofstream errLog(actual);

    const std::string user1 = "Peter Selie";
    const std::string user2 = "Freddy Gonzalez";


    App app = App(new XmlParser(errLog), nullptr);
    EXPECT_TRUE(app.isProperlyInitialized());
    app.parseFile("./test-files/InvalidData2.xml", errLog);

    // Campus
    Campus* c = app.getCampus("c");
    EXPECT_NE(nullptr, c);
    Building* b = app.getBuilding("b");
    EXPECT_NE(nullptr, b);

    // Test room #1
    Room* r1 = app.getRoom("mg025");
    ASSERT_NE(nullptr, r1);
    EXPECT_EQ("M.G.025", r1->toString());
    EXPECT_EQ(25u, r1->getCapacity());
    EXPECT_EQ(b, r1->getBuilding());
    EXPECT_EQ(c, r1->getCampus());

    // Test room #2
    Room* r2 = app.getRoom("gt123");
    ASSERT_NE(nullptr, r2);
    EXPECT_EQ("G.T.123", r2->toString());
    EXPECT_EQ(100u, r2->getCapacity());
    EXPECT_EQ(b, r1->getBuilding());
    EXPECT_EQ(c, r1->getCampus());

    // No other rooms
    EXPECT_EQ(2u, app.getAllRooms().size());

    //Test meeting #1
    Meeting* m1 = app.getMeetingById("m656");
    ASSERT_NE(nullptr, m1);
    EXPECT_EQ("Movie Marathon", m1->toString());
    EXPECT_EQ(r1, m1->getRoom());
    EXPECT_EQ(DateTime(2026, 6, 13, 12), m1->getDateTime());

    //Test meeting #2
    Meeting* m2 = app.getMeetingById("m720");
    ASSERT_NE(nullptr, m2);
    EXPECT_EQ("Very important meeting", m2->toString());
    EXPECT_EQ(r2, m2->getRoom());
    EXPECT_EQ(DateTime(2026, 6, 13, 12), m2->getDateTime());

    //Test meeting #3
    Meeting* m3 = app.getMeetingById("m999");
    EXPECT_EQ(nullptr, m3);

    // No other meetings
    EXPECT_EQ(size_t(2), app.getMeetingRegistry().getRawIdMap().size());

    //Test errors
    ASSERT_TRUE(file_exists(actual));
    ASSERT_TRUE(file_exists(expected));
    EXPECT_TRUE(file_compare(actual, expected));
}

TEST_F(TestParseFile, InvalidDataUsers)
{
    const std::string actual = "./test-files/TestParseFile.InvalidDataUsers-errors-actual.txt";
    const std::string expected = "./test-files/TestParseFile.InvalidDataUsers-errors-expected.txt";

    std::ofstream errLog(actual);


    App app = App(new XmlParser(errLog), nullptr);
    ASSERT_TRUE(app.isProperlyInitialized());
    app.parseFile("./test-files/InvalidDataUsers.xml", errLog);

    EXPECT_NE(nullptr, app.getCampus("c"));
    EXPECT_NE(nullptr, app.getBuilding("b"));
    EXPECT_NE(nullptr, app.getRoom("r"));

    Meeting* m1 = app.getMeetingById("m1");
    Meeting* m2 = app.getMeetingById("m2");

    ASSERT_NE(nullptr, m1);
    ASSERT_NE(nullptr, m2);

    EXPECT_NE(nullptr, m1->getParticipant("Peter Selie"));

    EXPECT_EQ(nullptr, m2->getParticipant("Peter Selie"));

    ASSERT_TRUE(file_exists(actual));
    ASSERT_TRUE(file_exists(expected));
    EXPECT_TRUE(file_compare(actual, expected));
}

TEST_F(TestParseFile, InvalidDataLayout)
{
    const std::string actual = "./test-files/TestParseFile.InvalidDataLayout-errors-actual.txt";
    const std::string expected = "./test-files/TestParseFile.InvalidDataLayout-errors-expected.txt";

    std::ofstream errLog(actual);

    App app = App(new XmlParser(errLog), nullptr);
    ASSERT_TRUE(app.isProperlyInitialized());
    app.parseFile("./test-files/InvalidDataLayout.xml", errLog);

    // It shouldnt have added invalid elements
    EXPECT_EQ(nullptr, app.getCampus("invalid-campus"));
    EXPECT_EQ(nullptr, app.getCampus("invalid-building"));
    EXPECT_EQ(nullptr, app.getCampus("invalid-room"));

    // Make sure valid elements were properly added
    Campus* c1 = app.getCampus("campus_1");
    Campus* c2 = app.getCampus("campus_2");
    Building* b1 = app.getBuilding("building_1");
    Building* b2 = app.getBuilding("building_2");
    ASSERT_NE(nullptr, c1);
    ASSERT_NE(nullptr, c2);
    ASSERT_TRUE(c1->isProperlyInitialized());
    ASSERT_TRUE(c2->isProperlyInitialized());
    ASSERT_NE(nullptr, b1);
    ASSERT_NE(nullptr, b2);
    ASSERT_TRUE(b1->isProperlyInitialized());
    ASSERT_TRUE(b2->isProperlyInitialized());
    ASSERT_EQ(c1, b1->getCampus());
    ASSERT_EQ(c2, b2->getCampus());

    // It shouldnt have added mismatched rooms
    EXPECT_EQ(nullptr, app.getRoom("r1"));
    EXPECT_EQ(nullptr, app.getRoom("r2"));

    // It should have added dup-room, but *only once*
    // It is added first to building_1
    Room* dup = app.getRoom("dup-room");
    ASSERT_NE(nullptr, dup);
    EXPECT_EQ(b1, dup->getBuilding());

    // It shouldn't duplicate or modify building_1
    EXPECT_EQ(b1, app.getBuilding("building_1"));
    EXPECT_EQ(c1, b1->getCampus());

    ASSERT_TRUE(file_exists(actual));
    ASSERT_TRUE(file_exists(expected));
    EXPECT_TRUE(file_compare(actual, expected));
}

TEST_F(TestParseFile, InvalidXml)
{
    App app = App(new XmlParser(), nullptr);
    EXPECT_TRUE(app.isProperlyInitialized());
    EXPECT_DEATH(app.parseFile("./test-files/InvalidXml.xml"), "");
}

TEST_F(TestParseFile, FileNotFound)
{
    App app = App(new XmlParser(), nullptr);
    ASSERT_TRUE(app.isProperlyInitialized());
    EXPECT_DEATH(app.parseFile("./test-files/FileThatDoesntExist.xml"), "");
}

TEST_F(TestParseFile, Renovations)
{
    const std::string error = "./test-files/TestParseFile.HappyDay1-errors.txt";

    std::ofstream errLog(error);

    App app = App(new XmlParser(errLog), nullptr);
}

//TODO: Add duplicate attribute scenario


#endif //MEETING_PLANNER_PARSEFILETESTS_H