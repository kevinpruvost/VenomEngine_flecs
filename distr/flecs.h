// Comment out this line when using as DLL
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////#define flecs_STATIC
/**
 * @file flecs.h
 * @brief Flecs public API.
 *
 * This file contains the public API for Flecs.
 */

#ifndef FLECS_H
#define FLECS_H

/**
 * @defgroup c C API
 *
 * @{
 * @}
 */

/**
 * @defgroup core Core
 * @ingroup c
 * Core ECS functionality (entities, storage, queries).
 *
 * @{
 */

/**
 * @defgroup options API defines
 * Defines for customizing compile time features.
 *
 * @{
 */

/* Flecs version macros */
#define FLECS_VERSION_MAJOR 4  /**< Flecs major version. */
#define FLECS_VERSION_MINOR 0  /**< Flecs minor version. */
#define FLECS_VERSION_PATCH 2  /**< Flecs patch version. */

/** Flecs version. */
#define FLECS_VERSION FLECS_VERSION_IMPL(\
    FLECS_VERSION_MAJOR, FLECS_VERSION_MINOR, FLECS_VERSION_PATCH)

/** @def FLECS_CONFIG_HEADER
 * Allows for including a user-customizable header that specifies compile-time 
 * features. */
#ifdef FLECS_CONFIG_HEADER
#include "flecs_config.h"
#endif

/** @def ecs_float_t
 * Customizable precision for floating point operations */
#ifndef ecs_float_t
#define ecs_float_t float
#endif

/** @def ecs_ftime_t
 * Customizable precision for scalar time values. Change to double precision for
 * processes that can run for a long time (e.g. longer than a day). */
#ifndef ecs_ftime_t
#define ecs_ftime_t ecs_float_t
#endif

/** @def FLECS_LEGACY
 * Define when building for C89
 */
// #define FLECS_LEGACY

/** @def FLECS_ACCURATE_COUNTERS
 * Define to ensure that global counters used for statistics (such as the
 * allocation counters in the OS API) are accurate in multithreaded
 * applications, at the cost of increased overhead.
 */
// #define FLECS_ACCURATE_COUNTERS

/** @def FLECS_DISABLE_COUNTERS
 * Disables counters used for statistics. Improves performance, but
 * will prevent some features that rely on statistics from working,
 * like the statistics pages in the explorer.
 */
// #define FLECS_DISABLE_COUNTERS

/* Make sure provided configuration is valid */
#if defined(FLECS_DEBUG) && defined(FLECS_NDEBUG)
#error "invalid configuration: cannot both define FLECS_DEBUG and FLECS_NDEBUG"
#endif
#if defined(FLECS_DEBUG) && defined(NDEBUG)
#error "invalid configuration: cannot both define FLECS_DEBUG and NDEBUG"
#endif

/** @def FLECS_DEBUG
 * Used for input parameter checking and cheap sanity checks. There are lots of
 * asserts in every part of the code, so this will slow down applications.
 */
#if !defined(FLECS_DEBUG) && !defined(FLECS_NDEBUG)
#if defined(NDEBUG)
#define FLECS_NDEBUG
#else
#define FLECS_DEBUG
#endif
#endif

/** @def FLECS_SANITIZE
 * Enables expensive checks that can detect issues early. Recommended for
 * running tests or when debugging issues. This will severely slow down code.
 */
#ifdef FLECS_SANITIZE
#ifndef FLECS_DEBUG
#define FLECS_DEBUG /* If sanitized mode is enabled, so is debug mode */
#endif
#endif

/* Tip: if you see weird behavior that you think might be a bug, make sure to
 * test with the FLECS_DEBUG or FLECS_SANITIZE flags enabled. There's a good
 * chance that this gives you more information about the issue! */

