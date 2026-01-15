/*
 * processing.c
 *
 *  Created on: 21-Nov-2025
 *      Author: Rahulr
 */
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <sys/neutrino.h>
#include <errno.h>
#include "common.h"


#define PROC_WIN 5

typedef struct {
    int chid;       
    int coid_disp;   
} proc_ctx_t;

static int reject_outlier(double x)
{
    // Valid ultrasonic distance range
    return (x < 2.0 || x > 400.0);
}

void* processing_task(void* arg)
{
    proc_ctx_t *ctx = (proc_ctx_t*)arg;

    double buf[PROC_WIN] = {0.0};
    int count = 0, idx = 0;

    while (1)
    {
        struct {
            int type;
            reading_t payload;
        } msg;

        int rcvid = MsgReceive(ctx->chid, &msg, sizeof(msg), NULL);
        if (rcvid < 0)
            continue;

        if (msg.type == MSG_READING)
        {
            reading_t r = msg.payload;
            MsgReply(rcvid, EOK, NULL, 0);

            double raw = r.dist_cm;

            if (!r.ok || reject_outlier(raw))
                continue;

            
            buf[idx] = raw;
            idx = (idx + 1) % PROC_WIN;
            if (count < PROC_WIN) count++;

            double sum = 0.0;
            for (int i = 0; i < count; ++i)
                sum += buf[i];

            double avg = sum / (double)count;

            filtered_t f = {
                .dist_cm_raw = raw,
                .dist_cm_filtered = avg,
                .ts_ns = now_ns()
            };

            struct {
                int type;
                filtered_t payload;
            } outmsg = { MSG_FILTERED, f };

            if (MsgSend(ctx->coid_disp, &outmsg, sizeof(outmsg), NULL, 0) == -1)
                perror("processing -> display MsgSend");
        }
        else
        {
            MsgReply(rcvid, ENOSYS, NULL, 0);
        }
    }

    return NULL;
}
