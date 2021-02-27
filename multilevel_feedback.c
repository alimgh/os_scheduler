//
// Created by alimgh on 2/3/21.
//

#include "multilevel_feedback.h"

#define QUANTUM1 6
#define QUANTUM2 8

sem_t   p1_mutex;
sem_t   p2_mutex;
sem_t   p3_mutex;

process p1_q      [LIST_SIZE];
process p2_q      [LIST_SIZE];
process p3_q      [LIST_SIZE];

int     p1_q_start  = 0;
int     p1_q_size   = 0;
int     p2_q_start  = 0;
int     p2_q_size   = 0;
int     p3_q_start  = 0;
int     p3_q_size   = 0;

void initializeCPUBurst() {
    sem_init(&p1_mutex, 0, 1);
    sem_init(&p2_mutex, 0, 1);
    sem_init(&p3_mutex, 0, 1);

    int i, lidx, pi1=0;
    lidx = (ready_q_start + ready_q_size) % LIST_SIZE;
    for(i=ready_q_start; i<lidx; i=(i+1)%LIST_SIZE) {
        ready_q[i].p_cpu_burst_time = QUANTUM1;
    }

    lidx = (ready_q_start + ready_q_size) % LIST_SIZE;
    for(i=ready_q_start; i<lidx; i=(i+1)%LIST_SIZE) {
        p1_q[pi1] = ready_q[i];
        p1_q[pi1].p_level = 1;
        pi1++;
        p1_q_size++;
    }
}

void scheduling() {}


void p1Scheduling() {

    if (p1_q_size == 0) {
        return;
    }

    int i, j, lidx, sidx;
    process p_tmp;

    lidx = (p1_q_start + p1_q_size - 1) % LIST_SIZE;
    for (i=p1_q_start; i<lidx; i = (i+1)%LIST_SIZE) {
        sidx = i;
        for (j=i+1; j<lidx+1; j = (j+1)%LIST_SIZE) {
            if (p1_q[sidx].type.pt_priority > p1_q[j].type.pt_priority)
                sidx = j;
        }

        p_tmp = p1_q[i];
        p1_q[i] = p1_q[sidx];
        p1_q[sidx] = p_tmp;
    }
}

void p2Scheduling() {

    if (p2_q_size == 0) {
        return;
    }

    int i, j, lidx, sidx;
    process p_tmp;

    lidx = (p2_q_start + p2_q_size - 1) % LIST_SIZE;
    for (i=p2_q_start; i<lidx; i = (i+1)%LIST_SIZE) {
        sidx = i;
        for (j=i+1; j<lidx+1; j = (j+1)%LIST_SIZE) {
            if (p2_q[sidx].type.pt_priority > p2_q[j].type.pt_priority)
                sidx = j;
        }

        p_tmp = p2_q[i];
        p2_q[i] = p2_q[sidx];
        p2_q[sidx] = p_tmp;
    }
}

void p3Scheduling() {}


process p1RunProcess() {
//    if (ready_q_size == 0)
//        exit(3);

    sem_wait(&p1_mutex);
    sem_wait(&resources_mutex);

    p1Scheduling();

    if (p1_q_size == 0) {
        sem_post(&resources_mutex);
        sem_post(&p1_mutex);
        return idle;
    }

    int i;
    process* p;

    do {
        p = &p1_q[p1_q_start];
        p1_q_start = (p1_q_start + 1) % LIST_SIZE;
        p1_q_size--;
        if (resources[0] < p->type.pt_resources[0] || resources[1] < p->type.pt_resources[1] ||
            resources[2] < p->type.pt_resources[2])
            waitProcess(*p);

    } while (p1_q_size && (resources[0] < p->type.pt_resources[0] || resources[1] < p->type.pt_resources[1] ||
                           resources[2] < p->type.pt_resources[2]));

    if (resources[0] < p->type.pt_resources[0] || resources[1] < p->type.pt_resources[1] ||
        resources[2] < p->type.pt_resources[2]) {
        sem_post(&resources_mutex);
        sem_post(&p1_mutex);
        return idle;
    }

    for (i=0; i<3; i++)
        resources[i] -= (p->type).pt_resources[i];

    sem_post(&resources_mutex);
    sem_post(&p1_mutex);

    return *p;
}

void p1UnWaitProcess(process p) {
//    if (waiting_q_size == 0)
//        exit(3);

    sem_wait(&p1_mutex);
    sem_wait(&waiting_mutex);

    int i, lidx;
    i = 0;
    while (waiting_q[i].pid != p.pid)
        i++;

    lidx = (p1_q_start + p1_q_size) % LIST_SIZE;
    p1_q_size++;
    p1_q[lidx] = waiting_q[i];

    waiting_q_size--;
    waiting_q[i] = waiting_q[waiting_q_size];

    sem_post(&waiting_mutex);
    sem_post(&p1_mutex);
}

void p1ReadyProcesses(process p) {
//    if (ready_q_size == 10)
//        exit(3);

    sem_wait(&p2_mutex);
    sem_wait(&resources_mutex);

    p2_q_size++;

    int i, idx;
    idx = (p2_q_start + p2_q_size - 1) % LIST_SIZE;

    p2_q[idx] = p;
    p2_q[idx].p_running_time = 0;
    p2_q[idx].p_cpu_burst_time = QUANTUM2;
    p2_q[idx].p_level = 2;

    for (i=0; i<3; i++)
        resources[i] += p.type.pt_resources[i];

    sem_post(&resources_mutex);
    sem_post(&p2_mutex);
}


