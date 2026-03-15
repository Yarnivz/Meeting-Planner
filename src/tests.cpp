#include <gtest/gtest.h>
#include "DateTests.h"
#include "MeetingTests.h"
#include "ParticipationTests.h"
#include "RoomTests.h"
#include "ParseFileTests.h"
#include "WriteToStreamTest.h"
#include "AppTests.h"
#include "ProcessMeetingsTests.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}