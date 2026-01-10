/** @file `ring.cpp`
 *  @brief 
 */

#include <gtest/gtest.h>

#include "container/ring.h"
#include "utility/errors.h"


TEST(ringCreate, ringTests) {
    const uint32_t ring_size = sizeof(float32_t);
    const uint32_t ring_len = 4;

    ring_t *ring = ring_new(ring_size, ring_len);
    ASSERT_NE(nullptr, ring);

    ASSERT_EQ(0, ring_destroy(&ring));
    ASSERT_EQ(nullptr, ring);
}


TEST(ringDestroyNull, ringTests) {
    const uint32_t ring_size = sizeof(float32_t);
    const uint32_t ring_len = 4;

    ASSERT_NE(0, ring_destroy(nullptr));
}


TEST(ringQueue, ringTests) {
    const uint32_t ring_size = sizeof(float32_t);
    const uint32_t ring_len = 4;
    const uint32_t datum = 1;

    ring_t *ring = ring_new(ring_size, ring_len);

    ASSERT_NE(nullptr, ring);
    ASSERT_EQ(0, ring_enqueue(ring, (void *)&datum));
    ASSERT_EQ(0, ring_destroy(&ring));
}


TEST(ringQueueDequeue, ringTests) {
    const uint32_t ring_size = sizeof(float32_t);
    const uint32_t ring_len = 4;
    ring_t *ring = ring_new(ring_size, ring_len);

    ASSERT_NE(nullptr, ring);

    const float32_t datum = 1.f;
    float32_t new_datum = 0.f;

    ASSERT_EQ(0, ring_enqueue(ring, (void *)&datum));
    ASSERT_EQ(0, ring_dequeue(ring, (void *)&new_datum));

    ASSERT_FLOAT_EQ(datum, new_datum);

    ASSERT_EQ(0, ring_destroy(&ring));
    ASSERT_EQ(nullptr, ring);
}


TEST(ringUB, ringTests) {
    const uint32_t ring_size = sizeof(uint32_t);
    const uint32_t ring_len = 4;
    ring_t *ring = ring_new(ring_size, ring_len);

    ASSERT_NE(nullptr, ring);

    // Write to the upper boundary (ring_len float values)

    uint32_t new_datum = 0.f;

    for (uint32_t i = 0; i < ring_len; ++i) {
        const uint32_t datum = (1.f + (uint32_t)i);
        ASSERT_EQ(0, ring_enqueue(ring, &datum));
    }

    // Read to the upper boundary ()

    for (uint32_t i = 0; i < ring_len; ++i) {
        uint32_t old_datum = 0.f;
        const uint32_t new_datum = (1.f + (float32_t)i);

        ASSERT_EQ(0, ring_dequeue(ring, (void *)&old_datum));

        printf("[%d]: %d\n", i, old_datum);
        ASSERT_EQ(old_datum, new_datum);
    }

    ASSERT_EQ(0, ring_destroy(&ring));
}


TEST(ringUUB, ringTests) {
    const uint32_t ring_size = sizeof(float32_t);
    const uint32_t ring_len = 4;
    ring_t *ring = ring_new(ring_size, ring_len);

    float32_t new_datum = 0.f;

    ASSERT_NE(nullptr, ring);
    ASSERT_EQ(16, ring_capacity(ring));

    // Write all the way to the upper boundary...

    for (uint32_t i = 0; i < ring_len; ++i) {
        const float32_t datum = (1.f + (float32_t)i);
        ASSERT_EQ(0, ring_enqueue(ring, &datum));
    }

    // Make sure read_ptr == write_ptr
    ASSERT_EQ(ring_write_ptr(ring), ring_read_ptr(ring));

    // Write over the first index (wrapping write)

    const float32_t sentinel = -1.f;
    ASSERT_EQ(0, ring_enqueue(ring, &sentinel));

    // Read for the first time (get the first index)

    float32_t compare = 0.f;
    ASSERT_EQ(0, ring_dequeue(ring, &compare));
    ASSERT_FLOAT_EQ(sentinel, compare);

    ASSERT_EQ(0, ring_destroy(&ring));
    ASSERT_EQ(nullptr, ring);
}


TEST(ringLowerBoundary, ringTests) {
    const uint32_t ring_size = sizeof(float32_t);
    const uint32_t ring_len = 4;

    ring_t *ring = ring_new(ring_size, ring_len);
    ASSERT_NE(nullptr, ring);

    float32_t datum = 0.f;

    ASSERT_EQ(0, ring_dequeue(ring, &datum));
    ASSERT_FLOAT_EQ(0.f, datum);

    ASSERT_EQ(0, ring_destroy(&ring));
}
