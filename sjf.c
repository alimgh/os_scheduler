//
// Created by alimgh on 2/3/21.
//

#include "sjf.h"

void initializeCPUBurst() {

    int i, lidx;
    lidx = (ready_q_start + ready_q_size) % LIST_SIZE;
    for(i=ready_q_start; i<lidx; i=(i+1)%LIST_SIZE)
        ready_q[i].p_cpu_burst_time = ready_q[i].p_duration;
}

void scheduling() {

    if (ready_q_size == 0) {
        sem_post(&ready_mutex);
        return;
    }

    int i, j, lidx, sidx;
    process p_tmp;

    lidx = (ready_q_start + ready_q_size - 1) % LIST_SIZE;
    for (i=ready_q_start; i<lidx; i = (i+1)%LIST_SIZE) {
        sidx = i;
        for (j=i+1; j<lidx+1; j = (j+1)%LIST_SIZE) {
            if (ready_q[sidx].p_duration > ready_q[j].p_duration)
                sidx = j;
        }

        p_tmp = ready_q[i];
        ready_q[i] = ready_q[sidx];
        ready_q[sidx] = p_tmp;
    }
}
