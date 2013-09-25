#include <FPT.h>

int click_polygon(double*, double*);
double perimeter(double*, double*, int);
int read_polygon_from_file(FILE *h, double *x, double *y);
void my_fill_polygon(double *x, double *y, int n);
void print_x_y(double *x, double *y, int n);
void quick_sort(double *x, double *y, int n);
void sort(double *x, int n);
double point_slope(double x, double y, double a, double b, double intersection_x);

int main(int argc, char **argv)
{
	double xp[100], yp[100], xq[50], yq[50];
	int np, nq;
	int i;
	FILE *f, *g;

	if(argc == 3) {
		f = fopen(argv[1], "r");
		g = fopen(argv[2], "r");
		np = read_polygon_from_file(f, xp, yp);
		nq = read_polygon_from_file(f, xq, yq);
	}
	else if(argc == 1) {
		np = click_polygon(xp, yp);
		nq = click_polygon(xq, yq);
	}
	else {
		printf("Please provide either zero or two command line arguments.\n");
	}

	G_init_graphics(600, 600);


	if(perimeter(xp, yp, np) > perimeter(xq, yq, nq)) {
		G_rgb(1, 0, 0);
		/* G_fill_polygon(xp, yp, np); */
		my_fill_polygon(xp, yp, np);
	} else {
		G_rgb(0, 1, 0);
		G_fill_polygon(xq, yq, nq);
	}

	G_wait_key();

}

int read_polygon_from_file(FILE *h, double *x, double *y)
{
	int n, i;
	fscanf(h, "%d", &n);
	for(i = 0; i < n; i++)
	{
		fscanf(h, "%lf %lf", &x[i], &y[i]);
	}
	return n;
}

void sort(double *x, int n){
	int i, min, k;
	double temp;

	for(i = 0; i < n; i++){
		min = i;
		for(k = i + 1; k < n; k++){
			if(x[k] < x[min]){
				min = k;
			}
			temp = x[i];
			x[i] = x[min];
			x[min] = temp;
		}
	}
}

int click_polygon(double* x, double* y){
	int i = 0;
	double p[2];

	G_fill_rectangle(580, 580, 600, 600);

	while(1){
		G_wait_click(p);
		if((p[0] >= 580) && (p[1] >= 580)){
			break;
		}
		G_fill_circle(p[0], p[1], 1);
		x[i] = p[0];
		y[i] = p[1];
		i++;
	}
	return i;
}

double perimeter(double *x, double *y, int n) {
	int i, j;
	double perim = 0;

	for(i = 0; i < n; i++){
		j = (i + 1) % n;
		perim += sqrt(pow(x[i] - x[j], 2) + pow(y[i] - y[j], 2));
	}
	return perim;
}

void my_fill_polygon(double *x, double *y, int n){
	/* int row, column, i, k, h; */
	/* double intersections[n]; */
	/* double j; */
	/* int p = 0; */
	/* int minY = 400; */
	/* int maxY = 0; */
	/* quick_sort(x, y, n); */

	/* for(k=0; k<n; k++){ */
	/* 	if(y[k] < minY){ */
	/* 		minY = y[k]; */
	/* 	} */
	/* 	if(y[k] > maxY) { */
	/* 		maxY = y[k]; */
	/* 	} */
	/* } */


	/* for(j= minY + .01 ; j < maxY; j++){ */
	/* 	p = 0; */
	/* 	G_rgb(1,0,0); */
	/* 	for(i=0; i < n; i++){ */
	/* 		if(((j <= y[i]) && (j > y[(i+1) % n])) || */
	/* 				((j > y[i]) && (j <= y[(i+1) % n]))){ */
	/* 			for (i = 0; i < n; i++){ */
	/* 				intersections[p] = ((x[(i+1)%n] - x[i])/(y[(i+1)%n]-y[i])*(j-y[i])) + x[i]; */
	/* 				p++; */
	/* 			} */
	/* 		} */
	/* 	} */
	/* } */

	/* sort(intersections, p); */
	/* for(h=0; h < p; h+=2){ */
	/* 	G_line(intersections[h], j, intersections[h+1], j); */
	/* } */

	int minY = 400;
	int maxY = 0;
	int h, i,  k;

	double j ;

	double intersections[n], a[2];
	double slope, b;
	int p = 0;
	double color = 0.9;

	/* Find min y and max y. */
	for(k=0; k<n; k++){
		if(y[k] < minY){
			minY = y[k];
		}
		if(y[k] > maxY) {
			maxY = y[k];
		}
	}

	/* Go up, pixel by pixel, making horizontal lines. */
	for(j = minY+.01; j < maxY; j++) {

		p=0;
		color-= 0.005;
		G_rgb(color,0.7,0.9);

		/* Go through each line connecting the polygon. */
		for(i =0; i<n; i++) {

			/* Check if horizontal line's y value is between this and
			   the next point's y value. Offset by 0.001 to avoid checking vertices. */

			if(((j <= y[i])&&(j > y[(i+1)%n])) ||
					((j > y[i])&&(j <= y[(i+1)%n]))){

				/* Find intersection, and save it to intersections[p] */
				/*	slope = (y[i]-y[(i+1)%n]) / (x[i]-x[(i+1)%n]);
					b = y[i] - (slope * x[i]);
					intersections[p] = (j - b) / slope ;*/

				intersections[p] = ((x[(i+1)%n] - x[i])/(y[(i+1)%n]-y[i])*(j-y[i])) + x[i];


				/*printf("j: %d, i: %d, slope: %lf, b: %lf, intersections[%d]: %lf. \n",
				  j, i, slope, b,p,intersections[p]); */
				p++;
			}
		}

		sort(intersections, p);

		/* Draw horizontal lines. */
		for(h=0; h < p; h+=2) {
			G_line(intersections[h], j, intersections[h+1], j);
		}
	}

}

double point_slope(double x, double y, double a, double b, double intersection_x){
	double slope, intersection_y;
	int i;

	slope = (y - b) / (x - a);
	intersection_y = slope * (intersection_x - x) + y;

	return intersection_y;
}


void print_x_y(double *x, double *y, int n){
	int i;

	for(i = 0; i < n; i++){
		printf("x: %lf, y: %lf\n", x[i], y[i]);
	}
}

void quick_sort(double *x, double *y, int n){
	int i, j, min;
	double temp;

	for(i = 0; i < n; i++){
		min = i;
		for(j = i + 1; j < n; j++){
			if(x[j] < x[min]){
				min = j;
			}
		}
		temp = x[i];
		x[i] = x[min];
		x[min] = temp;

		temp = y[i];
		y[i] = y[min];
		y[min] = temp;

	}
}
