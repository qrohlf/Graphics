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
    if (!isfinite(mp)) {
        //p1-p2 is a vertical line
        intersect->x = p1.x;
        intersect->y = mj * intersect->x + bj;
        return;
    }
    if (!isfinite(mj)) {
        //j1-j2 is a vertical line
        intersect->x = j1.x;
        intersect->y = mp * intersect->x + bp;
        return;
    }
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

// Use the cross product to tell if a point is on the right or colinear of the line bc
int isRight(point a, point b, point c){
     return ((b.x - a.x)*(c.y - a.y) - (b.y - a.y)*(c.x - a.x)) <= 0;
}

// Clip shape fig by a single line
void clip_line(shape* fig, point l1, point l2) {
    shape out = {
        0, 
        {}, 
        {},
        1,
        0,
        0
    };
    for (int i=0; i<fig->n; i++) {
        point p1 = {fig->xs[i], fig->ys[i]};
        point p2 = {fig->xs[(i+1)%fig->n], fig->ys[(i+1)%fig->n]};
        if (isRight(p1, l1, l2)) {
            out.xs[out.n] = p1.x;
            out.ys[out.n] = p1.y;
            out.n++;
        }
        point intersect;
        intersection(p1, p2, l1, l2, &intersect);
        if (in_range(intersect.x, p1.x, p2.x) && in_range(intersect.y, p1.y, p2.y)) {
            
            out.xs[out.n] = intersect.x;
            out.ys[out.n] = intersect.y;
            out.n++;
        }
    }
    *fig = out;
}

// Clip shape fig by shape window
void clip(shape* fig, shape* win) {
    for (int j=0; j<win->n; j++) {
        point p3 = {win->xs[j], win->ys[j]};
        point p4 = {win->xs[(j+1)%win->n], win->ys[(j+1)%win->n]};
        clip_line(fig, p3, p4);
    }
}





void click_polygon(shape* fig) {
    G_rgb(.5, 0, 0);
    G_fill_rectangle(580, 580, 600, 600);
    G_rgb(0.8, 0.8, 0.8);
    int i=0;
    double p[] = {0, 0};
    while(true) {
        G_wait_click(p);
        if (p[0] > 580 && p[1] > 580) {
            break;
        }
        G_fill_circle(p[0], p[1], 2);
        fig->xs[i] = p[0];
        fig->ys[i] = p[1];
        printf("Clicked %5.0f, %5.0f\n", p[0], p[1]);
        if (i>0) G_line(fig->xs[i], fig->ys[i], fig->xs[i-1], fig->ys[i-1]);
        i++;
    }

    G_line(fig->xs[i-1], fig->ys[i-1], fig->xs[0], fig->ys[0]);
    printf("Done clicking polygon\n");
    fig->n = i;
}


int main(int argc, char const *argv[]) {
    G_init_graphics(CANVAS_X, CANVAS_Y);
    //Testing data
    // shape fig = {
    //     10, 
    //     {240,  85, 169, 459, 526, 305, 529, 459, 177,  30}, 
    //     {262, 353, 479, 543, 369, 411, 165,  47, 101, 204},
    //     .8,
    //     .8,
    //     .8
    // };
    shape fig;
    click_polygon(&fig);
    draw(&fig);
    // shape window = {
    //     4,
    //     {200, 200, 400, 400},
    //     {200, 400, 400, 200},
    //     .8,
    //     .8,
    //     .8

    // };
    shape window;
    click_polygon(&window);
    shape clipped = fig;
    clip(&clipped, &window);
    clipped.R = 0;
    clipped.G = 1;
    clipped.B = 1;
    G_rgb(1, 1, 1);
    G_clear();
    G_rgb(0, 1, 0);
    G_polygon(fig.xs, fig.ys, fig.n);
    G_rgb(0, 0, 1);
    G_polygon(window.xs, window.ys, window.n);
    draw(&clipped); //Blue


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