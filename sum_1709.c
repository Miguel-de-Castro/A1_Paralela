#include <stdio.h>
#include <string.h>
#include <omp.h>

// #define START  100000
// #define STEP   100000
// #define END   1000000
#define START  10
#define STEP   10
#define END   100

#define DIGITS 1000

void sum(char* output, const long unsigned int d, const long unsigned int n) {
    long unsigned int digit, i, remainder, div, mod;
    long unsigned int digits[d + 11];

    #pragma omp parallel for
    for (digit = 0; digit < d + 11; ++digit) {
        digits[digit] = 0;
    }

    #pragma omp parallel for private(remainder, div, mod) shared(digits)
    for (i = 1; i <= n; ++i) {
        remainder = 1;
        long unsigned int local_digits[d + 11];
        memcpy(local_digits, digits, sizeof(local_digits));
        
        for (digit = 0; digit < d + 11 && remainder; ++digit) {
            div = remainder / i;
            mod = remainder % i;
            local_digits[digit] += div;
            remainder = mod * 10;
        }

        #pragma omp critical
        {
            for (digit = 0; digit < d + 11; ++digit) {
                digits[digit] += local_digits[digit];
            }
        }
    }

    for (i = d + 11 - 1; i > 0; --i) {
        digits[i - 1] += digits[i] / 10;
        digits[i] %= 10;
    }

    if (digits[d + 1] >= 5) {
        ++digits[d];
    }

    for (i = d; i > 0; --i) {
        digits[i - 1] += digits[i] / 10;
        digits[i] %= 10;
    }

    sprintf(output,"%lu.",digits[0]);
    unsigned long int t = strlen(output);
    for (i = 1; i <= d; ++i)
        output[t++] = digits[i]+'0';
    output[t] = '\0';
}

int main() {
    long unsigned int n;
    double start, finish;
    char output[DIGITS + 10]; // extra chars to avoid error
    for (n=START; n<=END; n+=STEP) {
        start = omp_get_wtime();
        sum(output, DIGITS, n);
        finish = omp_get_wtime();  
        fprintf(stdout,"%s\n",output);
        fprintf(stderr,"%lu %lf\n",n,finish-start);
    }
    return 0;
}
