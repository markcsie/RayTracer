#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <array>

struct Vertex
{
  std::array<double, 3> position_;
  std::array<double, 3> color_diffuse_;
  std::array<double, 3> color_specular_;
  std::array<double, 3> normal_;
  double shininess_;
};

struct Triangle
{
  std::array<Vertex, 3> vertices;
};

struct Sphere
{
  std::array<double, 3> position_;
  std::array<double, 3> color_diffuse_;
  std::array<double, 3> color_specular_;
  double shininess_;
  double radius_;
};

struct Light
{
  std::array<double, 3> position_;
  std::array<double, 3> color_;
};

#endif // GEOMETRY_H
