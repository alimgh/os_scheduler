//
// Created by alimgh on 1/27/21.
//

#ifndef SCHEDULER_SCHEDULER_H
#define SCHEDULER_SCHEDULER_H
#define LIST_SIZE 10
#include <semaphore.h>

//typedef struct {
//    char r_name;
//    int r_count;
//} resource;

typedef struct {
    char pt_name;
    int pt_priority;
    int pt_resources[3]; // max need
    // 0: A
    // 1: B
    // 2: C

} p_type;

typedef struct {
    int pid;
    char p_name[5];
    p_type type;
    int p_duration, p_total_run;
    int p_cpu_burst_time, p_running_time; // simulator
    int p_completed;
    int p_state;
    // 1: ready
    // 2: running
    // 3: waiting

//    int p_resources[3];
} process;

extern sem_t ready_mutex;
extern sem_t waiting_mutex;
extern sem_t resources_mutex;

extern p_type typeX, typeY, typeZ;
//extern resource resources [10];
extern int      resources [3];
// 0: A
// 1: B
// 2: C

extern process  idle;
extern process  ready_q   [LIST_SIZE];
extern process  waiting_q [LIST_SIZE];
//extern process  running_q [4];
//extern int      cpu_count;
extern int      ready_q_start;
extern int      ready_q_size;
//extern int      waiting_q_start;
extern int      waiting_q_size;
//extern int      running_q_start;
//extern int      running_q_size;
extern int      s_time; // simulator
extern int      p_count;

void    initialize          ();
void    initializeProcesses (); // simulator
void    scheduling          (); // scheduler
process runProcess          ();          // ready to running
void    unWaitProcess       (process p); // waiting to ready
void    waitProcess         (process p); // running to waiting
void    readyProcesses      (process p); // running to ready
void    terminate           (process p);
//int     deadlockAvoidance   (process p);
//int     deadlockAvoidance   (process p, resource ** r);

#endif //SCHEDULER_SCHEDULER_H
