#include <FPT.h>
#include <D2d_matrix.h>

// Run the program, jet_diagonal.exe, by typing  ./jet_diagonal.exe
// By continually holding down on a key, you will see a movie.
// Your task is to duplicate this program.

// The screen is 600x600 and there are 600 frames to the movie.
// Every hundredth frame, the jet switches the side of the diagonal.

// For full credit, compute any matrices you need BEFORE the loop
// that displays the movie.  An efficient solution does not need
// to perpetually rebuild them with each new scene.


//jet
double jx[5] = {0, 40, 35, 10,  0} ;
double jy[5] = {0,  0,  8,  8, 15} ;


int main()
{

  G_init_graphics(600,600) ;

  G_rgb(0,0,0) ;
  G_clear() ;

  G_rgb(0,0,1) ;
  G_fill_polygon(jx,jy,5) ;
  G_wait_key();
  double rot45[3][3];
  double useless[3][3];
  D2d_make_identity(rot45);
  D2d_rotate(rot45, useless, M_PI/4.0);
  D2d_mat_mult_points(jx, jy, rot45, jx, jy, 5);
  G_fill_polygon(jx, jy, 5);
  double upandover[3][3];
  D2d_make_identity(upandover);
  D2d_translate(upandover, useless, 1, 1);
  int count = 0;
  while (G_wait_key()) {
    count = (count+1)%200;

    D2d_mat_mult_points(jx, jy, upandover, jx, jy, 5);
    G_rgb(0, 0, 0);
    G_clear();
    G_rgb(0, 0, 1);
    if (count < 100) {
        G_fill_polygon(jx, jy, 5);
    } else {
        G_fill_polygon(jy, jx, 5);
    }
    G_rgb(1, 0, 0);
    G_line(0, 0, 600, 600);
  }
}
