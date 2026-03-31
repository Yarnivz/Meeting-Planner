#include <gtest/gtest.h>
#include "tests/TestDate.h"
#include "tests/TestMeeting.h"
#include "tests/TestParticipation.h"
#include "tests/TestRoom.h"
#include "tests/TestParseFile.h"
#include "tests/TestWriteToStream.h"
#include "tests/TestApp.h"
#include "tests/TestProcessMeetings.h"

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
