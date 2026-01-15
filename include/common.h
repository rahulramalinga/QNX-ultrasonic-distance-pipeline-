/*
 * common.h
 *
 *  Created on: 22-Nov-2025
 *      Author: Rahulr
 */

#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <time.h>


#define MSG_READING   1   // sensor → processing
#define MSG_FILTERED  2   // processing → display




typedef struct {
    int      ok;           // 1 = valid echo, 0 = timeout/error
    double   dist_cm;      // raw measured distance
    uint64_t t_start_ns;   // echo rising edge timestamp
    uint64_t t_end_ns;     // echo falling edge timestamp
} reading_t;


typedef struct {
    double   dist_cm_raw;       // raw distance
    double   dist_cm_filtered;  // moving average filtered value
    uint64_t ts_ns;             // timestamp of filtered reading
} filtered_t;


// High-Resolution Timestamp
// Returns current time in nanoseconds (QNX clock)
static inline uint64_t now_ns(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    return ((uint64_t)ts.tv_sec * 1000000000ULL) +
            (uint64_t)ts.tv_nsec;
}





#endif 
