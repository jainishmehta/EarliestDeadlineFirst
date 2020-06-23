#ifndef SCHED_H
#define SCHED_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define c_assert(e) ((e) ? (true) : \
		(printf("%s,%d: assertion '%s' failed\n", \
		__FILE__, __LINE__, #e), false))

typedef void (*taskPtr)(void);

typedef struct {
	taskPtr t;
	uint32_t T;
	uint32_t C;
} task_t;

/**
 * Create task execution order.  Assigns task numbers to slots.  Uses
 * non-preemptive EDF.
 * @param slots array representing task execution order
 * @param nSlot number of entries in slots
 * @param tasks array of tasks (the taskset)
 * @param nTask size of the taskset
 * @param LCM length of the fixed schedule
 * @return true on success, false on failure
 */
bool scheduleEDF(int slots[], int nSlot, task_t tasks[], int nTask, int LCM);

#endif