/** @def FLECS_SOFT_ASSERT
 * Define to not abort for recoverable errors, like invalid parameters. An error
 * is still thrown to the console. This is recommended for when running inside a
 * third party runtime, such as the Unreal editor.
 *
 * Note that internal sanity checks (ECS_INTERNAL_ERROR) will still abort a
 * process, as this gives more information than a (likely) subsequent crash.
 *
 * When a soft assert occurs, the code will attempt to minimize the number of
 * side effects of the failed operation, but this may not always be possible.
 * Even though an application may still be able to continue running after a soft
 * assert, it should be treated as if in an undefined state.
 */
// #define FLECS_SOFT_ASSERT

/** @def FLECS_KEEP_ASSERT
 * By default asserts are disabled in release mode, when either FLECS_NDEBUG or
 * NDEBUG is defined. Defining FLECS_KEEP_ASSERT ensures that asserts are not
 * disabled. This define can be combined with FLECS_SOFT_ASSERT.
 */
// #define FLECS_KEEP_ASSERT

/** \def FLECS_CPP_NO_AUTO_REGISTRATION
 * When set, the C++ API will require that components are registered before they
 * are used. This is useful in multithreaded applications, where components need
 * to be registered beforehand, and to catch issues in projects where component 
 * registration is mandatory. Disabling automatic component registration also
 * slightly improves performance.
 * The C API is not affected by this feature.
 */
// #define FLECS_CPP_NO_AUTO_REGISTRATION

/** \def FLECS_CUSTOM_BUILD
 * This macro lets you customize which addons to build flecs with.
 * Without any addons Flecs is just a minimal ECS storage, but addons add
 * features such as systems, scheduling and reflection. If an addon is disabled,
 * it is excluded from the build, so that it consumes no resources. By default
 * all addons are enabled.
 *
 * You can customize a build by either whitelisting or blacklisting addons. To
 * whitelist addons, first define the FLECS_CUSTOM_BUILD macro, which disables
 * all addons. You can then manually select the addons you need by defining
 * their macro, like "FLECS_SYSTEM".
 *
 * To blacklist an addon, make sure to *not* define FLECS_CUSTOM_BUILD, and
 * instead define the addons you don't need by defining FLECS_NO_<addon>, for
 * example "FLECS_NO_SYSTEM". If there are any addons that depend on the
 * blacklisted addon, an error will be thrown during the build.
 *
 * Note that addons can have dependencies on each other. Addons will
 * automatically enable their dependencies. To see the list of addons that was
 * compiled in a build, enable tracing before creating the world by doing:
 *
 * @code
 * ecs_log_set_level(0);
 * @endcode
 *
 * which outputs the full list of addons Flecs was compiled with.
 */
// #define FLECS_CUSTOM_BUILD

/** @def FLECS_CPP_NO_AUTO_REGISTRATION
 * When set, the C++ API will require that components are registered before they
 * are used. This is useful in multithreaded applications, where components need
 * to be registered beforehand, and to catch issues in projects where component
 * registration is mandatory. Disabling automatic component registration also
 * slightly improves performance.
 * The C API is not affected by this feature.
 */
// #define FLECS_CPP_NO_AUTO_REGISTRATION

#ifndef FLECS_CUSTOM_BUILD
// #define FLECS_C          /**< C API convenience macros, always enabled */
#define FLECS_CPP           /**< C++ API */
#define FLECS_MODULE        /**< Module support */
#define FLECS_SCRIPT        /**< ECS data definition format */
#define FLECS_STATS         /**< Track runtime statistics */
#define FLECS_METRICS       /**< Expose component data as statistics */
#define FLECS_ALERTS        /**< Monitor conditions for errors */
#define FLECS_SYSTEM        /**< System support */
#define FLECS_PIPELINE      /**< Pipeline support */
#define FLECS_TIMER         /**< Timer support */
#define FLECS_META          /**< Reflection support */
#define FLECS_UNITS         /**< Builtin standard units */
#define FLECS_JSON          /**< Parsing JSON to/from component values */
#define FLECS_DOC           /**< Document entities & components */
#define FLECS_LOG           /**< When enabled ECS provides more detailed logs */
#define FLECS_APP           /**< Application addon */
#define FLECS_OS_API_IMPL   /**< Default implementation for OS API */
#define FLECS_HTTP          /**< Tiny HTTP server for connecting to remote UI */
#define FLECS_REST          /**< REST API for querying application data */
// #define FLECS_JOURNAL    /**< Journaling addon (disabled by default) */
// #define FLECS_PERF_TRACE /**< Enable performance tracing (disabled by default) */
#endif // ifndef FLECS_CUSTOM_BUILD

