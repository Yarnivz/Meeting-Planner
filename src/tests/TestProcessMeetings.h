//
// Created by lucas on 2026-03-15.
//

#ifndef MEETING_PLANNER_PROCESSMEETINGSTESTS_H
#define MEETING_PLANNER_PROCESSMEETINGSTESTS_H

#include "gtest/gtest.h"
#include "../App.h"

class ProcessMeetingsTest : public ::testing::Test {
protected:
};


TEST_F(ProcessMeetingsTest, HappyDay) {
    App p = App(nullptr, nullptr);
    EXPECT_TRUE(p.isProperlyInitialized());

    p.addRoom(new Room("M.G.025", "MG025", 40));
    p.addRoom(new Room("G.T.103", "GT103", 100));

    Meeting* m1 = new Meeting("Important Meeting", "M1", "MG025", Date(2026, 1, 1));
    Meeting* m2 = new Meeting("Important Meeting 2", "M2", "GT103", Date(2026, 1, 1));
    Meeting* m3 = new Meeting("Conflict", "M3", "MG025", Date(2026, 1, 1));
    Meeting* m4 = new Meeting("Conflict 2", "M4", "GT103", Date(2026,1,1));
    p.addMeeting(m1);
    p.addMeeting(m2);
    p.addMeeting(m3);
    p.addMeeting(m4);

    p.processSingleMeeting("M1", false);
    EXPECT_EQ(m1     , p.getDoneMeeting("M1"));
    EXPECT_EQ(nullptr, p.getDoneMeeting("M2"));
    EXPECT_EQ(nullptr, p.getDoneMeeting("M3"));
    EXPECT_EQ(nullptr, p.getDoneMeeting("M4"));
    EXPECT_EQ(nullptr, p.getCanceledMeeting("M1"));
    EXPECT_EQ(nullptr, p.getCanceledMeeting("M2"));
    EXPECT_EQ(nullptr, p.getCanceledMeeting("M3"));
    EXPECT_EQ(nullptr, p.getCanceledMeeting("M4"));

    p.processSingleMeeting("M2", false);
    EXPECT_EQ(m1     , p.getDoneMeeting("M1"));
    EXPECT_EQ(m2     , p.getDoneMeeting("M2"));
    EXPECT_EQ(nullptr, p.getDoneMeeting("M3"));
    EXPECT_EQ(nullptr, p.getDoneMeeting("M4"));
    EXPECT_EQ(nullptr, p.getCanceledMeeting("M1"));
    EXPECT_EQ(nullptr, p.getCanceledMeeting("M2"));
    EXPECT_EQ(nullptr, p.getCanceledMeeting("M3"));
    EXPECT_EQ(nullptr, p.getCanceledMeeting("M4"));

    p.processSingleMeeting("M3", false);
    EXPECT_EQ(m1     , p.getDoneMeeting("M1"));
    EXPECT_EQ(m2     , p.getDoneMeeting("M2"));
    EXPECT_EQ(nullptr, p.getDoneMeeting("M3"));
    EXPECT_EQ(nullptr, p.getDoneMeeting("M4"));
    EXPECT_EQ(nullptr, p.getCanceledMeeting("M1"));
    EXPECT_EQ(nullptr, p.getCanceledMeeting("M2"));
    EXPECT_EQ(m3     , p.getCanceledMeeting("M3"));
    EXPECT_EQ(nullptr, p.getCanceledMeeting("M4"));
    EXPECT_EQ("conflict with meeting M1", p.getCancellationReason("M3"));

    p.processSingleMeeting("M4", false);
    EXPECT_EQ(m1     , p.getDoneMeeting("M1"));
    EXPECT_EQ(m2     , p.getDoneMeeting("M2"));
    EXPECT_EQ(nullptr, p.getDoneMeeting("M3"));
    EXPECT_EQ(nullptr, p.getDoneMeeting("M4"));
    EXPECT_EQ(nullptr, p.getCanceledMeeting("M1"));
    EXPECT_EQ(nullptr, p.getCanceledMeeting("M2"));
    EXPECT_EQ(m3     , p.getCanceledMeeting("M3"));
    EXPECT_EQ(m4     , p.getCanceledMeeting("M4"));
    EXPECT_EQ("conflict with meeting M2", p.getCancellationReason("M4"));
}

