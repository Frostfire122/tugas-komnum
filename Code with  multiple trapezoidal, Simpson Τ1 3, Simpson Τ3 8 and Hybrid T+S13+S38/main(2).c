#include <stdio.h>
#include <math.h>

#define N 25   // 25 points = 24 segments

int main() {
    double P[N];
    double h = 1.0;

    printf("Enter %d load values (P0 to P24):\n", N);

    for (int i = 0; i < N; i++) {
        scanf("%lf", &P[i]);
    }

    /* =========================
       Reference Energy
       ========================= */
    double E_ref = 0.0;

    for (int i = 0; i < N - 1; i++) {
        E_ref += P[i];
    }

    /* =========================
       Multiple Trapezoidal
       ========================= */
    double E_trap = P[0] + P[N - 1];

    for (int i = 1; i < N - 1; i++) {
        E_trap += 2.0 * P[i];
    }

    E_trap *= h / 2.0;

    /* =========================
       Multiple Simpson 1/3
       ========================= */
    double odd_sum = 0.0;
    double even_sum = 0.0;

    for (int i = 1; i < N - 1; i++) {
        if (i % 2 == 1)
            odd_sum += P[i];
        else
            even_sum += P[i];
    }

    double E_s13 =
        (h / 3.0) *
        (P[0] + P[N - 1] + 4.0 * odd_sum + 2.0 * even_sum);

    /* =========================
       Multiple Simpson 3/8
       ========================= */
    double coeff_sum = 0.0;

    for (int i = 1; i < N - 1; i++) {
        if (i % 3 == 0)
            coeff_sum += 2.0 * P[i];
        else
            coeff_sum += 3.0 * P[i];
    }

    double E_s38 =
        (3.0 * h / 8.0) *
        (P[0] + coeff_sum + P[N - 1]);

    /* =========================
       Hybrid Method
       Trap(0-6)
       + S13(6-18)
       + S38(18-24)
       ========================= */

    /* Part 1: Trapezoidal (0-6) */
    double E_part1 =
        (h / 2.0) *
        (
            P[0]
            + 2.0 * (P[1] + P[2] + P[3] + P[4] + P[5])
            + P[6]
        );

    /* Part 2: Simpson 1/3 (6-18) */
    double odd_s13_h = 0.0;
    double even_s13_h = 0.0;

    for (int i = 7; i < 18; i++) {

        if ((i - 6) % 2 == 1)
            odd_s13_h += P[i];
        else
            even_s13_h += P[i];
    }

    double E_part2 =
        (h / 3.0) *
        (
            P[6]
            + P[18]
            + 4.0 * odd_s13_h
            + 2.0 * even_s13_h
        );

    /* Part 3: Simpson 3/8 (18-24) */
    double coeff_s38_h = 0.0;

    for (int i = 19; i < 24; i++) {

        int local = i - 18;

        if (local % 3 == 0)
            coeff_s38_h += 2.0 * P[i];
        else
            coeff_s38_h += 3.0 * P[i];
    }

    double E_part3 =
        (3.0 * h / 8.0) *
        (
            P[18]
            + coeff_s38_h
            + P[24]
        );

    double E_hybrid =
        E_part1 + E_part2 + E_part3;

    /* =========================
       Relative Errors
       ========================= */
    double err_trap =
        fabs(E_ref - E_trap) / E_ref * 100.0;

    double err_s13 =
        fabs(E_ref - E_s13) / E_ref * 100.0;

    double err_s38 =
        fabs(E_ref - E_s38) / E_ref * 100.0;

    double err_hybrid =
        fabs(E_ref - E_hybrid) / E_ref * 100.0;

    /* =========================
       Results
       ========================= */
    printf("\n===== ENERGY RESULTS =====\n");
    printf("Reference           = %.2f MWh\n", E_ref);
    printf("Trapezoidal         = %.2f MWh\n", E_trap);
    printf("Simpson 1/3         = %.2f MWh\n", E_s13);
    printf("Simpson 3/8         = %.2f MWh\n", E_s38);
    printf("Hybrid T+S13+S38    = %.2f MWh\n", E_hybrid);

    printf("\n===== RELATIVE ERRORS =====\n");
    printf("Trapezoidal         = %.6f %%\n", err_trap);
    printf("Simpson 1/3         = %.6f %%\n", err_s13);
    printf("Simpson 3/8         = %.6f %%\n", err_s38);
    printf("Hybrid T+S13+S38    = %.6f %%\n", err_hybrid);

    return 0;
}