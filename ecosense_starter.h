// EcoSense starter header for xv6-style integration.
// Drop this into your xv6 tree (e.g., user/ecosense.h or a shared include)
// and adjust includes/names as needed for your specific xv6 version.

#ifndef ECOSENSE_STARTER_H
#define ECOSENSE_STARTER_H

// Example sensor types. You can extend this list.
enum sensor_type {
  SENSOR_TEMPERATURE = 0,
  SENSOR_AIR_QUALITY = 1,
  SENSOR_ENERGY_USAGE = 2,
  SENSOR_COUNT
};

// Simple representation of a single sensor reading.
struct sensordata {
  int id;           // sensor ID (0..SENSOR_COUNT-1)
  int type;         // one of enum sensor_type
  int value;        // raw value (e.g., degrees * 10, AQI, watts, etc.)
  int threshold;    // alert threshold (optional; 0 if unused)
  int alert;        // non-zero if value crossed threshold when sampled
};

// Example user/kernel-facing API you might implement as syscalls
// in xv6 (actual signatures can be adjusted as needed).

// Read up to 'max' sensor readings into 'buf'.
// Returns number of readings copied, or -1 on error.
int readsensors(struct sensordata *buf, int max);

// Set an alert threshold for a particular sensor ID.
// Returns 0 on success, -1 on error.
int setsensorthreshold(int id, int threshold);

// Semaphore API (if you choose to implement semaphores as a feature).
int sem_create(int initial);   // returns semaphore id or -1
int sem_wait(int semid);       // block until semaphore > 0, then decrement
int sem_signal(int semid);     // increment semaphore and potentially wake waiters
int sem_destroy(int semid);    // free semaphore, return 0 on success

#endif // ECOSENSE_STARTER_H