/** @def FLECS_LOW_FOOTPRINT
 * Set a number of constants to values that decrease memory footprint, at the
 * cost of decreased performance. */
// #define FLECS_LOW_FOOTPRINT
#ifdef FLECS_LOW_FOOTPRINT
#define FLECS_HI_COMPONENT_ID (16)
#define FLECS_HI_ID_RECORD_ID (16)
#define FLECS_SPARSE_PAGE_BITS (6)
#define FLECS_ENTITY_PAGE_BITS (6)
#define FLECS_USE_OS_ALLOC
#endif

/** @def FLECS_HI_COMPONENT_ID
 * This constant can be used to balance between performance and memory
 * utilization. The constant is used in two ways:
 * - Entity ids 0..FLECS_HI_COMPONENT_ID are reserved for component ids.
 * - Used as lookup array size in table edges.
 *
 * Increasing this value increases the size of the lookup array, which allows
 * fast table traversal, which improves performance of ECS add/remove
 * operations. Component ids that fall outside of this range use a regular map
 * lookup, which is slower but more memory efficient. */
#ifndef FLECS_HI_COMPONENT_ID
#define FLECS_HI_COMPONENT_ID (256)
#endif

/** @def FLECS_HI_ID_RECORD_ID
 * This constant can be used to balance between performance and memory
 * utilization. The constant is used to determine the size of the id record
 * lookup array. Id values that fall outside of this range use a regular map
 * lookup, which is slower but more memory efficient.
 */
#ifndef FLECS_HI_ID_RECORD_ID
#define FLECS_HI_ID_RECORD_ID (1024)
#endif

/** @def FLECS_SPARSE_PAGE_BITS
 * This constant is used to determine the number of bits of an id that is used
 * to determine the page index when used with a sparse set. The number of bits
 * determines the page size, which is (1 << bits).
 * Lower values decrease memory utilization, at the cost of more allocations. */
#ifndef FLECS_SPARSE_PAGE_BITS
#define FLECS_SPARSE_PAGE_BITS (12)
#endif

/** @def FLECS_ENTITY_PAGE_BITS
 * Same as FLECS_SPARSE_PAGE_BITS, but for the entity index. */
#ifndef FLECS_ENTITY_PAGE_BITS
#define FLECS_ENTITY_PAGE_BITS (12)
#endif

/** @def FLECS_USE_OS_ALLOC
 * When enabled, Flecs will use the OS allocator provided in the OS API directly
 * instead of the builtin block allocator. This can decrease memory utilization
 * as memory will be freed more often, at the cost of decreased performance. */
// #define FLECS_USE_OS_ALLOC

/** @def FLECS_ID_DESC_MAX
 * Maximum number of ids to add ecs_entity_desc_t / ecs_bulk_desc_t */
#ifndef FLECS_ID_DESC_MAX
#define FLECS_ID_DESC_MAX (32)
#endif

/** \def FLECS_EVENT_DESC_MAX
 * Maximum number of events in ecs_observer_desc_t */
#ifndef FLECS_EVENT_DESC_MAX
#define FLECS_EVENT_DESC_MAX (8)
#endif

/** @def FLECS_VARIABLE_COUNT_MAX
 * Maximum number of query variables per query */
#define FLECS_VARIABLE_COUNT_MAX (64)

/** \def FLECS_TERM_COUNT_MAX 
 * Maximum number of terms in queries. Should not exceed 64. */
