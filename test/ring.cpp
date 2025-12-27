/** @file `ring.cpp`
 *  @brief 
 */

#include <gtest/gtest.h>

#include "container/ring.h"
#include "utility/errors.h"


TEST(ringCreate, ringTests) {
    ring_t ring;

    const uint32_t ring_size = sizeof(float32_t);
    const uint32_t ring_len = 4;

    ASSERT_EQ(0, ring_create(&ring, ring_size, ring_len));
    ASSERT_EQ(0, ring_destroy(&ring));
}


TEST(ringCreateNull, ringTests) {
    const uint32_t ring_size = sizeof(float32_t);
    const uint32_t ring_len = 4;

    ASSERT_NE(0, ring_create(nullptr, ring_size, ring_len));
    ASSERT_NE(0, ring_destroy(nullptr));
}


TEST(ringQueue, ringTests) {
    ring_t ring;

    const uint32_t ring_size = sizeof(float32_t);
    const uint32_t ring_len = 4;
    const uint32_t datum = 1;

    ASSERT_EQ(0, ring_create(&ring, ring_size, ring_len));
    ASSERT_EQ(0, ring_enqueue(&ring, (void *)&datum));
    ASSERT_EQ(0, ring_destroy(&ring));
}


TEST(ringQueueDeque, ringTests) {
    ring_t ring;

    const uint32_t ring_size = sizeof(float32_t);
    const uint32_t ring_len = 4;

    ASSERT_EQ(0, ring_create(&ring, ring_size, ring_len));

    const float32_t datum = 1.f;
    float32_t new_datum = 0.f;

    ASSERT_EQ(0, ring_enqueue(&ring, (void *)&datum));
    ASSERT_EQ(0, ring_dequeue(&ring, (void *)&new_datum));
    ASSERT_FLOAT_EQ(datum, new_datum);

    ASSERT_EQ(0, ring_destroy(&ring));
    ASSERT_EQ(nullptr, ring.data);
}


TEST(ringUB, ringTests) {
    ring_t ring;

    const uint32_t ring_size = sizeof(uint32_t);
    const uint32_t ring_len = 4;

    ASSERT_EQ(0, ring_create(&ring, ring_size, ring_len));

    // Write to the upper boundary (ring_len float values)

    uint32_t new_datum = 0.f;

    for (uint32_t i = 0; i < ring_len; ++i) {
        const uint32_t datum = (1.f + (uint32_t)i);
        ASSERT_EQ(0, ring_enqueue(&ring, &datum));
    }

    // Read to the upper boundary ()

    for (uint32_t i = 0; i < ring_len; ++i) {
        uint32_t old_datum = 0.f;
        const uint32_t new_datum = (1.f + (float32_t)i);

        ASSERT_EQ(0, ring_dequeue(&ring, (void *)&old_datum));

        printf("[%d]: %d\n", i, old_datum);
        ASSERT_EQ(old_datum, new_datum);
    }

    ASSERT_EQ(0, ring_destroy(&ring));
}


TEST(ringUUB, ringTests) {
    ring_t ring;

    const uint32_t ring_size = sizeof(float32_t);
    const uint32_t ring_len = 4;
    float32_t new_datum = 0.f;

    ASSERT_EQ(0, ring_create(&ring, ring_size, ring_len));

    // Write all the way to the upper boundary...

    for (uint32_t i = 0; i < ring_len; ++i) {
        const float32_t datum = (1.f + (float32_t)i);
        ASSERT_EQ(0, ring_enqueue(&ring, &datum));
    }

    // Write over the first index (wrapping write)

    const float32_t sentinel = -1.f;
    ASSERT_EQ(0, ring_enqueue(&ring, &sentinel));

    // Read for the first time (get the first index)

    float32_t compare = 0.f;
    ASSERT_EQ(0, ring_dequeue(&ring, &compare));
    ASSERT_FLOAT_EQ(sentinel, compare);

    ASSERT_EQ(0, ring_destroy(&ring));
}


TEST(ringLowerBoundary, ringTests) {
    ring_t ring;

    const uint32_t ring_size = sizeof(float32_t);
    const uint32_t ring_len = 4;
    float32_t datum = 0.f;

    ASSERT_EQ(0, ring_create(&ring, ring_size, ring_len));
    ASSERT_EQ(0, ring_dequeue(&ring, &datum));
    ASSERT_FLOAT_EQ(0.f, datum);

    ASSERT_EQ(0, ring_destroy(&ring));
}
