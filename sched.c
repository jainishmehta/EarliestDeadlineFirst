/**
 * Implement scheduleEDF().
 * @author Jainish Mehta
 */

#include "sched.h"
//Most efficient way is probably a priority queue, which I guess this code mimics in terms of data structures. I
//also included the code that I attempted before, extremely complex and hard to debug!
/**
 * Create task execution order.  Assigns task numbers to slots.  Uses
 * non-preemptive EDF scheduling.
 * @param slots array representing task execution order
 * @param nSlot number of entries in slots
 * @param tasks array of tasks (the taskset)
 * @param nTask size of the taskset
 * @param LCM length of the fixed schedule
 * @return true on success, false on failure
 */
bool scheduleEDF(int slots[], int nSlot, task_t tasks[], int nTask, int LCM)
{
    //Check for U is <= 1
    int sum = 0;
    for (int i = 0; i < nTask; i++){
        sum += ((tasks[i].C) / (tasks[i].T));
        if (sum > 1 || sum < 0)
        {
            return false;
        }
    }
    //check nSlots is correct
    int nSlotsum = 0;
    for (int i = 0; i< nTask; i++) {
       nSlotsum+= LCM/tasks[i].T;
    }
		if (!c_assert(nSlotsum == nSlot))
        return false;
		
    //Check to see if LCM truly goes into each task deadline
    for (int i = 0;i< nTask; i++) {
        if (!c_assert(LCM % tasks[i].T == 0))
            return false;
    }

    int peaks[nSlot];
    int peakcounter = 0;
    //List of all peaks, don't need this code anymore
    for (int j = 0; j < nTask; j++) {
        int multiple = 0;
        while ((multiple * tasks[j].T) < LCM) {
            peaks[peakcounter] = (multiple * tasks[j].T);
            multiple++;
            peakcounter++;
        }
    }
    //Sort peaks
    int placeholder;
    for (int f = 0; f < peakcounter; f++)
    {
        for (int hehe = f + 1; hehe < peakcounter; ++hehe)
        {
            if (peaks[f] > peaks[hehe])
            {
                placeholder = peakcounter;
                peaks[f] = peaks[hehe];
                peaks[hehe] = placeholder;
            }
        }
    }

 		int deadlines[nTask];
		//Populate the deadlines index
    for (int i = 0; i < nTask; i++) {
        deadlines[i] = tasks[i].T;
    }
    int slotindex = 0;
    int remaining = 0;
    int index = 0;
		//through until we reach LCM, I just realized that it says assume no idle task
    for (int counter = 0; counter < LCM; counter++){
		for (int i = 0; i < nTask; i++){
							//If a new task is raised, updating deadlines array to reflect this
                if (counter % tasks[i].T == 0)
                    deadlines[i] = counter + tasks[i].T;
            }
					//Incase a task is executing, don't interrupt
						if (remaining > 0)
            remaining--;
        else{
					//Maximum number that unit32_t will go, will get overwritten right away
            uint32_t min = 2147483647;
            index = -1;
            for (int i = 0; i < nTask; i++){
                //find earliest deadline
                if (deadlines[i] < min && deadlines[i] >= 0) {
                    index = i;
                    min = deadlines[i];
                }
                else{
									//If we reach smallest deadline, lower numbered task starts first whne there is a tie/choice
                    if (deadlines[i] == min) {
                        if (tasks[i].C > tasks[index].C) {
                            index = i;
                        }
                    }
                }
            }
            if (index >= 0) {
                slots[slotindex] = index; //adds task number to array
                slotindex++;
                //Slot index has recorded the task, deadline is invalid, need to get it renewed. Don't think this is important as much because of no ideal 
                deadlines[index] = -1;
								//Allocate space for task in future time, -1 so we don't have any remaining, time increases in the loop
                remaining = tasks[index].C - 1;
            }
        }
    }
    return true;
} 
