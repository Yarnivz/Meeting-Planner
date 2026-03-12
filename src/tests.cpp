#include <gtest/gtest.h>
#include "DateTest.h"
#include "MeetingTest.h"
#include "ParticipationTest.h"
#include "RoomTest.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}