#ifndef FLECS_TERM_COUNT_MAX
#define FLECS_TERM_COUNT_MAX 32
#endif

/** \def FLECS_TERM_ARG_COUNT_MAX 
 * Maximum number of arguments for a term. */
#ifndef FLECS_TERM_ARG_COUNT_MAX
#define FLECS_TERM_ARG_COUNT_MAX (16)
#endif

/** \def FLECS_QUERY_VARIABLE_COUNT_MAX
 * Maximum number of query variables per query. Should not exceed 128. */
#ifndef FLECS_QUERY_VARIABLE_COUNT_MAX
#define FLECS_QUERY_VARIABLE_COUNT_MAX (64)
#endif

/** @def FLECS_QUERY_SCOPE_NESTING_MAX
 * Maximum nesting depth of query scopes */
#ifndef FLECS_QUERY_SCOPE_NESTING_MAX
#define FLECS_QUERY_SCOPE_NESTING_MAX (8)
#endif

/** @} */

/**
 * @file api_defines.h
 * @brief Supporting defines for the public API.
 *
 * This file contains constants / macros that are typically not used by an
 * application but support the public API, and therefore must be exposed. This
 * header should not be included by itself.
 */

#ifndef FLECS_API_DEFINES_H
#define FLECS_API_DEFINES_H

/**
 * @file api_flags.h
 * @brief Bitset flags used by internals.
 */

#ifndef FLECS_API_FLAGS_H
#define FLECS_API_FLAGS_H

