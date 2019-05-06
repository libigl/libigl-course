// Copyright (C) 2019 Daniele Panozzo <daniele.panozzo@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public License
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.
#include <igl/avg_edge_length.h>
#include <igl/barycenter.h>
#include <igl/local_basis.h>
#include <igl/readOFF.h>
#include <igl/readOBJ.h>
#include <igl/opengl/glfw/Viewer.h>
#include <igl/triangle_triangle_adjacency.h>
#include <igl/unproject_onto_mesh.h>

#include "vector_field.h"

// Mesh
Eigen::MatrixXd V;
Eigen::MatrixXi F;

// Triangle-triangle adjacency
Eigen::MatrixXi TT;
Eigen::MatrixXi TTi;

// Constrained faces id
Eigen::VectorXi b;

// Cosntrained faces representative vector
Eigen::MatrixXd bc;

// Currently selected face
int selected;

// Local basis
Eigen::MatrixXd B1, B2, B3;

// Plots the mesh with a vector field
// The constrained faces (b) are colored in red.
void plot_vector_field(
  igl::opengl::glfw::Viewer& viewer,
  Eigen::MatrixXd& V,
  Eigen::MatrixXi& F,
  Eigen::MatrixXd& Y,
  Eigen::VectorXi& b)
{
  using namespace Eigen;
  using namespace std;

  // Clear the mesh
  viewer.data().clear();
  viewer.data().set_mesh(V,F);

  // Compute average edge lengh for scaling
  double avg = igl::avg_edge_length(V, F);

  // Compute face barycenters
  MatrixXd B;
  igl::barycenter(V,F,B);

  // Show the vector field as lines
  viewer.data().add_edges(B,B+Y*(avg/2),RowVector3d(0,0,1));

  // Highlight in red the constrained faces
  MatrixXd C = MatrixXd::Constant(F.rows(),3,1);
  for (unsigned i=0; i<b.size();++i)
    C.row(b(i)) << 1, 0, 0;
  viewer.data().set_colors(C);
}

bool key_down(igl::opengl::glfw::Viewer& viewer, unsigned char key, int modifier)
{
  using namespace Eigen;
  using namespace std;

  // Plot the field
  if (key >= '1')
  {
    MatrixXd R = vector_field(V,F,TT,b,bc);
    plot_vector_field(viewer,V,F,R,b);
  }

  // Rotate the field
  if (key == '[' || key == ']')
  {
    if (selected >= b.size() || selected < 0)
      return false;

    int i = b(selected);
    Vector3d v = bc.row(selected);

    double x = B1.row(i) * v;
    double y = B2.row(i) * v;
    double norm = sqrt(x*x+y*y);
    double angle = atan2(y,x);

    angle += key == '[' ? -M_PI/16 : M_PI/16;

    double xj = cos(angle)*norm;
    double yj = sin(angle)*norm;

    bc.row(selected) = xj * B1.row(i) + yj * B2.row(i);
    MatrixXd R = vector_field(V,F,TT,b,bc);
    plot_vector_field(viewer,V,F,R,b);
  }

  // Scale the field
  if (key == 'Q' || key == 'W')
  {
    if (selected >= b.size() || selected < 0)
      return false;

    bc.row(selected) =  bc.row(selected) * (key == 'Q' ? 3./2. : 2./3.);

    MatrixXd R = vector_field(V,F,TT,b,bc);
    plot_vector_field(viewer,V,F,R,b);
  }

  // Erase the currently selected constraint
  if (key == 'E')
  {
    if (selected >= b.size() || selected < 0)
      return false;

    b(selected) = b(b.rows()-1);
    b.conservativeResize(b.size()-1);
    bc.row(selected) = bc.row(bc.rows()-1);
    bc.conservativeResize(b.size(),bc.cols());
    MatrixXd R = vector_field(V,F,TT,b,bc);
    plot_vector_field(viewer,V,F,R,b);
  }

  return false;
}

bool mouse_down(igl::opengl::glfw::Viewer& viewer, int, int)
{
  int fid_ray;
  Eigen::Vector3f bary;
  // Cast a ray in the view direction starting from the mouse position
  double x = viewer.current_mouse_x;
  double y = viewer.core.viewport(3) - viewer.current_mouse_y;
  if(igl::unproject_onto_mesh(Eigen::Vector2f(x,y), viewer.core.view,
    viewer.core.proj, viewer.core.viewport, V, F, fid_ray, bary))
  {
    // If it is a constraint, just make it current
    bool found = false;
    for (int i=0;i<b.size();++i)
    {
      if (b(i) == fid_ray)
      {
        found = true;
        selected = i;
      }
    }

    // If it is not a constraint, make a constraint and highlight it in red
    if (!found)
    {
      b.conservativeResize(b.size()+1);
      b(b.size()-1) = fid_ray;
      bc.conservativeResize(bc.rows()+1,bc.cols());
      bc.row(bc.rows()-1) << 1, 1, 1;
      selected = bc.rows()-1;

      Eigen::MatrixXd R = vector_field(V,F,TT,b,bc);
      plot_vector_field(viewer,V,F,R,b);
    }

    return true;
  }
  return false;
};


int main(int argc, char *argv[])
{
  using namespace std;
  using namespace Eigen;

  // Load a mesh in OBJ format
  igl::readOFF("../bumpy.off", V, F);
  
  // Triangle-triangle adjacency
  igl::triangle_triangle_adjacency(F,TT,TTi);

  // Compute the local_basis
  igl::local_basis(V,F,B1,B2,B3);

  // Simple constraints
  b.resize(2);
  b(0) = 0;
  b(1) = F.rows()-1;
  bc.resize(2,3);
  bc << 1,1,1,0,1,1;

  selected = 0;

  // Initialize the viewer
  igl::opengl::glfw::Viewer viewer;

  // Interpolate the field and plot
  key_down(viewer, '1', 0);

  // Register the callbacks
  viewer.callback_key_down = &key_down;
  viewer.callback_mouse_down = &mouse_down;
  
  // Disable wireframe
  viewer.data().show_lines = false;

  // Launch the viewer
  viewer.launch();
}
