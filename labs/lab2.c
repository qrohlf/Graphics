#include <FPT.h>
#include <qdmlib.h>
#include <D2d_matrix.h>
#include <math.h>
#define true 1
#define false 0;


struct shape {
	int num_vertices;
	int vertices[1000];
	double R;
	double G;
	double B;
};

struct drawObject {
	int num_shapes;
	int num_points;
	double xs[1000];
	double ys[1000];
	struct shape shapes[100];
};



/*
 * constructs an object from a list of points, connections, and colors
 */
void make_drawObject(FILE* f, struct drawObject *obj) {
	// Get the points
	fscanf(f, "%d", &obj->num_points);
	printf("num_points is %d\n", obj->num_points);
	for (int i=0; i<obj->num_points; i++) {
		fscanf(f, "%lf %lf", &obj->xs[i], &obj->ys[i]);
	}


	// Get the shapes
	fscanf(f, "%d", &obj->num_shapes);
	for (int i=0; i<obj->num_shapes; i++) {
		fscanf(f, "%d", &obj->shapes[i].num_vertices);
		for (int j=0; j<obj->shapes[i].num_vertices; j++) {
			fscanf(f, "%d", &obj->shapes[i].vertices[j]);
		}
	}

	// Get the colors
	for (int i=0; i<obj->num_shapes; i++) {
		fscanf(f, "%lf %lf %lf", &obj->shapes[i].R, &obj->shapes[i].G, &obj->shapes[i].B);
	}
}

void draw_drawObject(struct drawObject obj) {
	struct shape *s;
	double x[1000];
	double y[1000];
	int point_index;
	printf("%d shapes in object to draw\n", obj.num_shapes);
	for (int i=0; i<obj.num_shapes; i++) {
		s = &obj.shapes[i];
		printf("%d vertices in shape %d\n", s->num_vertices, i);
		for (int j=0; j<s->num_vertices; j++) {
			point_index = s->vertices[j];
			x[j] = obj.xs[point_index];
			y[j] = obj.ys[point_index];
		}
		G_rgb(s->R, s->G, s->B);
		printf("Color is %f %f %f\n", s->R, s->G, s->B);
		G_fill_polygon(x, y, s->num_vertices);
	}
}

int main(int argc, char *argv[]) {
	struct drawObject objects[10];
	FILE *f;
	int num_objects = argc-1;
	for (int i=0; i<num_objects; i++) {
		f = fopen(argv[i+1], "r");
		if (f == NULL) {
			printf("Invalid file %s\n", argv[i+1]);
			exit(1);
		}
		make_drawObject(f, &objects[i]);
	}
	printf("%d objects loaded. Press any digit to draw.\n", num_objects);
	
	G_init_graphics(600, 600);
	int c;
	int i;
	double rot[3][3];
	double rot_inv[3][3];
	D2d_make_identity(rot);
	D2d_rotate(rot, rot_inv, M_PI/180.0);
	//D2d_make_identity(rot);
	while(true) {
		G_rgb(1, 1, 1);
		c = G_wait_key();
		G_clear();
		i = c - 48; //Convert ASCII character codes to digits
		printf("i is %d\n", i);
		if (i < num_objects+1 && i >= 1) {
			D2d_mat_mult_points (objects[i-1].xs, objects[i-1].ys, rot, objects[i-1].xs, objects[i-1].ys, objects[i-1].num_points);
			draw_drawObject(objects[i-1]);
		} else {
			printf("Invalid selection. Please press a key from 1-%d\n", num_objects);
		}
	}
	
}