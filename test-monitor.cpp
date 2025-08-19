#include <gtest/gtest.h>
#include "./monitor.h"

TEST(Monitor, NotOkWhenAnyVitalIsOffRange) {
    // Critical Cases
    // High pulse rate
    ASSERT_FALSE(vitalsOk(98.1, 102, 98));
    // Low pulse rate
    ASSERT_FALSE(vitalsOk(98.1, 58, 98));
    // High temperature
    ASSERT_FALSE(vitalsOk(103, 70, 98));
    // Low temperature
    ASSERT_FALSE(vitalsOk(94, 70, 98));
    // Low oxygen saturation
    ASSERT_FALSE(vitalsOk(98.1, 70, 89));

    // Warning Cases
    // High temperature warning
    ASSERT_TRUE(vitalsOk(101.5, 70, 98));
    // Low temperature warning
    ASSERT_TRUE(vitalsOk(95.5, 70, 98));
    // High pulse rate warning
    ASSERT_TRUE(vitalsOk(98.1, 99, 98));
    // Low pulse rate warning
    ASSERT_TRUE(vitalsOk(98.1, 61, 98));
    // Low oxygen saturation warning
    ASSERT_TRUE(vitalsOk(98.1, 70, 91));

    // OK Case
    ASSERT_TRUE(vitalsOk(98.1, 70, 98));
}
