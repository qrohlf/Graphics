#include <FPT.h>
#include <D3d_matrix.h>
#include <Drawframework.h>
#include <qdmlib.h>
#define true 1
#define false 0
#define CANVAS_X 600
#define CANVAS_Y 600

double x_ccw[4][4], 
    x_cw[4][4], 
    y_ccw[4][4], 
    y_cw[4][4],
    z_ccw[4][4],
    z_cw[4][4],
    x_minus[4][4],
    x_plus[4][4];

void setup_matrices() {
    D3d_make_identity(x_ccw);
    D3d_make_identity(x_cw);
    D3d_rotate_x(x_ccw, x_cw, M_PI/90);
    D3d_make_identity(y_ccw);
    D3d_make_identity(y_cw);
    D3d_rotate_y(y_ccw, y_cw, M_PI/90);
    D3d_make_identity(z_ccw);
    D3d_make_identity(z_cw);
    D3d_rotate_z(z_ccw, z_cw, M_PI/90);
    D3d_make_identity(x_plus);
    D3d_make_identity(x_minus);
    D3d_translate(x_plus, x_minus, .5, 0, 0);
}

int main(int argc, char const *argv[]) {
    object3d obj;
    FILE* f = fopen(argv[1], "r");
    read_object3d_from_file(f, &obj);
    setup_matrices();
    
    char c;
    double fov = 150;//M_PI/2.0;
    double viewdistance = max(obj.zs, obj.n)+1;
    // double ztrans[4][4], useless[4][4];
    // D3d_make_identity(ztrans);
    // D3d_translate(ztrans, useless, 0, 0, -3);
    // print_object3d(&obj);
    // transform_object3d(&obj, ztrans);
    // print_object3d(&obj);
    G_init_graphics(600, 600);
    draw_object3d(&obj, fov, viewdistance);

    while(true) {
        c = G_wait_key();
        switch (c) {
            case 84:
                //Down arrow key
                transform_object3d(&obj, x_cw);
                break;
            case 82:
                //Up arrow key
                transform_object3d(&obj, x_ccw);
                break;
            case 81:
                //Left arrow key
                transform_object3d(&obj, y_ccw);
                break;
            case 83:
                //Right arrow key
                transform_object3d(&obj, y_cw);
                break;
            case 'a':
                transform_object3d(&obj, x_minus);
                break;
            case 'd':
                transform_object3d(&obj, x_plus);
                break;
            case '+':
            case '=':
                viewdistance -= .2;
                break;
            case '-':
                viewdistance += .2;
                break;
            case 32:
                //Spacebar
                break;
        }
        G_rgb(1, 1, 1);
        G_clear();
        draw_object3d(&obj, fov, viewdistance);
    }
    return 0;
}