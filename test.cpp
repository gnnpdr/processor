#include <stdio.h>
void print_binary_int (int a);

int main(void)
{
    char bin_str[8 * sizeof(int) + 1];
    int n;
    puts("print\n");
    scanf("%d", &n);


    print_binary_int (n);
    
    printf("\n");

    n |= 1 << (sizeof(int)*8 - 1);
    print_binary_int (n);
    
    printf("\n");

    return 0;
}

void print_binary_int (int a)
{
    int size = sizeof(int)*8;
    int bin_str[size + 1] = {}; 

    for (int i = (sizeof(int)*8 - 1); i >= 0; i--)
    {
        bin_str[i] = (1 & a);
        a >>= 1;
    }

    int k = 0;
    while (k < size)
    {
        printf("%d", bin_str[k]);
        k++;
        if (k % 4 == 0)
            printf(" ");
    }
}