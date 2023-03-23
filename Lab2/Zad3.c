#include <stdio.h>
#include <gsl/gsl_ieee_utils.h>

int main (void)
{
    float x = 1e-32;

    while(x > 0) {

        x = x/(2.0);

        printf ("x = ");
        gsl_ieee_printf_float(&x);
        printf ("\n");
    }

    return 0;
}

// mantysa przestaje być znormalizowana po przekroczeniu zakresu cechy
