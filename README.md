# QNX-ultrasonic-distance-pipeline-


## Overview
This project implements a **real-time ultrasonic distance measurement system**
using **QNX Neutrino RTOS** on a **Raspberry Pi 4**.  
The system demonstrates **deterministic timing**, **RTOS-native message passing**,
and **hardware-level GPIO control** using an HC-SR04 ultrasonic sensor.

The design follows a **modular pipeline architecture**, where sensing,
processing, and display are isolated into independent real-time threads.

---

## Objectives
- Demonstrate real-time behavior using QNX Neutrino microkernel
- Implement deterministic sensor timing using high-resolution clocks
- Use QNX native IPC (`MsgSend`, `MsgReceive`, `MsgReply`)
- Build a scalable and modular RTOS sensing pipeline

---

## System Architecture
The application runs as a **single QNX process** with **multiple threads**:

### Threads
1. **Sensor Thread**
   - Triggers the HC-SR04 ultrasonic sensor
   - Captures echo pulse width using high-resolution timers
   - Sends raw time-of-flight data to processing thread

2. **Processing Thread**
   - Converts time-of-flight into distance (cm)
   - Applies outlier rejection and moving average filtering
   - Sends filtered distance values to display thread

3. **Display**
   - Prints distance to terminal
   - Optionally logs distance data to a file

Inter-thread communication is implemented using **QNX message passing**, ensuring
deterministic execution and clean separation of responsibilities.

---

## Timing & Distance Calculation
Distance is calculated using the standard ultrasonic ranging formula:
Distance = (Time_of_Flight × Speed_of_Sound) / 2



- Speed of sound assumed: **34300 cm/s (at 25°C)**
- Echo timing captured using:
  - `ClockCycles()`
  - `clock_gettime(CLOCK_MONOTONIC, ...)`

`ClockCycles()` is preferred when cycle-level accuracy is required.

---

## Hardware & Software

### Hardware
- Raspberry Pi 4
- HC-SR04 Ultrasonic Sensor
- Breadboard and jumper wires
- Ethernet connection to host PC

### Software
- QNX Neutrino RTOS
- QNX Momentics IDE
- C (POSIX + QNX native APIs)

---

---

## Key RTOS Concepts Demonstrated
- Microkernel-based RTOS design
- Priority-aware multithreading
- Deterministic interprocess communication
- Hardware abstraction layer (HAL)
- Low-jitter timing suitable for control systems

---

## Future Enhancements
- Assign thread priorities and CPU affinities
- Add watchdog monitoring
- Support multiple sensors
- Publish data over network (UDP/TCP)
- Integrate with QNX resource manager

---

## Author
Rahul R  
Electronics & Communication Engineering  
RTOS | Embedded Systems | Hardware–Software Interface


