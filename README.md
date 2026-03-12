## EcoSense: Environmental Monitoring on xv6

**Course**: SE3313 – Operating Systems  
**Project type**: Mini project – OS design for environmental sustainability  

### 1. Project overview

**EcoSense** is a real-time environmental monitoring subsystem built on top of **xv6**.  
We simulate environmental sensors as xv6 processes/threads and expose their readings through a simple virtual device and system calls. The design emphasizes:

- **Environmental sustainability**: showing how OS mechanisms can support green/IoT monitoring workloads.
- **OS concepts**: synchronization, system calls, virtual devices, and user-level interfaces.

The system is intentionally modular so that each feature can be implemented, tested, and demoed independently (matching the rubric's per-feature marking scheme).

### 2. Sustainability angle

EcoSense targets **design for environmental sustainability**:

- **Simulated sensors** represent temperature, air quality, and energy-usage probes that might exist in a real building or data center.
- **Efficient aggregation** reduces redundant sampling and avoids busy-waiting, illustrating how good OS design can lower CPU utilization and therefore energy use.
- **Centralized policy** in the kernel lets us enforce sampling intervals and alert thresholds, which is how real systems reduce unnecessary work and respond quickly to environmental risks.

This directly matches the instructor’s example of "environmental sensors as processes/threads in xv6" and extends it with a richer synchronization and monitoring story.

### 3. Planned xv6 features (≥4 new features)

Each bullet below is designed to be a **separate, demoable xv6 feature** worth 2.5 marks on the rubric.

- **Feature 1 – Virtual sensor device `/dev/sensors`**  
  - Add a new character device in xv6 that exposes current environmental readings.  
  - Reading from `/dev/sensors` returns a small struct/line per sensor (temperature, air quality, energy usage, etc.).

- **Feature 2 – Sensor processes / kernel-backed sensor simulator**  
  - Implement one or more kernel threads or user processes that periodically "sample" each sensor (using a pseudo-random generator).  
  - Values are written into per-sensor ring buffers managed in the kernel.

- **Feature 3 – Synchronization via semaphores/monitors**  
  - Add a new synchronization primitive to xv6 (e.g., counting semaphores or a simple monitor abstraction).  
  - Use this primitive to protect the shared sensor buffers between producers (sensor simulators) and consumers (aggregator or user-space readers).

- **Feature 4 – Aggregation & threshold alerts**  
  - Implement a kernel-side aggregator that computes rolling averages/min/max for each sensor.  
  - Expose a simple alert mechanism (e.g., a flag in the struct or a special return code) when a reading crosses a pre-set threshold (high temperature, poor air quality, high energy usage).

- **Feature 5 – User-space live dashboard (`ecosense`)**  
  - Add a user program that periodically reads `/dev/sensors` or calls a `readsensors()` syscall.  
  - Render a live, text-based dashboard using ANSI escape codes (ncurses-style) to show current values and highlight alerts in real time.

Only four new features are required by the rubric; the extra one gives us flexibility in case one feature is incomplete by demo time.

### 4. High-level architecture

- **Kernel additions**
  - Sensor manager: maintains a small array of sensor descriptors and ring buffers.
  - New system calls (examples):  
    - `int readsensors(struct sensordata *buf, int max);` – copy up to `max` latest readings into user space.  
    - `int setsensorthreshold(int id, int threshold);` – configure alert thresholds per sensor (optional).  
  - New synchronization primitive (e.g., semaphore or monitor struct) and corresponding syscalls:  
    - `int sem_create(int initial);`  
    - `int sem_wait(int id);`  
    - `int sem_signal(int id);`  
    - `int sem_destroy(int id);`
  - Virtual device implementation so that `/dev/sensors` maps onto the sensor manager and/or `readsensors()` logic.

- **User space**
  - `ecosense` dashboard program that:
    - Periodically calls `readsensors()` or reads `/dev/sensors`.  
    - Clears the terminal and reprints a table of sensor values.  
    - Uses simple ASCII/ANSI colors to highlight alerts.
  - Optional helper programs for unit tests (e.g., `sensortest`, `semtest`).

### 5. How this meets the rubric

- **Functional requirement: sustainability**  
  - Directly models environmental sensors and shows how OS-level design (scheduling, synchronization, virtual devices) can support sustainable monitoring applications.
- **At least four new xv6 features**  
  - New synchronization primitive (semaphores/monitors) + associated syscalls.  
  - New sensor manager and kernel data structures.  
  - New `/dev/sensors` device (or `readsensors()` syscall interface).  
  - New user-level dashboard program (`ecosense`).  
  - (Optional extra) Threshold alert mechanism and configuration syscalls.
- **Documentation & tests**  
  - Each feature will have:
    - A short description of its purpose and interface.  
    - A unit test / demo program (e.g., a small user program exercising just that feature).

### 6. Implementation roadmap (file-level plan for xv6)

This repository currently holds the **design and starter code**; the actual implementation will happen inside an xv6 tree (e.g., in a fork on GitHub).

In xv6, we expect to modify/add (names may differ slightly depending on xv6 version):

- **Kernel**
  - `syscall.h`, `syscall.c`, `sysproc.c`: declare and wire up new syscalls (`readsensors`, semaphore calls, etc.).  
  - `defs.h`, `proc.c` or a new `sensor.c`: implement the sensor manager, sensor buffers, and aggregator.  
  - `file.c`, `fs.c`, `devsw` table: register a new character device for `/dev/sensors` (if using the device interface).

- **User programs**
  - Add `ecosense.c` (dashboard) to `user/` and update the user `Makefile` to build it.  
  - Add small test programs such as `semtest.c` and `sensortest.c` to verify synchronization and sensor behavior.

We will keep each change as self-contained as possible so that any broken feature can be disabled for the demo without affecting others.

### 7. Testing and demo plan

- **Unit tests (per feature)**  
  - **Synchronization tests**: create producer/consumer pairs using the new semaphore/monitor APIs, check for correct ordering and absence of deadlock.  
  - **Sensor simulator tests**: verify that sensor values change over time and stay within expected ranges.  
  - **Aggregation/alert tests**: feed extreme values and check that thresholds trigger correctly.  
  - **Dashboard tests**: run `ecosense` for a fixed duration and visually confirm that values update and alerts are highlighted.

- **Demo script**
  1. Boot xv6 with EcoSense enabled.  
  2. Run `ecosense` to show the live dashboard.  
  3. Trigger scenarios (e.g., simulate heatwave or high energy usage) and show alerts flipping on.  
  4. Briefly explain which OS features make this possible (syscalls, synchronization, virtual devices).

### 8. LinkedIn-ready description

You can use or adapt the following text:

> Built a real-time environmental monitoring subsystem in xv6 (MIT’s teaching OS), implementing multi-threaded sensor simulation, semaphore-synchronized data aggregation, and a virtual `/dev/sensors` device with a live text-based dashboard — demonstrating how OS design can support IoT sustainability applications.

### 9. Statement of contribution (to be completed at the end)

On the last page of the full project report (separate from this README), include:

- **Per-member contributions**: design, kernel implementation, user programs, testing, documentation, demo preparation, etc.  
- **Signatures** (or electronic acknowledgement) from each team member confirming the statement.