process p2RunProcess() {
//    if (ready_q_size == 0)
//        exit(3);

    sem_wait(&p2_mutex);
    sem_wait(&resources_mutex);

    p2Scheduling();

    if (p2_q_size == 0) {
        sem_post(&resources_mutex);
        sem_post(&p2_mutex);
        return idle;
    }

    int i;
    process* p;

    do {
        p = &p2_q[p2_q_start];
        p2_q_start = (p2_q_start + 1) % LIST_SIZE;
        p2_q_size--;
        if (resources[0] < p->type.pt_resources[0] || resources[1] < p->type.pt_resources[1] ||
            resources[2] < p->type.pt_resources[2])
            waitProcess(*p);

    } while (p2_q_size && (resources[0] < p->type.pt_resources[0] || resources[1] < p->type.pt_resources[1] ||
                           resources[2] < p->type.pt_resources[2]));

    if (resources[0] < p->type.pt_resources[0] || resources[1] < p->type.pt_resources[1] ||
        resources[2] < p->type.pt_resources[2]) {
        sem_post(&resources_mutex);
        sem_post(&p2_mutex);
        return idle;
    }

    for (i=0; i<3; i++)
        resources[i] -= (p->type).pt_resources[i];

    sem_post(&resources_mutex);
    sem_post(&p2_mutex);

    return *p;
}

void p2UnWaitProcess(process p) {
//    if (waiting_q_size == 0)
//        exit(3);

    sem_wait(&p2_mutex);
    sem_wait(&waiting_mutex);

    int i, lidx;
    i = 0;
    while (waiting_q[i].pid != p.pid)
        i++;

    lidx = (p2_q_start + p2_q_size) % LIST_SIZE;
    p2_q_size++;
    p2_q[lidx] = waiting_q[i];

    waiting_q_size--;
    waiting_q[i] = waiting_q[waiting_q_size];

    sem_post(&waiting_mutex);
    sem_post(&p2_mutex);
}

void p2ReadyProcesses(process p) {
//    if (ready_q_size == 10)
//        exit(3);

    sem_wait(&p3_mutex);
    sem_wait(&resources_mutex);

    p3_q_size++;

    int i, idx;
    idx = (p3_q_start + p3_q_size - 1) % LIST_SIZE;

    p3_q[idx] = p;
    p3_q[idx].p_cpu_burst_time = p3_q[idx].p_duration - p3_q[idx].p_total_run;
    p3_q[idx].p_running_time = 0;
    p3_q[idx].p_level = 3;

    for (i=0; i<3; i++)
        resources[i] += p.type.pt_resources[i];

    sem_post(&resources_mutex);
    sem_post(&p3_mutex);
}


process p3RunProcess() {
//    if (ready_q_size == 0)
//        exit(3);

    sem_wait(&p3_mutex);
    sem_wait(&resources_mutex);

    p3Scheduling();

    if (p3_q_size == 0) {
        sem_post(&resources_mutex);
        sem_post(&p3_mutex);
        return idle;
    }

    int i;
    process* p;

    do {
        p = &p3_q[p3_q_start];
        p3_q_start = (p3_q_start + 1) % LIST_SIZE;
        p3_q_size--;
        if (resources[0] < p->type.pt_resources[0] || resources[1] < p->type.pt_resources[1] ||
            resources[2] < p->type.pt_resources[2])
            waitProcess(*p);

    } while (p3_q_size && (resources[0] < p->type.pt_resources[0] || resources[1] < p->type.pt_resources[1] ||
                           resources[2] < p->type.pt_resources[2]));

    if (resources[0] < p->type.pt_resources[0] || resources[1] < p->type.pt_resources[1] ||
        resources[2] < p->type.pt_resources[2]) {
        sem_post(&resources_mutex);
        sem_post(&p3_mutex);
        return idle;
    }

    for (i=0; i<3; i++)
        resources[i] -= (p->type).pt_resources[i];

    sem_post(&resources_mutex);
    sem_post(&p3_mutex);

    return *p;
}

void p3UnWaitProcess(process p) {
//    if (waiting_q_size == 0)
//        exit(3);

    sem_wait(&p3_mutex);
    sem_wait(&waiting_mutex);

    int i, lidx;
    i = 0;
    while (waiting_q[i].pid != p.pid)
        i++;

    lidx = (p3_q_start + p3_q_size) % LIST_SIZE;
    p3_q_size++;
    p3_q[lidx] = waiting_q[i];

    waiting_q_size--;
    waiting_q[i] = waiting_q[waiting_q_size];

    sem_post(&waiting_mutex);
    sem_post(&p3_mutex);
}

void p3ReadyProcesses(process p) {
//    if (ready_q_size == 10)
//        exit(3);

    sem_wait(&p3_mutex);
    sem_wait(&resources_mutex);

    p3_q_size++;

    int i, idx;
    idx = (p3_q_start + p3_q_size - 1) % LIST_SIZE;

    p3_q[idx] = p;
    p3_q[idx].p_running_time = 0;
    p3_q[idx].p_level = 3;

    for (i=0; i<3; i++)
        resources[i] += p.type.pt_resources[i];

    sem_post(&resources_mutex);
    sem_post(&p3_mutex);
}


void mfTerminate(process p) {

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
            resources[2] >= pt_tmp.pt_resources[2]) {
            if (p_tmp->p_level == 1)
                p1UnWaitProcess(*p_tmp);
            else if (p_tmp->p_level == 2)
                p2UnWaitProcess(*p_tmp);
            else
                p3UnWaitProcess(*p_tmp);
        }
    }
    p_count--;

    sem_post(&resources_mutex);
}
