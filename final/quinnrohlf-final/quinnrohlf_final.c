/* TO COMPILE THIS CODE:
acom -std=c99 -I. *.c 
*/

#include <FPT.h>
#define M_PI 3.1415926535897932384626433832795
#include <D3d_matrix.h>
#include "Drawframework.h"
#include "qdmlib.h"
#define true 1
#define false 0
#define CANVAS_X 600
#define CANVAS_Y 600

double fov;
lightmodel lm = {.2, .5, 20};
point3d light_pos = {100, 200, 100};
int commandmode = 0;
int num_objs = 0;
int inverted[5];

int CURRENT = 3;
int ticks = 0;

object3d* obj;

object3d objs[6];
double rotate_mat[6][4][4];
double rotate_mat_inv[6][4][4];

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
    inverse[4][4];

void rotate(int i) {
    obj = &objs[i%6];
    if (i < 6) {
        transform_object3d(obj, rotate_mat[i]);
    }  else {
        transform_object3d(obj, rotate_mat_inv[i-6]);
    }
    
}

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
                light_pos.x = -x;
                light_pos.y = -y;
                light_pos.z = -z;
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

void animate() {
    ticks = (ticks+1)%90;
    rotate(CURRENT);
    if (ticks == 0) CURRENT = (CURRENT+1)%12;
    draw();
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
            case 's':
                while(true) {
                    animate();
                }
            case 32:
                animate();
                break;
        }
    }
}

void invert(object3d* obj) {
    for (int i=0; i<obj->num_shapes; i++) {
        shape* s = &obj->shapes[i];
        int temp = s->vertices[0];
        s->vertices[0] = s->vertices[3];
        s->vertices[3] = temp;
        temp = s->vertices[1];
        s->vertices[1] = s->vertices[2];
        s->vertices[2] = temp;
    }
}


int main(int argc, char const *argv[]) {
    double transform[4][4];
    double distance;
    FILE* f = fopen("sphere.xyz", "r");
    object3d sphere;
    read_object3d_from_file(f, &sphere);

    object3d cylinder;
    f = fopen("cylinder.xyz", "r");
    read_object3d_from_file(f, &cylinder);
    invert(&cylinder);
    //move cylinder back and rotate
    D3d_make_identity(transform);
    D3d_rotate_z(transform, inverse, M_PI/2);
    D3d_translate(transform, inverse, 0, 0, -12);
    D3d_scale(transform, inverse, 1, 2.5, 1);
    transform_object3d(&cylinder, transform);
    
    //move sphere back and scale
    D3d_make_identity(transform);
    D3d_scale(transform, inverse, .65, .65, .65);
    D3d_translate(transform, inverse, 0, 0, -12.0);
    transform_object3d(&sphere, transform);
    object3d sphere2 = sphere;
    D3d_make_identity(transform);
    D3d_make_identity(inverse);
    D3d_translate(transform, inverse, 0, 3, 0);
    transform_object3d(&sphere, transform);
    transform_object3d(&sphere2, inverse);

    color_obj(&sphere, .2, .5, .6);
    color_obj(&sphere2, .2, .5, .8);
    color_obj(&cylinder, .2, .5, 1);
    
    

    object3d barbell;
    object3d_concat(&barbell, &cylinder);
    object3d_concat(&barbell, &sphere);
    object3d_concat(&barbell, &sphere2);
    center_of_mass(&barbell, &barbell.xs[barbell.n], &barbell.ys[barbell.n], &barbell.zs[barbell.n]);
    barbell.center = barbell.n;
    barbell.n++;

    for (int i=0; i<6; i++) {
        objs[i] = barbell;
        obj = &objs[i];
        object3d_rotate(&objs[i], 0, 0, M_PI/6*i);
        D3d_make_identity(rotate_mat[i]);
        int c = obj->center;
        //center_of_mass(obj, &center);
        //printf("center of mass is %f %f %f\n", center.x, center.y, center.z);
        // translate to the origin
        D3d_translate(rotate_mat[i], inverse, -obj->xs[c], -obj->ys[c], -obj->zs[c]);
        D3d_rotate_z(rotate_mat[i], inverse, -M_PI/6*i);
        D3d_rotate_x(rotate_mat[i], inverse, M_PI/90);
        D3d_rotate_z(rotate_mat[i], inverse, M_PI/6*i);
        // translate back from the origin
        D3d_translate(rotate_mat[i], inverse, obj->xs[c], obj->ys[c], obj->zs[c]);

        D3d_make_identity(rotate_mat_inv[i]);
        //center_of_mass(obj, &center);
        //printf("center of mass is %f %f %f\n", center.x, center.y, center.z);
        // translate to the origin
        D3d_translate(rotate_mat_inv[i], inverse, -obj->xs[c], -obj->ys[c], -obj->zs[c]);
        D3d_rotate_z(rotate_mat_inv[i], inverse, -M_PI/6*i);
        D3d_rotate_x(rotate_mat_inv[i], inverse, -M_PI/90);
        D3d_rotate_z(rotate_mat_inv[i], inverse, M_PI/6*i);
        // translate back from the origin
        D3d_translate(rotate_mat_inv[i], inverse, obj->xs[c], obj->ys[c], obj->zs[c]);
        
    }
    num_objs = 6;
    setup_matrices();
    
    
    fov = M_PI/8.0;
    G_init_graphics(600, 600);

    key_handler();
    return 0;
}