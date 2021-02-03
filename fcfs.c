//
// Created by alimgh on 2/3/21.
//
#include "fcfs.h"

void initializeCPUBurst() {

    int i, lidx;
    lidx = (ready_q_start + ready_q_size) % LIST_SIZE;
    for(i=ready_q_start; i<lidx; i=(i+1)%LIST_SIZE)
        ready_q[i].p_cpu_burst_time = ready_q[i].p_duration;

}

void scheduling() {}
