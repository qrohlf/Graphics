#include <FPT.h>
#include <D3d_matrix.h>
#include <Drawframework.h>
#include <qdmlib.h>
#define true 1
#define false 0
#define CANVAS_X 600
#define CANVAS_Y 600

double fov;
lightmodel lm = {.2, .5, 50};
point3d light_pos = {.5, 1, .1};
int commandmode = 0;
int num_objs;

    object3d* obj;

object3d objs[5];

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
    D3d_translate(x_plus, x_minus, -.1, 0, 0);
    D3d_make_identity(y_plus);
    D3d_make_identity(y_minus);
    D3d_translate(y_plus, y_minus, 0, -.1, 0);
    D3d_make_identity(z_plus);
    D3d_make_identity(z_minus);
    D3d_translate(z_plus, z_minus, 0, 0, .1);
    D3d_make_identity(all_scale_up);
    D3d_make_identity(all_scale_down);
    D3d_scale(all_scale_up, all_scale_down, 1.1, 1.1, 1.1);
}

void draw() {
    G_rgb(.1, .1, .1);
    G_clear();
    draw_object3ds(objs, num_objs, fov, light_pos, lm);
    //printf("draw\n");
    char fovstring[128];
    sprintf(fovstring, "Fov: %.3f rad", fov);
    char camerastring[128];
    sprintf(camerastring, "Camera: x:0 y:0 z:%f", 0.0);
    G_rgb(1, 1, 1);
    G_draw_string(fovstring, 0, 580);
    G_draw_string(camerastring, 0, 560);
    G_display_image();
}

void print_lighting() {
    printf("Lighting weights: a: %f d: %f s: %f. Specular power: %d\n", lm.ambient_weight, lm.diffuse_weight, 1-lm.diffuse_weight-lm.ambient_weight, lm.specular_power);
}

void command_handler() {

    printf("Command Mode Enabled\n\n");
    char line[256];
    while(true) {
        draw();
        printf("✪ ");
        fgets(line, sizeof(line), stdin);
        line[strlen(line) - 1] = '\0'; //chop newline
        char* command;
        command = strtok(line, " ");
        if (strcmp(command, "help") == 0) printf("syntax: ✪ variable value\n");
        else if (strcmp(command, "exit") == 0) {
            printf("Exiting command mode\n");
            return;
        }
        else if (strcmp(command, "reposition") == 0) {
            char* item = strtok(NULL, " ");
            char* position = strtok(NULL, "");
            double x, y, z;
            sscanf(position, "%lf %lf %lf", &x, &y, &z);
            if (strcmp(item, "light") == 0) {
                light_pos.x = x;
                light_pos.y = y;
                light_pos.z = z;
            }
            else if (strcmp(item, "camera") == 0) {
                printf("not yet implemented\n");
                continue;
            }
            else {
                printf("Error: human\n");
                continue;
            }
            printf("repositioned.\n");
            continue;
        }

        else if (strcmp(command, "color") == 0) {
            char* number = strtok(NULL, " ");
            char* color = strtok(NULL, "");
            double R, G, B;
            int n;
            sscanf(number, "%d", &n);
            if (n < 1 || n > num_objs) {
                printf("Object out of range\n");
                continue;
            }
            sscanf(color, "%lf %lf %lf", &R, &G, &B);
            printf("got %f %f %f\n", R, G, B);
            color_obj(&objs[n-1], R, G, B);
            printf("recolored.\n");
            continue;
        }

        else if (strcmp(command, "ambient") == 0) {
            char* value = strtok(NULL, " ");
            double a;
            sscanf(value, "%lf", &a);
            if (a < 0 || a > 1 - lm.diffuse_weight) {
                printf("Error: Your ambient lighting must be >0 and <%f\n", 1 - lm.diffuse_weight);
                continue;
            }
            lm.ambient_weight = a;
            printf("ambient light set to %f.\n", lm.ambient_weight);
            print_lighting();
            continue;
        }
        else if (strcmp(command, "diffuse") == 0) {
            char* value = strtok(NULL, " ");
            double a;
            sscanf(value, "%lf", &a);
            if (a < 0 || a > 1 - lm.ambient_weight) {
                printf("Error: Your diffuse lighting must be >0 and <%f\n", 1 - lm.ambient_weight);
                continue;
            }
            lm.diffuse_weight = a;
            printf("diffuse light set to %f.\n", lm.diffuse_weight);
            print_lighting();
            continue;
        }
        else if (strcmp(command, "specular") == 0) {
            char* value = strtok(NULL, " ");
            int a = 0;
            sscanf(value, "%d", &a);
            if (a < 0) {
                printf("Error: Your specular power must be >0\n");
                continue;
            }
            lm.specular_power = a;
            printf("specular power power set to %d.\n", lm.specular_power);
            print_lighting();
            continue;
        }
        else if (strcmp(command, "draw") == 0) {
            draw(); //totally unneeded
        }
        else if (strcmp(command, "lighting") == 0) {
            print_lighting();
        }
        else {
            printf("Error: human\n");
            continue;
        }
    }
}

void key_handler() {
        char c, i;    
       while(true) {
        draw();
        c = G_wait_key();
        i = c - 48; //Convert ASCII character codes to digits
        if (i > 0 && i <= num_objs) {
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
                command_handler();
                break;
        }
    }
}


int main(int argc, char const *argv[]) {
    double moveback[4][4];
    double distance;
    for (int i=1; i<argc; i++) {
        FILE* f = fopen(argv[i], "r");
        read_object3d_from_file(f, &objs[i-1]);

        obj = &objs[i-1];
        //move the object backwards so that it doesn't intersect with the view
        distance = max(obj->zs, obj->n)+10;
        printf("initial distance is %f\n", distance);
        D3d_make_identity(moveback);
        D3d_translate(moveback, useless, 0, 0, -distance);
        //scale the object to fit the view
        double max_dim = 10;
        double m = max(obj->xs, obj->n);
        if (m > max_dim) max_dim = m;
        m = max(obj->ys, obj->n);
        if (m > max_dim) max_dim = m;
        printf("max dimension is %f\n", max_dim);
        //D3d_scale(moveback, useless, .1, .1, .1);
        transform_object3d(obj, moveback);
    }
    num_objs = argc-1;
    setup_matrices();
    
    
    fov = M_PI/8.0;
    G_init_graphics(600, 600);

    key_handler();
    return 0;
}