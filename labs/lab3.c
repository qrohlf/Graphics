#include <FPT.h>
#include <qdmlib.h>
#include <drawframework.h>
#define true 1
#define false 0
#define CANVAS_X 600
#define CANVAS_Y 600

void click_polygon(polygon* fig) {
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
    FILE* f = fopen(argv[1], "r");
    object2d fig;
    polygon win;
    read_object2d_from_file(f, &fig);
    draw_object2d(&fig);
    click_polygon(&win);
    clip_object2d(&fig, &win);
    G_rgb(1, 1, 1);
    G_clear();
    draw_object2d(&fig);
    G_rgb(.8, .8, .8);
    G_polygon(win.xs, win.ys, win.n);
    G_wait_key();
    return 0;
}