#ifdef __cplusplus
extern "C" {
#endif


////////////////////////////////////////////////////////////////////////////////
//// World flags
////////////////////////////////////////////////////////////////////////////////

#define EcsWorldQuitWorkers           (1u << 0)
#define EcsWorldReadonly              (1u << 1)
#define EcsWorldInit                  (1u << 2)
#define EcsWorldQuit                  (1u << 3)
#define EcsWorldFini                  (1u << 4)
#define EcsWorldMeasureFrameTime      (1u << 5)
#define EcsWorldMeasureSystemTime     (1u << 6)
#define EcsWorldMultiThreaded         (1u << 7)
#define EcsWorldFrameInProgress       (1u << 8)

////////////////////////////////////////////////////////////////////////////////
//// OS API flags
////////////////////////////////////////////////////////////////////////////////

#define EcsOsApiHighResolutionTimer   (1u << 0)
#define EcsOsApiLogWithColors         (1u << 1)
#define EcsOsApiLogWithTimeStamp      (1u << 2)
#define EcsOsApiLogWithTimeDelta      (1u << 3)


////////////////////////////////////////////////////////////////////////////////
//// Entity flags (set in upper bits of ecs_record_t::row)
////////////////////////////////////////////////////////////////////////////////

#define EcsEntityIsId                 (1u << 31)
#define EcsEntityIsTarget             (1u << 30)
#define EcsEntityIsTraversable        (1u << 29)


////////////////////////////////////////////////////////////////////////////////
//// Id flags (used by ecs_id_record_t::flags)
////////////////////////////////////////////////////////////////////////////////

#define EcsIdOnDeleteRemove            (1u << 0)
#define EcsIdOnDeleteDelete            (1u << 1)
#define EcsIdOnDeletePanic             (1u << 2)
#define EcsIdOnDeleteMask\
    (EcsIdOnDeletePanic|EcsIdOnDeleteRemove|EcsIdOnDeleteDelete)

#define EcsIdOnDeleteObjectRemove      (1u << 3)
#define EcsIdOnDeleteObjectDelete      (1u << 4)
#define EcsIdOnDeleteObjectPanic       (1u << 5)
#define EcsIdOnDeleteObjectMask\
    (EcsIdOnDeleteObjectPanic|EcsIdOnDeleteObjectRemove|\
        EcsIdOnDeleteObjectDelete)

#define EcsIdOnInstantiateOverride     (1u << 6)
#define EcsIdOnInstantiateInherit      (1u << 7)
#define EcsIdOnInstantiateDontInherit  (1u << 8)
#define EcsIdOnInstantiateMask\
    (EcsIdOnInstantiateOverride|EcsIdOnInstantiateInherit|\
        EcsIdOnInstantiateDontInherit)

#define EcsIdExclusive                 (1u << 9)
#define EcsIdTraversable               (1u << 10)
#define EcsIdTag                       (1u << 11)
#define EcsIdWith                      (1u << 12)
#define EcsIdCanToggle                 (1u << 13)
#define EcsIdIsTransitive              (1u << 14)

#define EcsIdHasOnAdd                  (1u << 16) /* Same values as table flags */
#define EcsIdHasOnRemove               (1u << 17) 
#define EcsIdHasOnSet                  (1u << 18)
#define EcsIdHasOnTableFill            (1u << 19)
#define EcsIdHasOnTableEmpty           (1u << 20)
#define EcsIdHasOnTableCreate          (1u << 21)
#define EcsIdHasOnTableDelete          (1u << 22)
#define EcsIdIsSparse                  (1u << 23)
#define EcsIdIsUnion                   (1u << 24)
#define EcsIdEventMask\
    (EcsIdHasOnAdd|EcsIdHasOnRemove|EcsIdHasOnSet|\
        EcsIdHasOnTableFill|EcsIdHasOnTableEmpty|EcsIdHasOnTableCreate|\
            EcsIdHasOnTableDelete|EcsIdIsSparse|EcsIdIsUnion)

#define EcsIdMarkedForDelete           (1u << 30)

/* Utilities for converting from flags to delete policies and vice versa */
#define ECS_ID_ON_DELETE(flags) \
    ((ecs_entity_t[]){0, EcsRemove, EcsDelete, 0, EcsPanic}\
        [((flags) & EcsIdOnDeleteMask)])
#define ECS_ID_ON_DELETE_TARGET(flags) ECS_ID_ON_DELETE(flags >> 3)
#define ECS_ID_ON_DELETE_FLAG(id) (1u << ((id) - EcsRemove))
#define ECS_ID_ON_DELETE_TARGET_FLAG(id) (1u << (3 + ((id) - EcsRemove)))

/* Utilities for converting from flags to instantiate policies and vice versa */
#define ECS_ID_ON_INSTANTIATE(flags) \
    ((ecs_entity_t[]){EcsOverride, EcsOverride, EcsInherit, 0, EcsDontInherit}\
        [(((flags) & EcsIdOnInstantiateMask) >> 6)])
#define ECS_ID_ON_INSTANTIATE_FLAG(id) (1u << (6 + ((id) - EcsOverride)))


////////////////////////////////////////////////////////////////////////////////
//// Iterator flags (used by ecs_iter_t::flags)
////////////////////////////////////////////////////////////////////////////////

#define EcsIterIsValid                 (1u << 0u)  /* Does iterator contain valid result */
#define EcsIterNoData                  (1u << 1u)  /* Does iterator provide (component) data */
#define EcsIterNoResults               (1u << 3u)  /* Iterator has no results */
#define EcsIterIgnoreThis              (1u << 4u)  /* Only evaluate non-this terms */
#define EcsIterHasCondSet              (1u << 6u)  /* Does iterator have conditionally set fields */
#define EcsIterProfile                 (1u << 7u)  /* Profile iterator performance */
#define EcsIterTrivialSearch           (1u << 8u)  /* Trivial iterator mode */
#define EcsIterTrivialTest             (1u << 11u) /* Trivial test mode (constrained $this) */
#define EcsIterTrivialCached           (1u << 14u) /* Trivial search for cached query */
#define EcsIterCacheSearch             (1u << 15u) /* Cache search */
#define EcsIterFixedInChangeComputed   (1u << 16u) /* Change detection for fixed in terms is done */
#define EcsIterFixedInChanged          (1u << 17u) /* Fixed in terms changed */
#define EcsIterSkip                    (1u << 18u) /* Result was skipped for change detection */
#define EcsIterCppEach                 (1u << 19u) /* Uses C++ 'each' iterator */

/* Same as event flags */
#define EcsIterTableOnly               (1u << 20u)  /* Result only populates table */


////////////////////////////////////////////////////////////////////////////////
//// Event flags (used by ecs_event_decs_t::flags)
////////////////////////////////////////////////////////////////////////////////

#define EcsEventTableOnly              (1u << 20u) /* Table event (no data, same as iter flags) */
#define EcsEventNoOnSet                (1u << 16u) /* Don't emit OnSet for inherited ids */


////////////////////////////////////////////////////////////////////////////////
//// Query flags (used by ecs_query_t::flags)
////////////////////////////////////////////////////////////////////////////////

/* Flags that can only be set by the query implementation */
#define EcsQueryMatchThis             (1u << 11u) /* Query has terms with $this source */
#define EcsQueryMatchOnlyThis         (1u << 12u) /* Query only has terms with $this source */
#define EcsQueryMatchOnlySelf         (1u << 13u) /* Query has no terms with up traversal */
#define EcsQueryMatchWildcards        (1u << 14u) /* Query matches wildcards */
#define EcsQueryMatchNothing          (1u << 15u) /* Query matches nothing */
#define EcsQueryHasCondSet            (1u << 16u) /* Query has conditionally set fields */
#define EcsQueryHasPred               (1u << 17u) /* Query has equality predicates */
#define EcsQueryHasScopes             (1u << 18u) /* Query has query scopes */
#define EcsQueryHasRefs               (1u << 19u) /* Query has terms with static source */
#define EcsQueryHasOutTerms           (1u << 20u) /* Query has [out] terms */
#define EcsQueryHasNonThisOutTerms    (1u << 21u) /* Query has [out] terms with no $this source */
#define EcsQueryHasMonitor            (1u << 22u) /* Query has monitor for change detection */
#define EcsQueryIsTrivial             (1u << 23u) /* Query can use trivial evaluation function */
#define EcsQueryHasCacheable          (1u << 24u) /* Query has cacheable terms */
#define EcsQueryIsCacheable           (1u << 25u) /* All terms of query are cacheable */
#define EcsQueryHasTableThisVar       (1u << 26u) /* Does query have $this table var */
#define EcsQueryCacheYieldEmptyTables (1u << 27u) /* Does query cache empty tables */


////////////////////////////////////////////////////////////////////////////////
//// Term flags (used by ecs_term_t::flags_)
////////////////////////////////////////////////////////////////////////////////

#define EcsTermMatchAny               (1u << 0)
#define EcsTermMatchAnySrc            (1u << 1)
#define EcsTermTransitive             (1u << 2)
#define EcsTermReflexive              (1u << 3)
#define EcsTermIdInherited            (1u << 4)
#define EcsTermIsTrivial              (1u << 5)
#define EcsTermIsCacheable            (1u << 7)
#define EcsTermIsScope                (1u << 8)
#define EcsTermIsMember               (1u << 9)
#define EcsTermIsToggle               (1u << 10)
#define EcsTermKeepAlive              (1u << 11)
#define EcsTermIsSparse               (1u << 12)
#define EcsTermIsUnion                (1u << 13)
#define EcsTermIsOr                   (1u << 14)


////////////////////////////////////////////////////////////////////////////////
//// Observer flags (used by ecs_observer_t::flags)
////////////////////////////////////////////////////////////////////////////////

#define EcsObserverIsMulti             (1u << 1u)  /* Does observer have multiple terms */
#define EcsObserverIsMonitor           (1u << 2u)  /* Is observer a monitor */
#define EcsObserverIsDisabled          (1u << 3u)  /* Is observer entity disabled */
#define EcsObserverIsParentDisabled    (1u << 4u)  /* Is module parent of observer disabled  */
#define EcsObserverBypassQuery         (1u << 5u)  /* Don't evaluate query for multi-component observer*/
#define EcsObserverYieldOnCreate       (1u << 6u)  /* Yield matching entities when creating observer */
#define EcsObserverYieldOnDelete       (1u << 7u)  /* Yield matching entities when deleting observer */


////////////////////////////////////////////////////////////////////////////////
//// Table flags (used by ecs_table_t::flags)
////////////////////////////////////////////////////////////////////////////////

#define EcsTableHasBuiltins            (1u << 1u)  /* Does table have builtin components */
#define EcsTableIsPrefab               (1u << 2u)  /* Does the table store prefabs */
#define EcsTableHasIsA                 (1u << 3u)  /* Does the table have IsA relationship */
#define EcsTableHasChildOf             (1u << 4u)  /* Does the table type ChildOf relationship */
#define EcsTableHasName                (1u << 5u)  /* Does the table type have (Identifier, Name) */
#define EcsTableHasPairs               (1u << 6u)  /* Does the table type have pairs */
#define EcsTableHasModule              (1u << 7u)  /* Does the table have module data */
#define EcsTableIsDisabled             (1u << 8u)  /* Does the table type has EcsDisabled */
#define EcsTableNotQueryable           (1u << 9u)  /* Table should never be returned by queries */
#define EcsTableHasCtors               (1u << 10u)
#define EcsTableHasDtors               (1u << 11u)
#define EcsTableHasCopy                (1u << 12u)
#define EcsTableHasMove                (1u << 13u)
#define EcsTableHasToggle              (1u << 14u)
#define EcsTableHasOverrides           (1u << 15u)

#define EcsTableHasOnAdd               (1u << 16u) /* Same values as id flags */
#define EcsTableHasOnRemove            (1u << 17u)
#define EcsTableHasOnSet               (1u << 18u)
#define EcsTableHasOnTableFill         (1u << 19u)
#define EcsTableHasOnTableEmpty        (1u << 20u)
#define EcsTableHasOnTableCreate       (1u << 21u)
#define EcsTableHasOnTableDelete       (1u << 22u)
#define EcsTableHasSparse              (1u << 23u)
#define EcsTableHasUnion               (1u << 24u)

#define EcsTableHasTraversable         (1u << 26u)
#define EcsTableMarkedForDelete        (1u << 30u)

/* Composite table flags */
#define EcsTableHasLifecycle     (EcsTableHasCtors | EcsTableHasDtors)
#define EcsTableIsComplex        (EcsTableHasLifecycle | EcsTableHasToggle | EcsTableHasSparse)
#define EcsTableHasAddActions    (EcsTableHasIsA | EcsTableHasCtors | EcsTableHasOnAdd | EcsTableHasOnSet)
#define EcsTableHasRemoveActions (EcsTableHasIsA | EcsTableHasDtors | EcsTableHasOnRemove)
#define EcsTableEdgeFlags        (EcsTableHasOnAdd | EcsTableHasOnRemove | EcsTableHasSparse | EcsTableHasUnion)
#define EcsTableAddEdgeFlags     (EcsTableHasOnAdd | EcsTableHasSparse | EcsTableHasUnion)
#define EcsTableRemoveEdgeFlags  (EcsTableHasOnRemove | EcsTableHasSparse | EcsTableHasUnion)

////////////////////////////////////////////////////////////////////////////////
//// Aperiodic action flags (used by ecs_run_aperiodic)
////////////////////////////////////////////////////////////////////////////////

#define EcsAperiodicEmptyTables        (1u << 1u)  /* Process pending empty table events */
#define EcsAperiodicComponentMonitors  (1u << 2u)  /* Process component monitors */
#define EcsAperiodicEmptyQueries       (1u << 4u)  /* Process empty queries */

#ifdef __cplusplus
}
#endif

