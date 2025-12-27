#ifndef SCHED_CONTAINERS_ITEM_
#define SCHED_CONTAINERS_ITEM_

#include "utility/errors.h"
#include "traits/display.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct item {
    display_trait_t display;
} item_t;


#ifdef __cplusplus
};
#endif


#endif  // SCHED_CONTAINERS_ITEM_
