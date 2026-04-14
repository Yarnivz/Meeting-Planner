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
    std::string meeting_id = "Meeting_478463";


    XmlParser* xml_parser = new XmlParser();
    App app = App(xml_parser, nullptr);
    EXPECT_TRUE(app.isProperlyInitialized());

    app.parseFile("./test-files/HappyDay1.xml", errLog);

    //Test room #1
    Room* room = app.getRoom("M.G.025");
    ASSERT_NE(nullptr, room);
    EXPECT_EQ("M.G.025", room->toString());
    EXPECT_EQ(55u, room->getCapacity());

    //Test room #2
    room = app.getRoom("Room98732");
    ASSERT_NE(nullptr, room);
    EXPECT_EQ("M.G.023", room->toString());
    EXPECT_EQ(10u, room->getCapacity());

    //Test meeting
    Meeting* meeting = app.getMeetingById(meeting_id);
    ASSERT_NE(nullptr, meeting);
    EXPECT_EQ("Weekly meeting", meeting->toString());
    EXPECT_EQ("Room98732", meeting->getRoom()->getId());
    EXPECT_EQ(DateTime(2026, 5, 22, 13), meeting->getDateTime());

    //Test participation
    const User* peter = app.getUser("Peter Selie");
    ASSERT_NE(nullptr, peter);
    EXPECT_EQ("Peter Selie", peter->getId());

    EXPECT_TRUE(file_is_empty(error));
}


TEST_F(TestParseFile, HappyDay2)
{
    const std::string errors = "./test-files/TestParseFile.HappyDay2-errors.txt";
    std::ofstream errLog(errors);
    std::string meeting1 = "Meeting_478463";
    std::string meeting2 = "Meeting_465831";


    App app = App(new XmlParser(), &std::cout);

    EXPECT_TRUE(app.isProperlyInitialized());
    app.parseFile("./test-files/HappyDay2.xml", errLog);

    //Test room #1
    Room* room = app.getRoom("id_026");
    ASSERT_NE(nullptr, room);
    EXPECT_EQ(room->toString(), "M.G.026");
    EXPECT_EQ(room->getCapacity(), 30u);

    //Test room #2
    room = app.getRoom("Room98732");
    ASSERT_NE(nullptr, room);
    EXPECT_EQ("M.G.023", room->toString());
    EXPECT_EQ(10u, room->getCapacity());

    //Test room #3
    room = app.getRoom("Aula23");
    ASSERT_NE(nullptr, room);
    EXPECT_EQ("M.A.043", room->toString());
    EXPECT_EQ(46u, room->getCapacity());

    //Test meeting #1
    Meeting* meeting = app.getMeetingById(meeting1);
    ASSERT_NE(nullptr, meeting);
    EXPECT_EQ("Weekly Meeting", meeting->toString());
    ASSERT_NE(nullptr, meeting->getRoom());
    EXPECT_EQ("Aula23", meeting->getRoom()->getId());
    EXPECT_EQ(DateTime(2026, 5, 22, 13), meeting->getDateTime());

    //Test meeting #2
    meeting = app.getMeetingById(meeting2);
    ASSERT_NE(nullptr, meeting);
    EXPECT_EQ("Sales Report 2025", meeting->toString());
    EXPECT_EQ("Room98732", meeting->getRoom()->getId());
    EXPECT_EQ(DateTime(2026, 1, 3, 21), meeting->getDateTime());

    //Test participations meeting #1
    ASSERT_NE(nullptr, app.getUser("Peter Selie"));
    ASSERT_NE(nullptr, app.getUser("John Cena"));
    ASSERT_NE(nullptr, app.getUser("Arnold Schwarzenegger"));
    ASSERT_NE(nullptr, app.getUser("Mick Turner"));

    EXPECT_NE(nullptr, app.getMeetingById("Meeting_478463")->getParticipant("Peter Selie"));
    EXPECT_NE(nullptr, app.getMeetingById("Meeting_478463")->getParticipant("John Cena"));
    EXPECT_NE(nullptr, app.getMeetingById("Meeting_478463")->getParticipant("Arnold Schwarzenegger"));
    EXPECT_NE(nullptr, app.getMeetingById("Meeting_465831")->getParticipant("Mick Turner"));

    EXPECT_TRUE(file_is_empty(errors));
}