#endif


#if defined(_WIN32) || defined(_MSC_VER)
#define ECS_TARGET_WINDOWS
#elif defined(__ANDROID__)
#define ECS_TARGET_ANDROID
#define ECS_TARGET_POSIX
#elif defined(__linux__)
#define ECS_TARGET_LINUX
#define ECS_TARGET_POSIX
#elif defined(__FreeBSD__)
#define ECS_TARGET_FREEBSD
#define ECS_TARGET_POSIX
#elif defined(__APPLE__) && defined(__MACH__)
#define ECS_TARGET_DARWIN
#define ECS_TARGET_POSIX
#elif defined(__EMSCRIPTEN__)
#define ECS_TARGET_EM
#define ECS_TARGET_POSIX
#endif

#if defined(__MINGW32__) || defined(__MINGW64__)
#define ECS_TARGET_MINGW
#endif

#if defined(_MSC_VER)
#ifndef __clang__
#define ECS_TARGET_MSVC
#endif
#endif

#if defined(__clang__)
#define ECS_TARGET_CLANG
#endif

#if defined(__GNUC__)
#define ECS_TARGET_GNU
#endif

/* Map between clang and apple clang versions, as version 13 has a difference in
 * the format of __PRETTY_FUNCTION__ which enum reflection depends on. */
