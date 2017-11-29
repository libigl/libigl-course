// g++ -std=c++11 -I../libigl/include -I../libigl/external/nanogui/ext/eigen  main.cpp

#include <igl/doublearea.h>
#include <igl/readOBJ.h>
#include <Eigen/Core>
#include <iostream>

int main(int argc, char *argv[])
{
  using namespace Eigen;
  using namespace std;

  MatrixXd V;
  MatrixXi F;

  igl::readOBJ("cube_twist.obj",V,F);

  // Compute areas, min, max and standard deviation
  VectorXd area;
  igl::doublearea(V,F,area);
  area = area.array() / 2;

  double area_avg   = area.mean();
  double area_min   = area.minCoeff() / area_avg;
  double area_max   = area.maxCoeff() / area_avg;
  double area_sigma = sqrt(((area.array()-area_avg)/area_avg).square().mean());

  printf("Areas (Min/Max)/Avg_Area Sigma: \n%.2f/%.2f (%.2f)\n",
    area_min,area_max,area_sigma);

}
