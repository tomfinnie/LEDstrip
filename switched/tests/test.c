#include "../ledlib.h"
#include "TESTickle.h"
#include <stdlib.h>

void setup() { }
void teardown() { }

TEST(upper_limit_over)
    ASSERT_EQUAL(calcBatteryPercentage(861), 100, "%d", "Readings >=861 are 100%");
END_TEST

TEST(upper_limit_at)
    ASSERT_EQUAL(calcBatteryPercentage(860), 100, "%d", "Readings >=861 are 100%");
END_TEST