#if defined(__clang__)
    #if defined(__APPLE__)
        #if __clang_major__ == 13
            #if __clang_minor__ < 1
                #define ECS_CLANG_VERSION 12
            #else
                #define ECS_CLANG_VERSION 13
            #endif
        #else
            #define ECS_CLANG_VERSION __clang_major__
        #endif
    #else
        #define ECS_CLANG_VERSION __clang_major__
    #endif
#endif

/* Ignored warnings */
#if defined(ECS_TARGET_CLANG)
/* Ignore unknown options so we don't have to care about the compiler version */
#pragma clang diagnostic ignored "-Wunknown-warning-option"
/* Warns for double or redundant semicolons. There are legitimate cases where a
 * semicolon after an empty statement is useful, for example after a macro that
 * is replaced with a code block. With this warning enabled, semicolons would 
 * only have to be added after macro's that are not code blocks, which in some
 * cases isn't possible as the implementation of a macro can be different in
 * debug/release mode. */
#pragma clang diagnostic ignored "-Wextra-semi-stmt"
/* This is valid in C99, and Flecs must be compiled as C99. */
#pragma clang diagnostic ignored "-Wdeclaration-after-statement"
/* Clang attribute to detect fallthrough isn't supported on older versions. 
 * Implicit fallthrough is still detected by gcc and ignored with "fall through"
 * comments */
