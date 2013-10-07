#include <FPT.h>
#include <qdmlib.h>
#include <D2d_matrix.h>
#include <math.h>
#define true 1
#define false 0
#define CANVAS_X 600
#define CANVAS_Y 600


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
	for (int i=0; i<obj.num_shapes; i++) {
		s = &obj.shapes[i];
		for (int j=0; j<s->num_vertices; j++) {
			point_index = s->vertices[j];
			x[j] = obj.xs[point_index];
			y[j] = obj.ys[point_index];
		}
		G_rgb(s->R, s->G, s->B);
		G_fill_polygon(x, y, s->num_vertices);
	}
}

void initial_center(struct drawObject *obj) {
	double maxX = max(obj->xs, obj->num_points);
	double minX = min(obj->xs, obj->num_points);
	double centerX = (maxX + minX)/2;
	double maxY = max(obj->ys, obj->num_points);
	double minY = min(obj->ys, obj->num_points);
	double centerY = (maxY + minY)/2;
	double m[3][3];
	double useless[3][3];
	D2d_make_identity(m);
	D2d_translate(m, useless, -(centerX - CANVAS_X/2), -(centerY - CANVAS_Y/2));
	D2d_mat_mult_points(obj->xs, obj->ys, m, obj->xs, obj->ys, obj->num_points);
}

void rotate_and_center(struct drawObject *obj) {
	double maxX = max(obj->xs, obj->num_points);
	double minX = min(obj->xs, obj->num_points);
	double centerX = (maxX + minX)/2;
	double maxY = max(obj->ys, obj->num_points);
	double minY = min(obj->ys, obj->num_points);
	double centerY = (maxY + minY)/2;
	double m[3][3];
	double useless[3][3];
	D2d_make_identity(m);
	D2d_translate(m, useless, -CANVAS_X/2, -CANVAS_Y/2);
	D2d_rotate(m, useless, M_PI/90.0);
	D2d_translate(m, useless, CANVAS_X/2, CANVAS_Y/2);
	D2d_mat_mult_points(obj->xs, obj->ys, m, obj->xs, obj->ys, obj->num_points);
}

void scale_to_fit(struct drawObject *obj) {
	//lots of code duplication here to translate a shape to the origin.
	//Todo: refactor this into something in qdmlib
	double maxX = max(obj->xs, obj->num_points);
	double minX = min(obj->xs, obj->num_points);
	double centerX = (maxX + minX)/2;
	double widthX = (maxX - minX);
	double maxY = max(obj->ys, obj->num_points);
	double minY = min(obj->ys, obj->num_points);
	double centerY = (maxY + minY)/2;
	double widthY = (maxY - minY);
	double m[3][3];
	double useless[3][3];
	D2d_make_identity(m);
	double scalefactor = 1;
	D2d_translate(m, useless, -CANVAS_X/2, -CANVAS_Y/2);
	double farthestedge[] = {
		fabs(maxX - CANVAS_X/2.0),
		fabs(minX - CANVAS_X/2.0),
		fabs(maxY - CANVAS_Y/2.0),
		fabs(minY - CANVAS_Y/2.0)
	};

	scalefactor = 300/max(farthestedge, 4);

	D2d_scale(m, useless, scalefactor, scalefactor);
	D2d_translate(m, useless, CANVAS_X/2, CANVAS_X/2);
	D2d_mat_mult_points(obj->xs, obj->ys, m, obj->xs, obj->ys, obj->num_points);
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
		initial_center(&objects[i]);
	}
	printf("%d objects loaded. Press a key from 1-%d to draw.\n", num_objects, num_objects);
	
	G_init_graphics(CANVAS_X, CANVAS_Y);
	int c;
	int i;
	//D2d_make_identity(rot);
	while(true) {
		G_rgb(1, 1, 1);
		c = G_wait_key();
		G_clear();
		i = c - 48; //Convert ASCII character codes to digits
		if (i < num_objects+1 && i >= 1) {
			//Rotate the object
			//D2d_mat_mult_points (objects[i-1].xs, objects[i-1].ys, rot, objects[i-1].xs, objects[i-1].ys, objects[i-1].num_points);
			//Now scale and center it 
			rotate_and_center(&objects[i-1]);
			scale_to_fit(&objects[i-1]);
			draw_drawObject(objects[i-1]);
		} else {
			printf("Invalid selection. Please press a key from 1-%d\n", num_objects);
		}
	}
	
}