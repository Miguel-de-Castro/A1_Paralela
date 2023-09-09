#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>

#define d 10    // Número de dígitos (Exemplo: 5 => 0,xxxxx)
#define n 1000  // Valor de N (Exemplo: 5 => 1/1 + 1/2 + 1/3 + 1/4 + 1/5)

void HPSParallel(char* output) {
    unsigned long long digits[d + 11];
    memset(digits, 0, sizeof(digits));

    #pragma omp parallel for
    for (int i = 1; i <= n; ++i) {
        unsigned long long remainder = 1;
        for (unsigned long long digit = 0; digit < d + 11 && remainder; ++digit) {
            unsigned long long div = remainder / i;
            unsigned long long mod = remainder % i;

            #pragma omp critical
            {
                digits[digit] += div;
            }

            remainder = mod * 10;
        }
    }

    for (int i = d + 11 - 1; i > 0; --i) {
        digits[i - 1] += digits[i] / 10;
        digits[i] %= 10;
    }
    if (digits[d + 1] >= 5) {
        ++digits[d];
    }

    for (int i = 0; i <= d; ++i) {
        sprintf(output + i, "%llu", digits[i]);
    }
}

int main() {
    // Método Paralelo
    printf("Método Paralelo:\n");
    char outputParallel[d + 11];
    HPSParallel(outputParallel);
    printf("%s\n", outputParallel);

    return 0;
}


 