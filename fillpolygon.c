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

	G_init_graphics(600, 600);
	if (argc == 3) {
		np = read_polygon_from_file(argv[1], xp, yp);
		nq = read_polygon_from_file(argv[2], xq, yq);
	} else {
		//G_rgb(1, 0, 0);
		//G_fill_rectangle(580, 580, 600, 600);
		//np=click_polygon(xp, yp);
		//G_rgb(1, 1, 1);
		//G_clear();
		G_rgb(1, 0, 0);
		G_fill_rectangle(580, 580, 600, 600);
		nq=click_polygon(xq, yq);
		//G_rgb(1, 1, 1);
		//G_clear();
	}

	G_rgb(0, 0, 0);
	My_fill_polygon(xq, yq, nq);

	// if(perimeter(xp, yp, np) > perimeter(xq, yq, nq)) {
	// 	G_rgb(1, 0, 0);
	// 	My_fill_polygon(xp, yp, np);
	// } else {
	// 	G_rgb(0, 1, 0);
	// 	My_fill_polygon(xq, yq, nq);
	// }
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
	printf("Global Edge Table (%d):\n", global_edges_count);
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

	int active_edges[global_edges_count]; //Max number of possible active edges is global_edges_count;
	double scanline = global_edges[0][0]; //The initial scanline is the overall minimum y value
	int active_count = 0;
	while (scanline < global_edges[global_edges_count][1]) {
		// for (int i=0; i<global_edges_count; i++) {
		// 	if (global_edges[i][0] == scanline) {
		// 		active_edges[active_edges_count][0] = scanline;
		// 		active_edges[active_edges_count][1] = global_edges[i][1];
		// 		active_edges[active_edges_count][2] = global_edges[i][2];
		// 		active_edges[active_edges_count][3] = global_edges[i][3];
		// 		active_edges_count++;
		// 	}
		// }
		// for (int i=0; i<active_edges_count; i++) {
		// 	G_rgb(i%2, 0, 0);
		// 	G_line(active_edges[i][2], scanline, active_edges[(i+1)%active_edges_count][2], scanline);
		// 	active_edges[i][2] += 1.0f/active_edges[i][3];
		// 	active_edges[i][0] = scanline+1;
		// 	G_wait_key();
		// }

		for (int i=0; i<global_edges_count; i++) {
			if (global_edges[i][0] == scanline) {
				active_edges[active_count] = i;
				active_count++;
			}
		}
		int curr;
		int next;
		double x1, x2, ymax, slope;
		for (int i=0; i<active_count; i++) {
			curr = active_edges[i];
			next = active_edges[(i+1)%active_count];
			x1 = global_edges[curr][2];
			x2 = global_edges[next][2];
			ymax = global_edges[curr][1];
			slope = global_edges[curr][3];
			G_wait_key();
			G_rgb(i%2, 0, 0);
			G_line(x1, scanline, x2, scanline);
			if (scanline < ymax) {
				global_edges[curr][2] += 1.0/slope;
				global_edges[curr][0] += 1;
			}
		}
		scanline+=1;
	}

}


/* Take a polygon definition in the form of two lists of x, y
 * coordinates and transform it into an edge relation table.
 * There is a table row for each pair of vertices that form an edge.
 * The row format is [miny, maxy, x, slope].
 */
void Get_All_Edges(double* x, double* y, int n, double edges[][4] ) {
	int x_offset, ymin_i;
	double y1, y2, ymin, ymax, xmin, xmax;
	for(int i=0; i<n; i++) {
		y1 = y[i];
		y2 = y[(i+1)%n];
		ymin_i = y1 < y2;
		ymin = ymin_i? y[i] : y[(i+1)%n];
		ymax = !ymin_i? y[i] : y[(i+1)%n];
		xmin = ymin_i? x[i] : x[(i+1)%n];
		xmax = !ymin_i? x[i] : x[(i+1)%n];
		edges[i][0] = ymin;
		edges[i][1] = ymax;
		edges[i][2] = xmin;
		edges[i][3] = (ymax - ymin)/(xmax-xmin);
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