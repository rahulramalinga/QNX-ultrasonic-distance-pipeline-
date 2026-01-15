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