TEST_F(TestParseFile, InvalidData1)
{
    const std::string actual = "./test-files/TestParseFile.InvalidData1-errors-actual.txt";
    const std::string expected = "./test-files/TestParseFile.InvalidData1-errors-expected.txt";

    std::ofstream errLog(actual);

    const std::string meeting1 = "Meeting_478463";
    const std::string meeting2 = "Meeting_514203";
    const std::string user = "Peter Selie";


    App app = App(new XmlParser(), nullptr);
    ASSERT_TRUE(app.isProperlyInitialized());
    app.parseFile("./test-files/InvalidData1.xml", errLog);

    //Test room #1
    Room* room = app.getRoom("M.G.025");
    ASSERT_NE(nullptr, room);
    EXPECT_EQ("M.G.025", room->toString());
    EXPECT_EQ(55u, room->getCapacity());

    //Test room #2
    room = app.getRoom("Room98732");
    EXPECT_EQ(nullptr, room);

    //Test meeting #1
    Meeting* meeting = app.getMeetingById(meeting1);
    EXPECT_EQ(nullptr, meeting);

    //Test meeting #2
    meeting = app.getMeetingById(meeting2);
    EXPECT_EQ(nullptr, meeting);

    //Test participation
    User* p = app.getUser(user);
    EXPECT_EQ(nullptr, p);

    //Test errors
    EXPECT_TRUE(file_compare(actual, expected));
}

TEST_F(TestParseFile, InvalidData2)
{
    std::string actual = "./test-files/TestParseFile.InvalidData2-errors-actual.txt";
    std::string expected = "./test-files/TestParseFile.InvalidData2-errors-expected.txt";

    std::ofstream errLog(actual);

    std::string room1 = "mg025";
    std::string room2 = "gt123";
    std::string meeting1 = "m656";
    std::string meeting2 = "m720";
    std::string meeting3 = "m999";
    std::string user1 = "Peter Selie";
    std::string user2 = "Freddy Gonzalez";


    App app = App(new XmlParser(), nullptr);
    EXPECT_TRUE(app.isProperlyInitialized());
    app.parseFile("./test-files/InvalidData2.xml", errLog);

    //Test room #1
    Room* room = app.getRoom(room1);
    ASSERT_NE(nullptr, room);
    EXPECT_EQ("M.G.025", room->toString());
    EXPECT_EQ(25u, room->getCapacity());

    //Test room #2
    room = app.getRoom(room2);
    ASSERT_NE(nullptr, room);
    EXPECT_EQ("G.T.123", room->toString());
    EXPECT_EQ(100u, room->getCapacity());

    //Test room #3
    EXPECT_EQ(2u, app.getAllRooms().size());

    //Test meeting #1
    Meeting* meeting = app.getMeetingById(meeting1);
    ASSERT_NE(nullptr, meeting);
    EXPECT_EQ("Movie Marathon", meeting->toString());
    EXPECT_EQ(room1, meeting->getRoom()->getId());
    EXPECT_EQ(DateTime(2026, 6, 13, 12), meeting->getDateTime());

    //Test meeting #2
    meeting = app.getMeetingById(meeting2);
    ASSERT_NE(nullptr, meeting);
    EXPECT_EQ("Very important meeting", meeting->toString());
    EXPECT_EQ(room2, meeting->getRoom()->getId());
    EXPECT_EQ(DateTime(2026, 6, 13, 12), meeting->getDateTime());

    //Test meeting #3
    meeting = app.getMeetingById(meeting3);
    EXPECT_EQ(nullptr, meeting);
    EXPECT_EQ(size_t(2), app.getMeetingRegistry().getRawIdMap().size());

    //Test errors
    ASSERT_TRUE(file_exists(actual));
    ASSERT_TRUE(file_exists(expected));
    EXPECT_TRUE(file_compare(actual, expected));
}

TEST_F(TestParseFile, InvalidXml)
{
    App app = App(new XmlParser(), &std::cout);
    EXPECT_TRUE(app.isProperlyInitialized());
    EXPECT_DEATH(app.parseFile("./test-files/InvalidXml.xml"), "");
}

TEST_F(TestParseFile, FileNotFound)
{
    App app = App(new XmlParser(), &std::cout);
    ASSERT_TRUE(app.isProperlyInitialized());
    EXPECT_DEATH(app.parseFile("./test-files/FileThatDoesntExist.xml"), "");
}

//TODO: Add duplicate attribute scenario


#endif //MEETING_PLANNER_PARSEFILETESTS_H