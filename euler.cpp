#include <iostream>
#include <getopt.h>
#include <cmath>
#include <cstdio>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;

void usage(char **argv){
  fprintf(stderr, "\nUsage: %s [options]\n",argv[0]);
  fprintf(stderr, " -v vinit: initial speed [default 10 m/s]\n");
  fprintf(stderr, " -a theta0: initial angle above horizontal [45deg]\n");
  fprintf(stderr, " -t tstep: time step for approximation [0.01s]\n");
  fprintf(stderr, " -h: print this message\n");
}

int main(int argc, char **argv) {
  double vinit=10;    // m/s
  double theta0=45;   // deg
  double dt=0.01;     // time step [s]

  int opt;
  while ((opt = getopt(argc, argv, "v:a:t:h")) != -1) {
    switch (opt) {
    case 'v':
      vinit = atof(optarg);
      break;
    case 'a':
      theta0=atof(optarg);
      break;
    case 't':
      dt=atof(optarg);
      break;
    case 'h':
      usage(argv);
      return 0;
    default:
      ;
    }
  }

  printf("Simulating projectile motion with params:\n");
  printf("(vinit,theta0,dt)=(%7.2lf,%7.2lf,%7.2f)\n",vinit,theta0,dt);

  // ---------------------------------------------------------
  // INITIAL SETUP
  // ---------------------------------------------------------

  // convert angle from degrees to radians
  double theta = theta0 * M_PI / 180.0;

  // initial time
  double t = 0.0;

  // initial position
  double x = 0.0;
  double y = 0.0;

  // initial velocities
  double vx = vinit * cos(theta);
  double vy = vinit * sin(theta);

  const double g = 9.8;  // gravitational acceleration

  // open output file
  FILE *fp = fopen("trajectory.dat","w");
  fprintf(fp, "# t   x   y   vx   vy\n");

  // write initial point
  fprintf(fp, "%lf %lf %lf %lf %lf\n", t, x, y, vx, vy);

  // ---------------------------------------------------------
  // EULER LOOP
  // ---------------------------------------------------------

  while (y >= 0.0) {

    // update velocities
    double vx_new = vx;           // no horizontal acceleration
    double vy_new = vy - g * dt;  // gravity acts downward

    // update positions
    double x_new = x + vx * dt;
    double y_new = y + vy * dt;

    // update time
    t += dt;

    // assign updates
    vx = vx_new;
    vy = vy_new;
    x  = x_new;
    y  = y_new;

    // write to file
    fprintf(fp, "%lf %lf %lf %lf %lf\n", t, x, y, vx, vy);
  }

  fclose(fp);

  printf("Projectile landed at x ≈ %.3f meters\n", x);

  return 0;
}