TEST_F(ProcessMeetingsTest, Conflicts) {
    App p = App(nullptr, nullptr);
    EXPECT_TRUE(p.isProperlyInitialized());

    const std::string r1 = "MG025";
    const std::string r2 = "GT103";

    p.addRoom(new Room("M.G.025", r1, 40));
    p.addRoom(new Room("G.T.103", r2, 100));

    const Date d1 = Date(2026, 1, 1);
    const Date d2 = Date(2026, 3, 3);

    std::vector<Meeting*> l1 = {};
    std::vector<Meeting*> l2 = {};
    std::vector<Meeting*> l3 = {};
    std::vector<Meeting*> l4 = {};

    for (size_t i = 0; i < 30; ++i) {
        std::string id = "A"+ std::to_string(i);
        l1.push_back(new Meeting("Meeting "+id, id, r1, d1));

    }
    for (size_t i = 0; i < 30; ++i) {
        std::string id = "B"+ std::to_string(i);
        l2.push_back(new Meeting("Meeting "+id, id, r2, d1));
    }
    for (size_t i = 0; i < 30; ++i) {
        std::string id = "C"+ std::to_string(i);
        l3.push_back(new Meeting("Meeting "+id, id, r2, d2));
    }
    for (size_t i = 0; i < 30; ++i) {
        std::string id = "D"+ std::to_string(i);
        l4.push_back(new Meeting("Meeting "+id, id, r1, d2));
    }

    for (size_t i = 0; i < 30; ++i) {
        p.addMeeting(l1[i]);
        p.processSingleMeeting(l1[i]->getId(), false);
        p.addMeeting(l2[i]);
        p.processSingleMeeting(l2[i]->getId(), false);
        p.addMeeting(l3[i]);
        p.processSingleMeeting(l3[i]->getId(), false);
        p.addMeeting(l4[i]);
        p.processSingleMeeting(l4[i]->getId(), false);
    }

    EXPECT_EQ(l1[0], p.getDoneMeeting(l1[0]->getId()));
    EXPECT_EQ(l2[0], p.getDoneMeeting(l2[0]->getId()));
    EXPECT_EQ(l3[0], p.getDoneMeeting(l3[0]->getId()));
    EXPECT_EQ(l4[0], p.getDoneMeeting(l4[0]->getId()));
    EXPECT_EQ(nullptr, p.getCanceledMeeting(l1[0]->getId()));
    EXPECT_EQ(nullptr, p.getCanceledMeeting(l2[0]->getId()));
    EXPECT_EQ(nullptr, p.getCanceledMeeting(l3[0]->getId()));
    EXPECT_EQ(nullptr, p.getCanceledMeeting(l4[0]->getId()));

    for (size_t i = 1; i < 30; ++i) {
        EXPECT_EQ(nullptr, p.getDoneMeeting(l1[i]->getId()));
        EXPECT_EQ(nullptr, p.getDoneMeeting(l2[i]->getId()));
        EXPECT_EQ(nullptr, p.getDoneMeeting(l3[i]->getId()));
        EXPECT_EQ(nullptr, p.getDoneMeeting(l4[i]->getId()));
        EXPECT_EQ(l1[i], p.getCanceledMeeting(l1[i]->getId()));
        EXPECT_EQ(l2[i], p.getCanceledMeeting(l2[i]->getId()));
        EXPECT_EQ(l3[i], p.getCanceledMeeting(l3[i]->getId()));
        EXPECT_EQ(l4[i], p.getCanceledMeeting(l4[i]->getId()));

        static const std::string creason1 = "conflict with meeting " + l1[0]->getId();
        static const std::string creason2 = "conflict with meeting " + l2[0]->getId();
        static const std::string creason3 = "conflict with meeting " + l3[0]->getId();
        static const std::string creason4 = "conflict with meeting " + l4[0]->getId();

        EXPECT_EQ(creason1, p.getCancellationReason(l1[i]->getId()));
        EXPECT_EQ(creason2, p.getCancellationReason(l2[i]->getId()));
        EXPECT_EQ(creason3, p.getCancellationReason(l3[i]->getId()));
        EXPECT_EQ(creason4, p.getCancellationReason(l4[i]->getId()));
    }

}

