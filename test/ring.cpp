/** @file `ring.cpp`
 *  @brief 
 */

#include <gtest/gtest.h>

#include "utility/ring.h"
#include "utility/errors.h"


TEST(ringCreate, ringTests) {
    ring_t ring;
    ASSERT_EQ(0, ring_create(&ring, 4, sizeof(float32_t)));
    ASSERT_EQ(0, ring_destroy(&ring));
}


TEST(ringCreateNull, ringTests) {
    ASSERT_NE(0, ring_create(nullptr, 4, sizeof(float32_t) * 4));
    ASSERT_NE(0, ring_destroy(nullptr));
}


TEST(ringQueue, ringTests) {
    ring_t ring;
    float32_t datum = 1.f;

    ASSERT_EQ(0, ring_create(&ring, 4, sizeof(float32_t)));
    ASSERT_EQ(0, ring_enqueue(&ring, (uint8_t *)&datum));
    ASSERT_EQ(0, ring_destroy(&ring));
}


TEST(ringQueueDeque, ringTests) {
    ring_t ring;
    float32_t datum = 1.f;

    ASSERT_EQ(0, ring_create(&ring, 4, sizeof(float32_t)));
    ASSERT_EQ(0, ring_enqueue(&ring, (uint8_t *)&datum));
    ASSERT_EQ((int8_t *)&datum, ring_dequeue(&ring));
    ASSERT_EQ(0, ring_destroy(&ring));    
}


TEST(ringUpperBoundary, ringTests) {
    ring_t ring;
    float32_t datum = 1.f;

    ASSERT_EQ(0, ring_create(&ring, 4, sizeof(float32_t)));
    ASSERT_EQ(0, ring_enqueue(&ring, (uint8_t *)&datum));
    ASSERT_EQ((int8_t*)&datum, ring_dequeue(&ring));
    ASSERT_EQ(0, ring_destroy(&ring));
}


TEST(ringLowerBoundary, ringTests) {
    ring_t ring;
    float32_t datum = 1.f;

    ASSERT_EQ(0, ring_create(&ring, 4, sizeof(float32_t)));
    ASSERT_EQ(0, ring_enqueue(&ring, (uint8_t *)&datum));
    ASSERT_EQ((int8_t *)&datum, ring_dequeue(&ring));
    ASSERT_EQ(nullptr, ring_dequeue(&ring));
    ASSERT_EQ(0, ring_destroy(&ring));
}
