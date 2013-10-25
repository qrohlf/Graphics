#include <FPT.h>
#define true 1
#define false 0
#define CANVAS_X 600
#define CANVAS_Y 600

// Shape type definition
typedef struct {
    int n;   // Number of vertices in this shape
    double xs[100];     // Array of vertex x-coordinates
    double ys[100];     // Y-coords
    double R;           // Red
    double G;           // Green
    double B;           // Blue
} shape;

typedef struct {
    double x;
    double y;
} point;

// Retern the intersection
void intersection(
    point p1,
    point p2,
    point j1,
    point j2,
    point* intersect) {
    // Slope of line p (mp)
    double mp = (p2.y - p1.y) / (p2.x - p1.x);
    printf("Slope of line 1 is %f\n", mp);
    // Slope of line j (mj)
    double mj = (j2.y - j1.y) / (j2.x - j1.x);
    printf("Slope of line 2 is %f\n", mj);
    //p1.y = mp (p1.x) + bp
    double bp = p1.y - mp*p1.x;
    printf("Intercept of line 1 is %f\n", bp);
    //j1.y = mj (j1.x) + bj
    double bj = j1.y - mj*j1.x;
    printf("Intercept of line 2 is %f\n", bj);
    //y = mp * x + bp
    //y = mj * x + bj
    //mj*x + bj = mp*x + bp
    //mj*x - mp*x = bp - bj
    //x(mj - mp) = (bp - bj)
    intersect->x = (bp - bj)/(mj - mp);
    printf("Intersection x is %f\n", intersect->x);
    intersect->y = mp * intersect->x + bp;
    printf("Intersection y is %f\n", intersect->y);
}

// Clip shape fig by shape window
void clip(shape* fig, shape* window) {
    for (int i=0; i<window->n; i++) {
        point p1 = {window->xs[i], window->ys[i]};
        point p2 = {window->xs[(i+1)%window->n], window->ys[(i+1)%window->n]};
        G_rgb(1, 0, 0);
        G_line(p1.x, p1.y, p2.x, p2.y);
        for (int j=0; j<fig->n; j++) {
            point p3 = {fig->xs[j], fig->ys[j]};
            point p4 = {fig->xs[(j+1)%fig->n], fig->ys[(j+1)%fig->n]};
            G_rgb(1, 0, 0);
            G_line(p3.x, p3.y, p4.x, p4.y);
            point intersect;
            intersection(p1, p2, p3, p4, &intersect);
            G_rgb(0, 1, 1);
            G_fill_circle(intersect.x, intersect.y, 5);
            G_wait_key();
            G_rgb(.8, .8, .8);
            G_line(p3.x, p3.y, p4.x, p4.y);
        }
        G_rgb(.8, .8, .8);
        G_line(p1.x, p1.y, p2.x, p2.y);
    }
}

// Draw fig
void draw(shape* fig) {
    G_rgb(fig->R, fig->G, fig->B);
    G_polygon(fig->xs, fig->ys, fig->n);
}

void click_polygon(shape* fig) {
    G_rgb(.5, 0, 0);
    G_fill_rectangle(580, 580, 600, 600);
    G_rgb(0.8, 0.8, 0.8);
    int i=0;
    double p[] = {0, 0};
    while(true) {
        G_wait_click(p);
        if (p[0] > 580 && p[1] > 580) break;
        G_fill_circle(p[0], p[1], 2);
        fig->xs[i] = p[0];
        fig->ys[i] = p[1];
        printf("Clicked %5.0f, %5.0f\n", p[0], p[1]);
        if (i>0) G_line(fig->xs[i], fig->ys[i], fig->xs[i-1], fig->ys[i-1]);
        i++;
    }
    printf("Done clicking polygon\n");
    fig->n = i;
}


int main(int argc, char const *argv[]) {
    G_init_graphics(CANVAS_X, CANVAS_Y);
    //Testing data
    shape fig = {
        4, 
        {300, 400, 300, 200}, 
        {400, 300, 200, 300},
        .8,
        .8,
        .8
    };
    draw(&fig);
    shape window; 
    click_polygon(&window);
    G_rgb(1, 1, 1);
    clip(&fig, &window);
    draw(&fig);


    // G_rgb(0, 1, 0);
    // G_line(p1.x, p1.y, p2.x, p2.y);
    // G_rgb(0, 0, 1);
    // G_line(p3.x, p3.y, p4.x, p4.y);
    // point intersect;
    // intersection(p1, p2, p3, p4, &intersect);
    // G_rgb(0, 1, 1);
    // G_fill_circle(intersect.x, intersect.y, 5);
     G_wait_key();
    return 0;
}