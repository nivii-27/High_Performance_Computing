#include <stdio.h>
#include <omp.h>

double f(double x) {
    return x * x;
}

int main() {
    double a, b, h;
    int n;
    double result1, result2, result3;
    printf("\nEnter lower limit (a): ");
    scanf("%lf", &a);
    printf("Enter upper limit (b): ");
    scanf("%lf", &b);
    printf("Enter number of trapezoids (n): ");
    scanf("%d", &n);

    h = (b - a) / n;
    double local_sum1 = 0.0;
    double x, sum1;

    #pragma omp parallel for reduction(+:local_sum1) private(x)
    for (int i = 1; i < n; i++) {
        x = a + i * h;
        local_sum1 += f(x);
    }
    sum1 = local_sum1;
    result1 = ((f(a) + f(b)) / 2.0 + sum1) * h;
    result2 = (f(a) + f(b)) / 2.0;

    #pragma omp parallel
    {
        double local_sum2 = 0.0;
        #pragma omp for
        for (int i = 1; i < n; i++) {
            local_sum2 += f(a + i * h);
        }
        #pragma omp critical
        {
            result2 += local_sum2;
        }
    }
    result2 = result2 * h;
    result3 = (f(a) + f(b)) / 2.0;

    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int num_threads = omp_get_num_threads();
        double local_sum3 = 0.0;
        for (int i = id + 1; i < n; i += num_threads) {
            local_sum3 += f(a + i * h);
        }

        #pragma omp atomic
        result3 += local_sum3;
    }
    result3 = result3 * h;
    printf("\nResult 1 (Reduction): %f", result1);
    printf("\nResult 2 (Critical):  %f", result2);
    printf("\nResult 3 (Manual):    %f\n\n", result3);

    return 0;
}