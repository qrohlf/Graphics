#include <FPT.h>
#include <D3d_matrix.h>
#include <Drawframework.h>
#define true 1
#define false 0
#define CANVAS_X 600
#define CANVAS_Y 600

double x_ccw[4][4], 
    x_cw[4][4], 
    y_ccw[4][4], 
    y_cw[4][4],
    z_ccw[4][4],
    z_cw[4][4];

void setup_matrices() {
    D3d_make_identity(x_ccw);
    D3d_make_identity(x_cw);
    D3d_rotate_x(x_ccw, x_cw, M_PI/180);
    D3d_make_identity(y_ccw);
    D3d_make_identity(y_cw);
    D3d_rotate_y(y_ccw, y_cw, M_PI/180);
    D3d_make_identity(z_ccw);
    D3d_make_identity(z_cw);
    D3d_rotate_z(z_ccw, z_cw, M_PI/180);
}

int main(int argc, char const *argv[]) {
    object3d obj;
    FILE* f = fopen(argv[1], "r");
    read_object3d_from_file(f, &obj);
    setup_matrices();
    
    char c;
    double halfAngle = 150;//M_PI/2.0;

    G_init_graphics(600, 600);
    draw_object3d(&obj, halfAngle);

    while(true) {
        c = G_wait_key();
        printf("%d\n", c);
        switch (c) {
            case 84:
                //Down arrow key
                transform_object3d(&obj, x_ccw);
                break;
            case 82:
                //Up arrow key
                transform_object3d(&obj, x_cw);
                break;
            case 81:
                //Left arrow key
                transform_object3d(&obj, y_ccw);
                break;
            case 83:
                //Right arrow key
                transform_object3d(&obj, y_cw);
                break;
            case '+':
            case '=':
                halfAngle += 1;
                break;
            case '-':
                halfAngle -= 1;
                break;
            case 32:
                //Spacebar
                break;
        }
        G_rgb(1, 1, 1);
        G_clear();
        draw_object3d(&obj, halfAngle);
    }
    return 0;
}