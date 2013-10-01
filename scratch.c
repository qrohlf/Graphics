#include <qdmlib.h>
#include <FPT.h>

int main(int argc, char *argv[]) {
    double x[] = {1, 3, 2, 5, 5, 7, 5, 6};
    sort_asc(x, 8);
    for (int i = 0; i < 8; i++)
    {
        printf("%f ", x[i]);
    }
}