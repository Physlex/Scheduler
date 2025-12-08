#include <gtest/gtest.h>

#include "utility/ring.h"


TEST(createRing, ringTests) {
    ring_t ring;
    ASSERT_EQ(0, ring_create(&ring, 4, sizeof(float32_t)));
    ASSERT_EQ(0, ring_destroy(&ring));
}


TEST(createNullRing, ringTests) {
    ASSERT_NE(0, ring_create(nullptr, 4, sizeof(float32_t) * 4));
    ASSERT_NE(0, ring_destroy(nullptr));
}
