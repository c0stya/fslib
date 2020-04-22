#include <stdio.h>

// a little example of how to compatify an array

int main() {
    int shift = 0;
    int x [10] = {1,2,0,2,0,2,0,0,5,1};
    int i;

    for (i = 0; i < 10; ++i) {
        if (x[i] == 0)
            ++shift;
        else 
            if (shift != 0)
                x[i-shift] = x[i];
    }
    
    for (i = 0; i < 10; ++i)
        printf("%d ", x[i]);
    printf("\n");

    return 0;
}