#pragma clang diagnostic ignored "-Wimplicit-fallthrough"
/* This warning prevents adding a default case when all enum constants are part
 * of the switch. In C however an enum type can assume any value in the range of
 * the type, and this warning makes it harder to catch invalid enum values. */
#pragma clang diagnostic ignored "-Wcovered-switch-default"
/* This warning prevents some casts of function results to a different kind of
 * type, e.g. casting an int result to double. Not very useful in practice, as
 * it just forces the code to assign to a variable first, then cast. */
#pragma clang diagnostic ignored "-Wbad-function-cast"
/* Format strings can be passed down from other functions. */
#pragma clang diagnostic ignored "-Wformat-nonliteral"
/* Useful, but not reliable enough. It can incorrectly flag macro's as unused
 * in standalone builds. */
#pragma clang diagnostic ignored "-Wunused-macros"
#if __clang_major__ == 13
/* clang 13 can throw this warning for a define in ctype.h */
#pragma clang diagnostic ignored "-Wreserved-identifier"
#endif
/* Filenames aren't consistent across targets as they can use different casing 
 * (e.g. WinSock2 vs winsock2). */
#pragma clang diagnostic ignored "-Wnonportable-system-include-path"
/* Very difficult to workaround this warning in C, especially for an ECS. */
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
/* This warning gets thrown when trying to cast p