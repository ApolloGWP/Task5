#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

long long mtime() {
    struct timeval t;

    gettimeofday(&t, NULL);
    long long mt = (long long) t.tv_sec * 1000 + t.tv_usec / 1000;
    return mt;
}

double rnd(double left, double right) {
    return left + (right - left) * ((double) rand() / RAND_MAX);
}

int is_in(double r, double x, double y) {
    return x * x + y * y <= r * r;
}

int simulate(double r, int n) {
    int i, num;
    double x, y;

    srand(mtime());
    num = 0;

    for (i = 0; i < n; ++i) {
        x = rnd(-r, r);
        y = rnd(-r, r);

        if (is_in(r, x, y)) num += 1;
    }

    return num;
}

int main(int argc, char const *argv[]) {

    double r;
    int n;

    if (argc != 3) {
        fprintf(stderr, "Error: %s\n", "wrong number of arguments");
    }

    r = atof(argv[1]);
    n = atoi(argv[2]);

    return simulate(r, n);
}
