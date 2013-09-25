#include <qmlib.h>
#include <FPT.h>

int main(int argc, char *argv[]) {
    double foo[] = {1, 3, 2, 5, 4};
    printf("Before\n");
    for(int i=0; i<5; i++) {
        printf("%2.0f ", foo[i]);
    }
    sort_asc(foo, 5);
    printf("\nAfter\n");
    for(int i=0; i<5; i++) {
        printf("%2.0f ", foo[i]);
    }
    printf("\n");
}