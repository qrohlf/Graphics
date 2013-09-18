#include <FPT.h>
#include "qlib.h"
#define true 1
#define false 0;

int read_points_from_file(char*, double*, double*);
void draw_shape_from_file(char*, double*, double*, int);

int main(int argc, char *argv[]) {
	double x[100];
	double y[100];
	int n;
	G_init_graphics(600, 600);
	G_rgb(1, 0, 0);
	n = read_points_from_file(argv[1], x, y);
	draw_shape_from_file(argv[1], x, y, n);
	G_wait_key();
}

int read_points_from_file(char* file, double* x, double* y) {
	FILE *f;
	int vertices;
	f = fopen(file, "r");
	if (f==NULL) {
		printf("can't open file '%s'\n", file);
		exit(1);
	}
	
	fscanf(f, "%d\n", &vertices);
	printf("%d points read\n", vertices);
	for (int i=0; i<vertices; i++) {
		fscanf(f, "%lf %lf", &x[i], &y[i]);
	}
	return vertices;
}

void draw_shape_from_file(char* file, double* x, double* y, int vertices) {
	FILE *f;
	FILE *colors;
	double x1[100];
	double y1[100];
	int connections;
	f = fopen(file, "r");
	colors = fopen(file, "r");
	if (f==NULL) {
		printf("can't open file '%s'\n", file);
		exit(1);
	}
	for (int i=0; i<=vertices; i++) {
		fscanf(f, "%*[^\n]\n", NULL);
	}
	fscanf(f, "%d\n", &connections);
	printf("%d shapes in file\n", connections);
	for (int i=0; i<=vertices+connections+1; i++) {
		fscanf(colors, "%*[^\n]\n", NULL);
	}
	//char buffer[100];
	//fgets(buffer, sizeof buffer, colors);
	//printf("current line that colors is pointing to: %s\n", buffer);

	int pointnum;
	for (int i=0; i<connections; i++) {
		fscanf(f, "%d ", &vertices);
		printf("shape %d has %d vertices:\n", i, vertices);
		for (int j=0; j<vertices; j++) {
			fscanf(f, "%d", &pointnum);
			printf("\t vertex %d: %3f, %3f\n", j, x[pointnum], y[pointnum]);
			x1[j]=x[pointnum];
			y1[j]=y[pointnum];
		}
		//char buffer[100];
		//fgets(buffer, sizeof buffer, colors);
		//printf("current line that colors is pointing to: %s\n", buffer);
		double r, g, b;
		fscanf(colors, "%lf %lf %lf", &r, &g, &b);
		printf("color: %3f, %3f, %3f\n", r, g, b);
		G_rgb(r, g, b);
		G_fill_polygon(x1, y1, vertices);
	}
}
	
