
/*
 * sensor.c
 *
 *  Created on: 21-Nov-2025
 *      Author: Rahulr
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/neutrino.h>

#include "common.h"
#include "hc_sr04.h"
#include "gpio.h"        
#include "rpi_gpio.h"   

typedef struct {
    int coid_proc;   
} sensor_ctx_t;

void* sensor_task(void* arg)
{
    sensor_ctx_t *ctx = (sensor_ctx_t*)arg;

    // Configure GPIO directions
    gpio_set_output(TRIG_GPIO);
    gpio_set_input(ECHO_GPIO);

    while (1)
    {
        reading_t r = {0};
        double d = 0.0;
        uint64_t t0 = 0, t1 = 0;

        r.ok = hcsr04_measure_once(&d, &t0, &t1);
        r.dist_cm = d;
        r.t_start_ns = t0;
        r.t_end_ns = t1;

        struct {
            int type;
            reading_t payload;
        } msg = { MSG_READING, r };

        if (MsgSend(ctx->coid_proc, &msg, sizeof(msg), NULL, 0) == -1)
            perror("sensor -> processing MsgSend");

        usleep(100000); 
    }
    return NULL;
}
