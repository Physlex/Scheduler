#ifndef SCHED_CORE_TRAITS_DISPLAY_H_
#define SCHED_CORE_TRAITS_DISPLAY_H_

/** @file `display.h`
 *  @brief TODO: DOCS
 */

/** @typedef display_trait_t
 *  @brief This type defines the display trait.
 * 
 *         The display trait defines a way to display information about a given item.
 *
 *         This trait is typically defined in container types to simplify printing debug
 *         information.
 */
typedef void (*display_trait_t)(void *);

/** @typedef fmt_trait_t
 *  @brief This type defines the format trait. TODO: UPDATE DOCS
 */
typedef const char *(*fmt_trait_t)(void *);


#endif  // SCHED_CORE_TRAITS_DISPLAY_H_
