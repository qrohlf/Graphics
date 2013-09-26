#include <qdmlib.h>
#include <FPT.h>

int main(int argc, char *argv[]) {
    double a[] = {0, 1};
    double b[] = {1, 0};
    printf("Dot %f\n", dot(a, b));
}