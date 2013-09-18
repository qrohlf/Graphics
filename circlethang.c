#include <FPT.h>

void draw(double, double, double, double);

int main() 
{
 double x;
 double y;
 double p1[2];
 double p2[2];
 double r;
 double foo;
 double offset=0;
 G_init_graphics(400, 400);
 G_rgb(0,0,0);
 G_wait_click(p1);

 G_wait_click(p2);

 r = sqrt(pow(p1[0]-p2[0], 2)+pow(p1[1]-p2[1], 2));
 while (1) {
 	draw(p1[0], p1[1], r, offset);
        offset += M_PI/180000;
        sleep(0.05);
 }

 G_wait_key();
}
