#include <FPT.h>
#define true 1
#define false 0;

int click_polygon(double*, double*);
int read_polygon_from_file(char*, double*, double*);
double perimeter(double*, double*, int);
void My_fill_polygon(double*, double*, int);
int intersects_line(int, int, double*, double*, int);
int vertex(int, int, double*, double*, int);

int main(int argc, char *argv[]) {
	double xp[100], yp[100], xq[50], yq[50];
	int np, nq;

	G_init_graphics(600, 600);
	if (argc == 3) {
		np = read_polygon_from_file(argv[1], xp, yp);
		nq = read_polygon_from_file(argv[2], xq, yq);
	} else {
		G_rgb(1, 0, 0);
		G_fill_rectangle(580, 580, 600, 600);
		np=click_polygon(xp, yp);
		G_rgb(1, 1, 1);
		G_clear();
		G_rgb(1, 0, 0);
		G_fill_rectangle(580, 580, 600, 600);
		nq=click_polygon(xq, yq);
		G_rgb(1, 1, 1);
		G_clear();
	}

	if(perimeter(xp, yp, np) > perimeter(xq, yq, nq)) {
		G_rgb(1, 0, 0);
		My_fill_polygon(xp, yp, np);
	} else {
		G_rgb(0, 1, 0);
		My_fill_polygon(xq, yq, nq);
	}
	G_wait_key();
}

int click_polygon(double* x, double* y) {
	G_rgb(0.8, 0.8, 0.8);
	int i=0;
	double p[] = {0, 0};
	while(true) {
		G_wait_click(p);
		if (p[0] > 580 && p[1] > 580) break;
		G_fill_circle(p[0], p[1], 2);
		x[i] = p[0];
		y[i] = p[1];
		i++;
	}
	printf("Done clicking polygon\n");
	return i;
}

int read_polygon_from_file(char* file, double* x, double* y) {
	FILE *f;
	int vertices;
	f = fopen(file, "r");
	if (f==NULL) {
		printf("can't open file '%s'\n", file);
		exit(1);
	}
	fscanf(f, "%d\n", &vertices);
	printf("%d vertices\n", vertices);
	for (int i=0; i<vertices; i++) {
		fscanf(f, "%lf %lf", &x[i], &y[i]);
	}
	return vertices;
}


double perimeter(double* x, double* y, int n) {
	double perim = 0;
	int j;

	for(int i=0; i<n; i++) {
	  j = (i+1)%n ;
		perim += sqrt(pow(x[i]-x[j], 2)+pow(y[i]-y[j], 2));

		printf("Perim: %f\n", perim);
	}
	printf("---\n");
	return perim;
}

void My_fill_polygon(double* x, double* y, int n) {
	int pixel;
	int paint;

	// for (int i=0; i<n; i++) {
	// 	G_line(x[i], y[i], x[(i+1)%n], y[(i+1)%n]);
	// }
	//Iterate over every vertical row of the canvas
	for (int row=0; row<600; row++) {
		paint = false;
		for (int col=0; col<600; col++) {
			if (intersects_line(col, row, x, y, n) && !vertex(col, row, x, y, n)) {
				paint = true;
			}
			if(paint) G_point(col, row);
		}
	}
}

int intersects_line(int col, int row, double* x, double* y, int n) {
	int intersection_found = false;
	int x1, y1, x2, y2;
	int m, b;
	for (int i=0; i<n; i++) {
		x1 = x[i];
		y1 = y[i];
		x2 = x[(i+1)%n];
		y2 = y[(i+1)%n];
		m = (y2-y1)/(x2-x1);
		b = 0;
		if (row == m*col+b) intersection_found = true;
	}
	return intersection_found;
}

int vertex(int col, int row, double* x, double* y, int n) {
	int is_vertex = false;
	return is_vertex;
}