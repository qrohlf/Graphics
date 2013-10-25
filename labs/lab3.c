#include <FPT.h>
#include <qdmlib.h>
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

// Draw fig
void draw(shape* fig) {
    G_rgb(fig->R, fig->G, fig->B);
    G_fill_polygon(fig->xs, fig->ys, fig->n);
}

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

int p_in_range(point a, point b, point c) {
    double xmax = fmax(b.x, c.x);
    double xmin = fmin(b.x, c.x);
    double ymax = fmax(b.y, c.y);
    double ymin = fmin(b.y, c.y);
    return (in_range(a.x, xmin, xmax) && in_range(a.y, ymin, ymax));

}

int isRight(point a, point b, point c){
     return ((b.x - a.x)*(c.y - a.y) - (b.y - a.y)*(c.x - a.x)) < 0;
}

// Clip shape fig by a single line
void clip_line(shape* fig, point l1, point l2) {
    shape out = {
        0, 
        {}, 
        {},
        0,
        0,
        0
    };
    G_rgb(1, 0, 0);
    G_line(l1.x, l1.y, l2.x, l2.y);
    for (int i=0; i<fig->n; i++) {
        point p1 = {fig->xs[i], fig->ys[i]};
        point p2 = {fig->xs[(i+1)%fig->n], fig->ys[(i+1)%fig->n]};
        G_rgb(1, 0, 0);
        G_line(p1.x, p1.y, p2.x, p2.y);
        if (isRight(p1, l1, l2)) {
            out.xs[out.n] = p1.x;
            out.ys[out.n] = p1.y;
            out.n++;
            G_rgb(0, 1, 1);
            G_fill_circle(p1.x, p1.y, 5);
            G_wait_key();
        }
        point intersect;
        intersection(p1, p2, l1, l2, &intersect);
        if (in_range(intersect.x, p1.x, p2.x) && in_range(intersect.y, p1.y, p2.y)) {
            
            out.xs[out.n] = intersect.x;
            out.ys[out.n] = intersect.y;
            out.n++;
            G_rgb(0, 1, 1);
            G_fill_circle(intersect.x, intersect.y, 5);
            G_wait_key();
            G_rgb(.8, .8, .8);
            G_line(l1.x, l1.y, l2.x, l2.y);
        }
        G_wait_key();
        G_rgb(.8, .8, .8);
        G_line(p1.x, p1.y, p2.x, p2.y);
    }
    G_rgb(1, 1, 1);
    G_clear();
    draw(&out);
    G_wait_key();
    *fig = out;
}

// Clip shape fig by shape window
void clip(shape* fig, shape* win) {
    shape out = {
        0, 
        {}, 
        {},
        0,
        0,
        0
    };
    for (int i=0; i<fig->n; i++) {
        point p1 = {fig->xs[i], fig->ys[i]};
        point p2 = {fig->xs[(i+1)%fig->n], fig->ys[(i+1)%fig->n]};
        G_rgb(1, 0, 0);
        G_line(p1.x, p1.y, p2.x, p2.y);
        for (int j=0; j<win->n; j++) {
            point p3 = {win->xs[j], win->ys[j]};
            point p4 = {win->xs[(j+1)%win->n], win->ys[(j+1)%win->n]};
            clip_line(fig, p3, p4);
            
        }
        G_rgb(.8, .8, .8);
        G_line(p1.x, p1.y, p2.x, p2.y);
    }
    draw(&out);
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
        0, 
        {}, 
        {},
        .8,
        .8,
        .8
    };
    click_polygon(&fig);
    draw(&fig);
    shape window; 
    click_polygon(&window);
    G_rgb(1, 1, 1);
    clip(&fig, &window);


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