//
// Created by Yarni on 3/13/2026.
//

#ifndef MEETING_PLANNER_PARSEFILETESTS_H
#define MEETING_PLANNER_PARSEFILETESTS_H
#include <gtest/gtest.h>

#include "App.h"

class ParseFileTests : public ::testing::Test {
};

TEST_F(ParseFileTests, HappyDay) {
    // HappyDay Testfiles need to be added here
    std::string testfiles[] = {"HappyDay1.xml", "HappyDay2.xml"};

    for (const std::string& file : testfiles) {
        App* app = new App();
        EXPECT_TRUE(app->isProperlyInitialized());
        app->parseFile("./test-files/" + file);

        //Test cases HappyDay1
        if (file == "HappyDay1.xml")  {
            //Test room #1
            Room* room = app->getRoom("M.G.025");
            EXPECT_EQ(room->toString(), "M.G.025");
            EXPECT_EQ(room->getCapacity(), 55u);

            //Test room #2
            room = app->getRoom("Room98732");
            EXPECT_EQ(room->toString(), "M.G.023");
            EXPECT_EQ(room->getCapacity(), 10u);

            //Test meeting
            Meeting* meeting = app->getMeeting("Meeting_478463");
            EXPECT_EQ(meeting->toString(), "Weekly meeting");
            EXPECT_EQ(meeting->getRoomId(), "Room98732");
            EXPECT_EQ(meeting->getDate(), Date(2026,5,22));

            //Test participation
            const Participations* participationsByMeeting = app->getParticipationsByMeeting("Meeting_478463");
            const Participations* participationsByUser = app->getParticipationsByUser("Peter Selie");
            EXPECT_EQ(participationsByMeeting->size(), 1u);
            EXPECT_EQ(participationsByUser->size(), 1u);

        }
        delete app;

    }

}


#endif //MEETING_PLANNER_PARSEFILETESTS_H