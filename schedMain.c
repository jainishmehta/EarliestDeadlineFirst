/**
 * Perform timeline scheduling on a taskset.
 * @author Andrew Morton
 */

#include "sched.h"
#include <stdio.h>

void t0(void) { printf("t0\n"); }
void t1(void) { printf("t1\n"); }
void t2(void) { printf("t2\n"); }
void t3(void) { printf("t3\n"); }

int main(void) {
	task_t tasks[] = {
	//{ func,  T, C }
		{ t0, 20, 6},
		{ t1, 15, 3},
		{ t2, 10, 3 },
		{ t3, 10,2},};
	const int nTask = sizeof(tasks)/sizeof(task_t);
	const int LCM = 60;
	const int nSlot = 60/20 + 60/15 + 60/10+60/10;
	int slots[nSlot];

	if(scheduleEDF(slots, nSlot, tasks, nTask, LCM)) {
		uint32_t time = 0;
		for(int i=0; i<nSlot; i++) {
			if(!c_assert(slots[i] >= 0 && slots[i] < nTask)) continue;
			printf("%3d: ", time);
			tasks[slots[i]].t();
			time += tasks[slots[i]].C;
		}
	}
}
