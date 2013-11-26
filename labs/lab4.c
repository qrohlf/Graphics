#include <FPT.h>
#include <D3d_matrix.h>
#include <Drawframework.h>
#include <qdmlib.h>
#define true 1
#define false 0
#define CANVAS_X 600
#define CANVAS_Y 600

double fov;
double viewdistance;

double x_ccw[4][4], 
    x_cw[4][4], 
    y_ccw[4][4], 
    y_cw[4][4],
    z_ccw[4][4],
    z_cw[4][4],
    x_minus[4][4],
    x_plus[4][4],
    y_minus[4][4],
    y_plus[4][4],
    z_minus[4][4],
    z_plus[4][4],
    all_scale_up[4][4],
    all_scale_down[4][4],
    useless[4][4];

void setup_matrices() {
    D3d_make_identity(x_ccw);
    D3d_make_identity(x_cw);
    D3d_rotate_x(x_ccw, x_cw, M_PI/480);
    D3d_make_identity(y_ccw);
    D3d_make_identity(y_cw);
    D3d_rotate_y(y_ccw, y_cw, M_PI/480);
    D3d_make_identity(z_ccw);
    D3d_make_identity(z_cw);
    D3d_rotate_z(z_ccw, z_cw, M_PI/480);
    D3d_make_identity(x_plus);
    D3d_make_identity(x_minus);
    D3d_translate(x_plus, x_minus, .2, 0, 0);
    D3d_make_identity(y_plus);
    D3d_make_identity(y_minus);
    D3d_translate(y_plus, y_minus, 0, .2, 0);
    D3d_make_identity(z_plus);
    D3d_make_identity(z_minus);
    D3d_translate(z_plus, z_minus, 0, 0, .2);
    D3d_make_identity(all_scale_up);
    D3d_make_identity(all_scale_down);
    D3d_scale(all_scale_up, all_scale_down, 1.005, 1.005, 1.005);
}


int main(int argc, char const *argv[]) {
    object3d objs[9];
    object3d* obj;
    double moveback[4][4];
    for (int i=1; i<argc; i++) {
        FILE* f = fopen(argv[i], "r");
        read_object3d_from_file(f, &objs[i-1]);

        obj = &objs[i-1];
        viewdistance = max(obj->zs, obj->n)+10;
        printf("initial viewdistance is %f\n", viewdistance);
        D3d_make_identity(moveback);
        D3d_translate(moveback, useless, 0, 0, -viewdistance);
        transform_object3d(obj, moveback);
    }
    setup_matrices();
    
    char c, i;
    fov = M_PI/8.0;
    
    viewdistance = 0;
    G_init_graphics(600, 600);

    while(true) {
        i = c - 48; //Convert ASCII character codes to digits
        if (i > 0 && i < argc) {
            printf("Switching control to shape %d\n", i);
            obj = &objs[i-1];
        }
        switch (c) {
            case 84:
                //Down arrow key
                object3d_rotate(obj, -M_PI/60, 0, 0);
                break;
            case 82:
                //Up arrow key
                object3d_rotate(obj, M_PI/60, 0, 0);
                break;
            case 81:
                //Left arrow key
                object3d_rotate(obj, 0, M_PI/60, 0);
                break;
            case 83:
                //Right arrow key
                object3d_rotate(obj, 0, -M_PI/60, 0);
                break;
            case 'w':
                transform_object3d(obj, y_plus);
                break;
            case 'a':
                transform_object3d(obj, x_minus);
                break;
            case 's':
                transform_object3d(obj, y_minus);
                break;
            case 'd':
                transform_object3d(obj, x_plus);
                break;
            case 'q':
                transform_object3d(obj, z_minus);
                break;
            case 'e':
                transform_object3d(obj, z_plus);
                break;
            case ']':
                transform_object3d(obj, all_scale_up);
                break;
            case '[':
                transform_object3d(obj, all_scale_down);
                break;
            case '=':
                //viewdistance += .2;
                break;
            case '-':
                //viewdistance -= .2;
                break;
            case '+':
                //Smaller FOV zooms in
                fov -= M_PI/36;
                break;
            case '_':
                //Larger FOV zooms out
                fov += M_PI/36;
                break;
            case 32:
                //Spacebar
                break;
        }
        G_rgb(.1, .1, .1);
        G_clear();
        for (int j=0; j<argc; j++) {
            draw_object3d(&objs[j], fov, 0);
        }
        char fovstring[128];
        sprintf(fovstring, "Fov: %.3f rad", fov);
        char camerastring[128];
        sprintf(camerastring, "Camera: x:0 y:0 z:%f", 0);
        G_rgb(1, 1, 1);
        G_draw_string(fovstring, 0, 580);
        G_draw_string(camerastring, 0, 560);
        c = G_wait_key();
    }
    return 0;
}