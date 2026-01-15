/*
 * display.c
 *
 *  Created on: 21-Nov-2025
 *      Author: Rahulr
 */

#include <stdio.h>
#include <inttypes.h>
#include <sys/neutrino.h>
#include <errno.h>

#include "common.h"


typedef struct {
    int   chid;   
    FILE *logf;   
} disp_ctx_t;

void* display_task(void* arg)
{
    disp_ctx_t *ctx = (disp_ctx_t*)arg;

    
    ctx->logf = fopen("distance_log.csv", "a");
    if (ctx->logf) {
        fprintf(ctx->logf, "ts_ns,raw_cm,filtered_cm\n");
        fflush(ctx->logf);
    }

    while (1)
    {
        struct {
            int type;
            filtered_t payload;
        } msg;

        int rcvid = MsgReceive(ctx->chid, &msg, sizeof(msg), NULL);
        if (rcvid < 0)
            continue;

        if (msg.type == MSG_FILTERED)
        {
            MsgReply(rcvid, EOF, NULL, 0);

            filtered_t f = msg.payload;

            
            printf("Distance: raw=%6.2f cm | filtered=%6.2f cm\n",
                   f.dist_cm_raw, f.dist_cm_filtered);

            
            if (ctx->logf) {
                fprintf(ctx->logf, "%" PRIu64 ",%.3f,%.3f\n",
                        f.ts_ns, f.dist_cm_raw, f.dist_cm_filtered);
                fflush(ctx->logf);
            }
        }
        else
        {
            MsgReply(rcvid, ENOSYS, NULL, 0);
        }
    }

    return NULL;
}
