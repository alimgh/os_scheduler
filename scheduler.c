//
// Created by alimgh on 1/27/21.
//
#include <stdio.h>
#include "scheduler.h"

/*
 * creating global variables
 * */

p_type typeX, typeY, typeZ;

sem_t ready_mutex;
sem_t waiting_mutex;
sem_t resources_mutex;

process idle;

process ready_q   [LIST_SIZE];
process waiting_q [LIST_SIZE];
int     resources [3];

int ready_q_start   = 0;
int ready_q_size    = 0;
int waiting_q_size  = 0;
//int cpu_count       = 4;
int s_time          = 0;
int p_count;

/*
 * initializing values like semaphores and process types and Idle process
 * */
void initialize() {
    sem_init(&ready_mutex, 0, 1);
    sem_init(&waiting_mutex, 0, 1);
    sem_init(&resources_mutex, 0, 1);

    typeX.pt_name = 'X';
    typeX.pt_priority = 3;
    typeX.pt_resources[0] = 1;
    typeX.pt_resources[1] = 1;
    typeX.pt_resources[2] = 0;

    typeY.pt_name = 'Y';
    typeY.pt_priority = 2;
    typeY.pt_resources[0] = 0;
    typeY.pt_resources[1] = 1;
    typeY.pt_resources[2] = 1;

    typeZ.pt_name = 'Z';
    typeZ.pt_priority = 1;
    typeZ.pt_resources[0] = 1;
    typeZ.pt_resources[1] = 0;
    typeZ.pt_resources[2] = 1;

    idle.p_name[0] = 'I';
    idle.p_name[1] = 'd';
    idle.p_name[2] = 'l';
    idle.p_name[3] = 'e';
    idle.pid = -1;
    idle.p_duration = 2147483647; // MAX_INT
    idle.p_total_run = 0;
}

/*
 * get Tasks from user
 * */
void initializeProcesses() {
    int t, i;
//    process p;
    char pt;

    scanf("%d", &resources[0]);
    scanf("%d", &resources[1]);
    scanf("%d", &resources[2]);

    scanf("%d", &t);
    p_count = t;
    for (i=0; i<p_count; i++) {
        scanf("%s %s %d", ready_q[i].p_name, &pt, &(ready_q[i].p_duration));

        switch (pt) {
            case 'X':
                ready_q[i].type = typeX;
                break;
            case 'Y':
                ready_q[i].type = typeY;
                break;
            case 'Z':
                ready_q[i].type = typeZ;
        }

        ready_q[i].pid = i;
        ready_q[i].p_running_time = 0;
        ready_q[i].p_total_run = 0;

        ready_q_size++;
    }
}

/*
 * scheduler will return task which should run
 * */
process runProcess() {
//    if (ready_q_size == 0)
//        exit(3);

    sem_wait(&ready_mutex);
    sem_wait(&resources_mutex);

    if (ready_q_size == 0) {
        sem_post(&resources_mutex);
        sem_post(&ready_mutex);
        return idle;
    }

    scheduling();

    int i;
    process* p;

    do {
        p = &ready_q[ready_q_start];
        ready_q_start = (ready_q_start + 1) % LIST_SIZE;
        ready_q_size--;
        if (resources[0] < p->type.pt_resources[0] || resources[1] < p->type.pt_resources[1] ||
            resources[2] < p->type.pt_resources[2])
            waitProcess(*p);

    } while (ready_q_size && (resources[0] < p->type.pt_resources[0] || resources[1] < p->type.pt_resources[1] ||
                              resources[2] < p->type.pt_resources[2]));

    if (resources[0] < p->type.pt_resources[0] || resources[1] < p->type.pt_resources[1] ||
        resources[2] < p->type.pt_resources[2]) {
        sem_post(&resources_mutex);
        sem_post(&ready_mutex);
        return idle;
    }

    for (i=0; i<3; i++)
        resources[i] -= (p->type).pt_resources[i];

    sem_post(&resources_mutex);
    sem_post(&ready_mutex);

    return *p;
}

/*
 * move Process from ready queue to waiting queue
 * */
void unWaitProcess(process p) {
//    if (waiting_q_size == 0)
//        exit(3);

    sem_wait(&ready_mutex);
    sem_wait(&waiting_mutex);

    int i, lidx;
    i = 0;
    while (waiting_q[i].pid != p.pid)
        i++;

    lidx = (ready_q_start + ready_q_size) % LIST_SIZE;
    ready_q_size++;
    ready_q[lidx] = waiting_q[i];

    waiting_q_size--;
    waiting_q[i] = waiting_q[waiting_q_size];

    sem_post(&waiting_mutex);
    sem_post(&ready_mutex);
}

/*
 * move Process from running to waiting
 * */
void waitProcess(process p) {
//    if (waiting_q_size == 10)
//        exit(3);

    sem_wait(&waiting_mutex);

    waiting_q[waiting_q_size] = p;
    waiting_q_size++;

    sem_post(&waiting_mutex);
}

/*
 * move Process from running to ready queue and release it's resources
 * */
void readyProcesses(process p) {
//    if (ready_q_size == 10)
//        exit(3);

    sem_wait(&ready_mutex);
    sem_wait(&resources_mutex);

    ready_q_size++;

    int i, idx;
    process* p_tmp;
    p_type pt_tmp;
    idx = (ready_q_start + ready_q_size - 1) % LIST_SIZE;

    ready_q[idx] = p;
    ready_q[idx].p_running_time = 0;

    for (i=0; i<3; i++)
        resources[i] += p.type.pt_resources[i];

//    for (i=0; i<waiting_q_size; i++) {
//        p_tmp = &waiting_q[0];
//        pt_tmp = p_tmp->type;
//        if (resources[0] >= pt_tmp.pt_resources[0] && resources[1] >= pt_tmp.pt_resources[1] &&
//            resources[2] >= pt_tmp.pt_resources[2])
//            unWaitProcess(*p_tmp);
//    }

    sem_post(&resources_mutex);
    sem_post(&ready_mutex);
}

/*
 * terminate process and release it's resources
 * */
void terminate(process p) {

    sem_wait(&resources_mutex);

    int i;
    process* p_tmp;
    p_type pt_tmp;
    for (i=0; i<3; i++)
        resources[i] += p.type.pt_resources[i];

    for (i=0; i<waiting_q_size; i++) {
        p_tmp = &waiting_q[0];
        pt_tmp = p_tmp->type;
        if (resources[0] >= pt_tmp.pt_resources[0] && resources[1] >= pt_tmp.pt_resources[1] &&
            resources[2] >= pt_tmp.pt_resources[2])
            unWaitProcess(*p_tmp);
    }
    p_count--;

    sem_post(&resources_mutex);
}
