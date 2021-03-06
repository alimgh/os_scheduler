//
// Created by alimgh on 2/3/21.
//

#ifndef SCHEDULER_MULTILEVEL_H
#define SCHEDULER_MULTILEVEL_H

#include "scheduler.h"
extern sem_t    p1_mutex;
extern sem_t    p2_mutex;
extern sem_t    p3_mutex;

extern process  p1_q    [LIST_SIZE];
extern process  p2_q    [LIST_SIZE];
extern process  p3_q    [LIST_SIZE];

extern int      p1_q_start;
extern int      p1_q_size;
extern int      p2_q_start;
extern int      p2_q_size;
extern int      p3_q_start;
extern int      p3_q_size;

/*
 * p1: SJF
 * p2: FCFS
 * p3: RR
 * */

void    initializeCPUBurst  (); // scheduler
process p1RunProcess        ();          // p1 to running
void    p1UnWaitProcess     (process p); // waiting to p1
void    p1ReadyProcesses    (process p); // running to p1
void    p1Terminate         (process p);
process p2RunProcess        ();          // p2 to running
void    p2UnWaitProcess     (process p); // waiting to p2
void    p2ReadyProcesses    (process p); // running to p2
void    p2Terminate         (process p);
process p3RunProcess        ();          // p3 to running
void    p3UnWaitProcess     (process p); // waiting to p3
void    p3ReadyProcesses    (process p); // running to p3
void    p3Terminate         (process p);

#endif //SCHEDULER_MULTILEVEL_H
