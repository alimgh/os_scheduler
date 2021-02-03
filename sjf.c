//
// Created by alimgh on 2/3/21.
//

#include "sjf.h"
#include <stdio.h>

void initializeCPUBurst() {

//    sem_wait(&ready_mutex);

    int i, lidx;
    lidx = (ready_q_start + ready_q_size) % LIST_SIZE;
    for(i=ready_q_start; i<lidx; i=(i+1)%LIST_SIZE)
        ready_q[i].p_cpu_burst_time = ready_q[i].p_duration;

//    ready_q[0].p_cpu_burst_time = ready_q[0].p_duration;
//    printf("%d - %d", ready_q[1].p_cpu_burst_time, ready_q[1].p_duration);
//    sem_post(&ready_mutex);
}

void scheduling() {

//    sem_wait(&ready_mutex);
    if (ready_q_size == 0) {
        sem_post(&ready_mutex);
        return;
    }

    int i, j, lidx, sidx, zxc, asd;
    process p_tmp;

//    printf("-> %d\n", ready_q_size);
//    zxc = (ready_q_start + ready_q_size-1) % LIST_SIZE;
//    for (asd=ready_q_start; asd<zxc; asd=(asd+1)%LIST_SIZE) {
//        printf("%s-", ready_q[asd].p_name);
//    }
//    printf("%s\n########\n", ready_q[zxc].p_name);

    lidx = (ready_q_start + ready_q_size - 1) % LIST_SIZE;
    for (i=ready_q_start; i<lidx; i = (i+1)%LIST_SIZE) {
        sidx = i;
        for (j=i+1; j<lidx+1; j = (j+1)%LIST_SIZE) {
            if (ready_q[sidx].p_duration > ready_q[j].p_duration)
                sidx = j;
        }

        p_tmp = ready_q[i];
//        p_tmp.p_name[0] = ready_q[i].p_name[0];
//        p_tmp.p_name[1] = ready_q[i].p_name[1];
//        p_tmp.p_name[2] = ready_q[i].p_name[2];
//        p_tmp.p_name[3] = ready_q[i].p_name[3];
//        p_tmp.p_name[4] = ready_q[i].p_name[4];
//
//        p_tmp.pid = ready_q[i].pid;
//        p_tmp.type = ready_q[i].type;
//        p_tmp.p_duration = ready_q[i].p_duration;
//        p_tmp.p_total_run = ready_q[i].p_total_run;
//        p_tmp.p_running_time = ready_q[i].p_running_time;
//        p_tmp.p_cpu_burst_time = ready_q[i].p_cpu_burst_time;

        ready_q[i] = ready_q[sidx];
//        ready_q[i].p_name[0] = ready_q[sidx].p_name[0];
//        ready_q[i].p_name[1] = ready_q[sidx].p_name[1];
//        ready_q[i].p_name[2] = ready_q[sidx].p_name[2];
//        ready_q[i].p_name[3] = ready_q[sidx].p_name[3];
//        ready_q[i].p_name[4] = ready_q[sidx].p_name[4];
//
//        ready_q[i].pid = ready_q[sidx].pid;
//        ready_q[i].type = ready_q[sidx].type;
//        ready_q[i].p_duration = ready_q[sidx].p_duration;
//        ready_q[i].p_total_run = ready_q[sidx].p_total_run;
//        ready_q[i].p_running_time = ready_q[sidx].p_running_time;
//        ready_q[i].p_cpu_burst_time = ready_q[sidx].p_cpu_burst_time;

        ready_q[sidx] = p_tmp;
//        ready_q[sidx].p_name[0] = p_tmp.p_name[0];
//        ready_q[sidx].p_name[1] = p_tmp.p_name[1];
//        ready_q[sidx].p_name[2] = p_tmp.p_name[2];
//        ready_q[sidx].p_name[3] = p_tmp.p_name[3];
//        ready_q[sidx].p_name[4] = p_tmp.p_name[4];
//
//        ready_q[sidx].pid = p_tmp.pid;
//        ready_q[sidx].type = p_tmp.type;
//        ready_q[sidx].p_duration = p_tmp.p_duration;
//        ready_q[sidx].p_total_run = p_tmp.p_total_run;
//        ready_q[sidx].p_running_time = p_tmp.p_running_time;
//        ready_q[sidx].p_cpu_burst_time = p_tmp.p_cpu_burst_time;

    }

//    zxc = (ready_q_start + ready_q_size-1) % LIST_SIZE;
//    for (asd=ready_q_start; asd<zxc; asd=(asd+1)%LIST_SIZE) {
//        printf("%s-", ready_q[asd].p_name);
//    }
//    printf("%s\n########\n", ready_q[zxc].p_name);

//    sem_post(&ready_mutex);
}