TEST_F(ProcessMeetingsTest, Order) {
    App p = App(nullptr, nullptr);
    EXPECT_TRUE(p.isProperlyInitialized());

    p.addRoom(new Room("M.G.025", "MG025", 40));

    Meeting* m1 = new Meeting("Important Meeting", "M1", "MG025", Date(2026, 1, 1));
    Meeting* m2 = new Meeting("Importanter Meeting", "M2", "MG025", Date(2026, 1, 1));
    Meeting* m3 = new Meeting("Importantest Meeting", "M3", "MG025", Date(2026, 1, 1));
    m1->setOrder(100);
    m2->setOrder(0);
    m3->setOrder(-2);

    p.addMeeting(m3);
    p.addMeeting(m1);
    p.addMeeting(m2);

    p.processAllMeetings(false);
    EXPECT_EQ(nullptr, p.getDoneMeeting("M1"));
    EXPECT_EQ(nullptr, p.getDoneMeeting("M2"));
    EXPECT_EQ(m3     , p.getDoneMeeting("M3"));

    EXPECT_EQ(m1     , p.getCanceledMeeting("M1"));
    EXPECT_EQ(m2     , p.getCanceledMeeting("M2"));
    EXPECT_EQ(nullptr, p.getCanceledMeeting("M3"));

    EXPECT_EQ("conflict with meeting M3", p.getCancellationReason("M1"));
    EXPECT_EQ("conflict with meeting M3", p.getCancellationReason("M2"));
}

TEST_F(ProcessMeetingsTest, ParseOrder) {
    App p = App(new XmlParser(), nullptr);
    EXPECT_TRUE(p.isProperlyInitialized());

    p.parseFile("./test-files/ConflictMeetings2.xml");

    p.processAllMeetings(false);

    EXPECT_NE(nullptr, p.getMeeting("A1"));
    EXPECT_NE(nullptr, p.getMeeting("A2"));
    EXPECT_NE(nullptr, p.getMeeting("A3"));
    EXPECT_NE(nullptr, p.getMeeting("B1"));
    EXPECT_NE(nullptr, p.getMeeting("B2"));
    EXPECT_NE(nullptr, p.getMeeting("B3"));

    EXPECT_EQ(p.getMeeting("A1"), p.getDoneMeeting("A1"));
    EXPECT_EQ(p.getMeeting("B1"), p.getDoneMeeting("B1"));
    EXPECT_EQ(nullptr, p.getDoneMeeting("A2"));
    EXPECT_EQ(nullptr, p.getDoneMeeting("B2"));
    EXPECT_EQ(nullptr, p.getDoneMeeting("A3"));
    EXPECT_EQ(nullptr, p.getDoneMeeting("B3"));

    EXPECT_EQ(nullptr, p.getCanceledMeeting("A1"));
    EXPECT_EQ(nullptr, p.getCanceledMeeting("B1"));
    EXPECT_EQ(p.getMeeting("A2"), p.getCanceledMeeting("A2"));
    EXPECT_EQ(p.getMeeting("B2"), p.getCanceledMeeting("B2"));
    EXPECT_EQ(p.getMeeting("A3"), p.getCanceledMeeting("A3"));
    EXPECT_EQ(p.getMeeting("B3"), p.getCanceledMeeting("B3"));

    EXPECT_EQ("conflict with meeting A1", p.getCancellationReason("A2"));
    EXPECT_EQ("conflict with meeting A1", p.getCancellationReason("A3"));
    EXPECT_EQ("conflict with meeting B1", p.getCancellationReason("B2"));
    EXPECT_EQ("conflict with meeting B1", p.getCancellationReason("B3"));
}

TEST_F(ProcessMeetingsTest, ContractViolation) {
    App p = App(nullptr, nullptr);

    p.addRoom(new Room("r", "r", 20));

    p.addMeeting(new Meeting("m", "m", "r", Date(2026,1,1)));

    //double use
    EXPECT_NO_FATAL_FAILURE(p.processSingleMeeting("m", false));
    EXPECT_DEATH(p.processSingleMeeting("m", false), "");

    //same for processAllMeetings
    EXPECT_DEATH(p.processAllMeetings(false), "");

    //process fake meeting
    EXPECT_DEATH(p.processSingleMeeting("doesnt-exist", false), "");
}

#endif //MEETING_PLANNER_PROCESSMEETINGSTESTS_H