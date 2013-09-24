#include <FPT.h>
#include <math.h>
#define true 1
#define false 0;

int click_polygon(double*, double*);
int read_polygon_from_file(char*, double*, double*);
double perimeter(double*, double*, int);
void My_fill_polygon(double*, double*, int);
int intersects_line(double, double, double*, double*, int);
int vertex(int, int, double*, double*, int);
void Get_All_Edges(double*, double*, int, double[][4]);

int main(int argc, char *argv[]) {
	double xp[100], yp[100], xq[50], yq[50];
	int np, nq;

	//G_init_graphics(600, 600);
	if (argc == 3) {
		np = read_polygon_from_file(argv[1], xp, yp);
		nq = read_polygon_from_file(argv[2], xq, yq);
	} else {
		//G_rgb(1, 0, 0);
		//G_fill_rectangle(580, 580, 600, 600);
		//np=click_polygon(xp, yp);
		//G_rgb(1, 1, 1);
		//G_clear();
		//G_rgb(1, 0, 0);
		//G_fill_rectangle(580, 580, 600, 600);
		//nq=click_polygon(xq, yq);
		//G_rgb(1, 1, 1);
		//G_clear();
	}

	//Testing
	xp[0] = 445;
	yp[0] = 511;
	xp[1] = 446  ;
	yp[1] = 512;
	xp[2] = 253  ;
	yp[2] = 446;
	xp[3] = 112  ;
	yp[3] = 253 ;
	xp[4] =  94  ;
	yp[4] = 112 ;
	xp[5] =  94  ;
	yp[5] = 234;
	xp[6] = 234  ;
	yp[6] = 290 ;
	xp[7] = 290  ;
	yp[7] = 514;
	xp[8] = 512  ;
	yp[8] = 514;

	np = 9;

	//G_rgb(0, 0, 0);
	My_fill_polygon(xp, yp, np);

	// if(perimeter(xp, yp, np) > perimeter(xq, yq, nq)) {
	// 	G_rgb(1, 0, 0);
	// 	My_fill_polygon(xp, yp, np);
	// } else {
	// 	G_rgb(0, 1, 0);
	// 	My_fill_polygon(xq, yq, nq);
	// }
	//G_wait_key();
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

void Print_Edge_Array(double arr[][4], int n) {
	for (int i=0; i<n; i++) {
		printf("Edge %d: %4.0f %4.0f %4.0f %4.5f\n", 
			i, 
			arr[i][0],
			arr[i][1],
			arr[i][2],
			arr[i][3]);
	}
}

/* Count the number of edges in edge_list with ymin less than y
 */
int Sort_Index(double xmin, double ymin, double edges[][4], int n) {
	int count = 0;
	for (int i=0; i<n; i++) {
		if (ymin > edges[i][0]) {
			count++;
		} else if (ymin == edges[i][0] && xmin > edges[i][2]) {
			count++;
		}
	}
	return count;
}

void Get_Global_Edges(double all_edges[][4], int n, double global_edges[][4], int global_edges_count) {
	int global_edge_index;
	for (int i=0; i<n; i++) {
		if (all_edges[i][3] == 0.0) continue;
		int z = Sort_Index(all_edges[i][2], all_edges[i][0], all_edges, n);
		global_edges[z][0] = all_edges[i][0];
		global_edges[z][1] = all_edges[i][1];
		global_edges[z][2] = all_edges[i][2];
		global_edges[z][3] = all_edges[i][3];
	}
	printf("Global Edge Table:\n");
	Print_Edge_Array(global_edges, global_edges_count);
}

void My_fill_polygon(double* x, double* y, int n) {
	double all_edges[n][4];
	Get_All_Edges(x, y, n, all_edges);

	int global_edges_count = 0;
	for (int i=0; i<n; i++) {
		if (all_edges[i][3] == 0.0) continue;
		global_edges_count++;
	}
	double global_edges[global_edges_count][4];
	Get_Global_Edges(all_edges, n, global_edges, global_edges_count);

}



/* Take a polygon definition in the form of two lists of x, y
 * coordinates and transform it into an edge relation table.
 * There is a table row for each pair of vertices that form an edge.
 * The row format is [miny, maxy, x, slope].
 */
void Get_All_Edges(double* x, double* y, int n, double edges[][4] ) {
	int x_offset;
	for(int i=0; i<n; i++) {
		edges[i][0] = fmin(y[i], y[(i+1)%n]);
		edges[i][1] = fmax(y[i], y[(i+1)%n]);
		x_offset = y[(i+1)%n] > y[i];
		edges[i][2] = x[i+x_offset];
		edges[i][3] = (y[(i+1)%n] - y[i]) / (x[(i+1)%n] - x[i]);
	}
	printf("All Edge Table:\n");
	Print_Edge_Array(edges, n);
}

int intersects_line(double col, double row, double* x, double* y, int n) {
	double x1, y1, x2, y2;
	double m, b, b2;
	for (int i=0; i<n; i++) {
		x1 = x[i];
		y1 = y[i];
		x2 = x[(i+1)%n];
		y2 = y[(i+1)%n];
		m = (y2-y1)/(x2-x1);
		b = y1 - m*x1;
		if (abs(row - m*col+b) < .01) return true;
	}
	return false;
}

int vertex(int col, int row, double* x, double* y, int n) {
	int is_vertex = false;
	return is_vertex;
}