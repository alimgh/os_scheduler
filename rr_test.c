#include "rr.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

process running_q[4];
int     idle_time[4];

void* cpu(void* idx) {
    int c_time = 0;
//    scheduling();
    process p = runProcess();
    running_q[*(int*)idx] = p;
    idle_time[*(int*)idx] = 0;

    while (p_count) {
        if (s_time > c_time) {
            p.p_running_time += s_time - c_time;
            p.p_total_run    += s_time - c_time;

            if (p.pid == -1) {
                if (ready_q_size > 0) {
//                    scheduling();
                    p = runProcess();
                    running_q[*(int*)idx] = p;
                }
                idle_time[*(int*)idx] += s_time - c_time;
            }
            else if (p.p_running_time >= p.p_cpu_burst_time || p.p_total_run >= p.p_duration) {
                if (p.p_total_run >= p.p_duration){
                    printf("CPU%d: Terminate %s\n", *(int*)idx, p.p_name);
                    terminate(p);
                }
                else
                    readyProcesses(p);

//                scheduling();
                p = runProcess();
                running_q[*(int*)idx] = p;
            }
            c_time = s_time;
        }
    }
    free(idx);
}

int main() {
    pthread_t cpu1, cpu2, cpu3, cpu4;
    int i, lidx;
    int *c1 = malloc(sizeof(*c1));
    *c1 = 0;
    int *c2 = malloc(sizeof(*c2));
    *c2 = 1;
    int *c3 = malloc(sizeof(*c3));
    *c3 = 2;
    int *c4 = malloc(sizeof(*c4));
    *c4 = 3;

    initialize();
    initializeProcesses();
    initializeCPUBurst();

    pthread_create(&cpu1, NULL, cpu, (void *) c1);
    pthread_create(&cpu2, NULL, cpu, (void *) c2);
    pthread_create(&cpu3, NULL, cpu, (void *) c3);
    pthread_create(&cpu4, NULL, cpu, (void *) c4);

    while (p_count) {
        printf("A:%d B:%d C:%d\n\n", resources[0], resources[1], resources[2]);

        printf("ready queue:   ");

        if (ready_q_size) {
            lidx = (ready_q_start + ready_q_size-1) % LIST_SIZE;
            for (i=ready_q_start; i<lidx; i=(i+1)%LIST_SIZE) {
                printf("%s-", ready_q[i].p_name);
            }
            printf("%s", ready_q[lidx].p_name);
        }
        printf("\n\n");

        printf("waiting queue:   ");
        for (i=0; i<waiting_q_size-1; i++) {
            printf("%s-", waiting_q[i].p_name);
        }
        printf("%s\n\n", waiting_q[waiting_q_size-1].p_name);

        for (i=0; i<4; i++) {
            printf("cpu%d: %s\n", i, running_q[i].p_name);
        }
        printf("----------------------\n\n");
        s_time++;
        sleep(1);
    }

    for (i=0; i<4; i++) {
        printf("cpu%d Idle-Time: %d\n", i, idle_time[i]);
    }

    return 0;
}


/* test
8 8 8
5
T1 Z 7
T2 X 4
T3 X 2
T4 Y 6
T5 Y 3

1 1 1
3
T1 Y 3
T2 X 4
T3 Z 2
 * */
