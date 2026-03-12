// EcoSense dashboard starter code for xv6.
// This is NOT a complete program — it is a skeleton you can paste into
// your xv6 'user/' directory and wire up once you implement the syscalls
// declared in 'ecosense_starter.h'.

#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#include "ecosense_starter.h"

// Simple helper to clear the screen using ANSI escape codes.
static void
clear_screen(void)
{
  // ESC[2J  -> clear screen
  // ESC[H   -> move cursor to home position
  printf(1, "\x1b[2J\x1b[H");
}

// Basic function to print one sensor row.
static void
print_sensor_row(struct sensordata *s)
{
  char *name;
  switch (s->type) {
  case SENSOR_TEMPERATURE:
    name = "TEMP";
    break;
  case SENSOR_AIR_QUALITY:
    name = "AIRQ";
    break;
  case SENSOR_ENERGY_USAGE:
    name = "POWER";
    break;
  default:
    name = "UNK ";
    break;
  }

  // Highlight alerts in red using ANSI if alert is set.
  if (s->alert) {
    printf(1, "\x1b[31m"); // red
  }

  printf(1, "Sensor %-5s  id=%d  value=%d  thresh=%d  alert=%d\n",
         name, s->id, s->value, s->threshold, s->alert);

  if (s->alert) {
    printf(1, "\x1b[0m"); // reset color
  }
}

int
main(int argc, char *argv[])
{
  struct sensordata buf[16];
  int n;

  // TODO: (kernel side) implement 'readsensors' syscall and wire it up.
  // This loop assumes 'readsensors' is available and returns the number
  // of sensor records written into 'buf'.

  for (;;) {
    clear_screen();
    printf(1, "EcoSense Live Dashboard\n");
    printf(1, "=======================\n\n");

    n = readsensors(buf, 16);
    if (n < 0) {
      printf(1, "readsensors() not implemented yet or returned error.\n");
    } else if (n == 0) {
      printf(1, "No sensor data available.\n");
    } else {
      int i;
      for (i = 0; i < n; i++) {
        print_sensor_row(&buf[i]);
      }
    }

    // Sleep a bit between refreshes. Adjust as needed.
    sleep(50); // ~0.5s depending on xv6 tick rate
  }

  exit();
}

