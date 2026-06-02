#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX_DATA 100000
#define RUNS 10000000

double t[MAX_DATA];
double a[MAX_DATA];

int main() {
    int n = 0;
    while (scanf("%lf %lf", &t[n], &a[n]) == 2) {
        n++;
        if (n >= MAX_DATA) {
            break;
        }
    }
    if (n < 3) {
        return 1;
    }

    srand(1375);
    int target_indices[3];
    for (int i = 0; i < 3; i++) {
        target_indices[i] = (n / 4) + (rand() % (n / 2));
    }

    int skips[] = {1, 2, 5, 10, 25, 50};
    int cases = 6;
    double reference_derivatives[3];

    for (int p = 0; p < 3; p++) {
        int idx = target_indices[p];
        double h = t[idx + 1] - t[idx];
        reference_derivatives[p] = (a[idx + 1] - a[idx - 1]) / (2.0 * h);
    }

    for (int c = 0; c < cases; c++) {
        int skip = skips[c];
        double h = 0.02 * skip;
        
        clock_t start = clock();
        volatile double dummy;
        for (int r = 0; r < RUNS; r++) {
            int idx = target_indices[0];
            int prev_idx = idx - skip;
            int next_idx = idx + skip;
            if (prev_idx >= 0 && next_idx < n) {
                dummy = (a[next_idx] - a[prev_idx]) / (2.0 * h);
            } else if (next_idx < n) {
                dummy = (a[next_idx] - a[idx]) / h;
            } else {
                dummy = (a[idx] - a[prev_idx]) / h;
            }
        }
        clock_t end = clock();
        double runtime = (double)(end - start) / CLOCKS_PER_SEC;

        printf("====================================\n");
        printf("Skip = %d | h = %.2f s\n", skip, h);
        printf("Benchmark Runtime (%d runs): %.6f sec\n", RUNS, runtime);
        printf("------------------------------------\n");

        for (int p = 0; p < 3; p++) {
            int idx = target_indices[p];
            double approx_deriv = 0.0;
            int prev_idx = idx - skip;
            int next_idx = idx + skip;

            if (prev_idx >= 0 && next_idx < n) {
                approx_deriv = (a[next_idx] - a[prev_idx]) / (2.0 * h);
            } else if (next_idx < n) {
                approx_deriv = (a[next_idx] - a[idx]) / h;
            } else {
                approx_deriv = (a[idx] - a[prev_idx]) / h;
            }

            double ref = reference_derivatives[p];
            double error = 0.0;
            if (fabs(ref) > 1e-12) {
                error = fabs(ref - approx_deriv) / fabs(ref) * 100.0;
            }

            printf("Point %d (t = %.2f s):\n", p + 1, t[idx]);
            printf("  Calculated f'(x) = %.10lf\n", approx_deriv);
            printf("  Error Approx     = %.6lf %%\n", error);
        }
        printf("====================================\n\n");
    }
    return 0;
}