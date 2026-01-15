
/*
 * main.c
 *
 *  Created on: 20-Nov-2025
 *      Author: Rahulr
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/neutrino.h>
#include <sys/dispatch.h>
#include <pthread.h>

#include "common.h"
#include "gpio.h"       
#include "rpi_gpio.h"    


// Thread contexts
typedef struct {
	int coid_proc;
} sensor_ctx_t;

typedef struct {
	int chid;
	int coid_disp;
} proc_ctx_t;

typedef struct {
	int chid;
	FILE *logf;
} disp_ctx_t;

// Thread entry functions
void* sensor_task(void*);
void* processing_task(void*);
void* display_task(void*);


static volatile int running = 1;

static void on_sigint(int sig) {
	(void) sig;
	running = 0;
}


// Main
int main(void) {
	signal(SIGINT, on_sigint);

	// Initialize GPIO MMIO + configure pins
	if (gpio_hal_init() != 0) {
		fprintf(stderr, "ERROR: GPIO init failed (need root?)\n");
		return 1;
	}

	
	// Create channels
	int chid_disp = ChannelCreate(0);
	if (chid_disp < 0) {
		perror("ChannelCreate(display)");
		return 1;
	}

	int chid_proc = ChannelCreate(0);
	if (chid_proc < 0) {
		perror("ChannelCreate(processing)");
		return 1;
	}

	// Create connections
	int coid_disp = ConnectAttach(0, 0, chid_disp, 0, 0);
	if (coid_disp < 0) {
		perror("ConnectAttach(display)");
		return 1;
	}

	int coid_proc = ConnectAttach(0, 0, chid_proc, 0, 0);
	if (coid_proc < 0) {
		perror("ConnectAttach(processing)");
		return 1;
	}

	
	// Spawn threads
	pthread_t th_disp, th_proc, th_sensor;

	// Display thread
	disp_ctx_t dctx = { .chid = chid_disp, .logf = NULL };
	pthread_create(&th_disp, NULL, display_task, &dctx);

	// Processing thread
	proc_ctx_t prctx = { .chid = chid_proc, .coid_disp = coid_disp };
	pthread_create(&th_proc, NULL, processing_task, &prctx);

	// Sensor thread
	sensor_ctx_t sctx = { .coid_proc = coid_proc };
	pthread_create(&th_sensor, NULL, sensor_task, &sctx);

	
	
	while (running) {
		sleep(1);
	}

	gpio_hal_deinit();

	
	return 0;
}
