#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

#define NUM_P_PROC 250

static int num_process = 0;
static int num_throws = 0;
static double radius = 1.0;

typedef unsigned long res_t;

typedef struct {
    pid_t pid;
    int res;
} RES;

typedef struct {
    char radius[16];
    char processes[16];
    char throws[16];
} PAR;

void analyze_options(int argc, char *argv[]) {
    int c;
    opterr = 0;
    while ((c = getopt(argc, argv, "r:p:t:")) != -1) {
        switch (c) {
            case 'r':
                radius = atof(optarg);
                break;
            case 'p':
                num_process = atoi(optarg);
                break;
            case 't':
                num_throws = atoi(optarg);
                break;
            case '?':
                if ((optopt == 'r') || (optopt == 'p') || (optopt == 't')) {
                    fprintf(stderr, "Warning: option '-%c' requires an argument\n", optopt);
                } else {
                    fprintf(stderr, "Warning: unknown option '-%c'\n", optopt);
                }
                break;
            default:
                exit(EXIT_FAILURE);
        }
    }
}


int validate(PAR *par) {
    printf("Flags value:\n");
    printf("\tr = %g,  processes = %d,  throws = %d\n", radius, num_process, num_throws);

    if (radius <= 0) radius = 1.0;
    if (num_throws < 0) num_throws = 0;
    if (num_process < 0) num_process = 0;

    sprintf(par->radius, "%g", radius);
    sprintf(par->throws, "%d", NUM_P_PROC);
    if (num_process > 0) {
        sprintf(par->processes, "%d", num_process);
    } else {
        num_throws = (num_throws > 0) ? num_throws : NUM_P_PROC;
        sprintf(par->processes, "%d", num_throws / NUM_P_PROC +
                                      ((num_throws % NUM_P_PROC) ? 1 : 0));
    }


    printf("Score parameters\n");
    printf("Radius: %s\n", par->radius);
    printf("Processes: %s\n", par->processes);
    printf("Throws: %s\n", par->throws);

    return 0;
}


static void create_proc(RES *arr, int num, const PAR *p) {
    int i;
    pid_t pid;

    for (i = 0; i < num; i++) {
        usleep(1);
        pid = fork();
        if (pid < 0) {
            fprintf(stderr, "%s %d\n", "ERROR: Cannot create process", i);
            exit(EXIT_FAILURE);
        } else if (pid > 0) {
            arr[i].pid = pid;
        } else {
            execl("./child.out", "./child.out", p->radius, p->throws, NULL);
            fprintf(stderr, "%s%ld\n", "ERROR: Cannot execute program in ", (long) getpid());
            exit(EXIT_FAILURE);
        }
    }
}

static void wait_proc(RES *arr, int num) {
    int i, status;
    pid_t pid;

    while ((pid = wait(&status)) != -1) {
        for (i = 0; i < num; i++) {
            if (arr[i].pid == pid) {
                if (WIFEXITED(status)) {
                    arr[i].res = WEXITSTATUS(status);
                } else {
                    arr[i].res = -1;
                }
            }
        }
    }
}


static double score_proc(RES *arr, int num, const PAR *p) {
    int i;
    res_t count_all, count_in;
    double res;

    count_all = 0;
    count_in = 0;
    for (i = 0; i < num; i++) {
        if (arr[i].res > 0) {
            count_in += arr[i].res;
            count_all += 1;
        }
    }
    count_all *= atoi(p->throws);
    res = 4.0 * count_in / count_all;

    return res;
}


double score(const PAR *p) {
    RES *arr = NULL;
    int num;
    double res;

    num = atoi(p->processes);
    arr = (RES *) calloc(num, sizeof(RES));
    if (!arr) {
        fprintf(stderr, "%s\n", "ERROR: Cannot allocate memory\n");
        exit(EXIT_FAILURE);
    }

    create_proc(arr, num, p);
    wait_proc(arr, num);
    res = score_proc(arr, num, p);

    free(arr);

    return res;
}

int main(int argc, char *argv[]) {

    PAR params;
    double res;

    analyze_options(argc, argv);
    validate(&params);
    res = score(&params);

    printf("Approximate PI: %g\n", res);

    return EXIT_SUCCESS;
}
