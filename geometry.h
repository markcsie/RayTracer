#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <iostream>

#include <QVector3D>

const static float EPISILON = 0.00001;

struct Vertex
{
  QVector3D position_;
  QVector3D color_diffuse_;
  QVector3D color_specular_;
  QVector3D normal_;
  float shininess_;
};

struct Triangle
{
  std::array<Vertex, 3> vertices;

  bool intersect(const QVector3D &ray_origin, const QVector3D &unit_ray_direction, float &t) const
  {
    QVector3D unit_normal = QVector3D::crossProduct(vertices[1].position_ - vertices[0].position_, vertices[2].position_ - vertices[0].position_).normalized();
    float d = -QVector3D::dotProduct(vertices[0].position_, unit_normal);
    float nd = QVector3D::dotProduct(unit_normal, unit_ray_direction);
    if (nd == 0) { // parallel to the plane
      return false;
    }
    else
    {
      t = -(QVector3D::dotProduct(unit_normal, ray_origin) + d) / nd;
      if (t <= EPISILON) // the intersection is behind the ray origin
      {
        return false;
      }
      else
      {
        // test if the intersection is inside the triangle
        return isInside(ray_origin + unit_ray_direction * t);
      }
    }
  }

  bool isInside(const QVector3D &p) const
  {
    QVector3D v0 = vertices[2].position_ - vertices[0].position_;
    QVector3D v1 = vertices[1].position_ - vertices[0].position_;
    QVector3D v2 = p - vertices[0].position_;

    float dot00 = QVector3D::dotProduct(v0, v0);
    float dot01 = QVector3D::dotProduct(v0, v1);
    float dot02 = QVector3D::dotProduct(v0, v2);
    float dot11 = QVector3D::dotProduct(v1, v1);
    float dot12 = QVector3D::dotProduct(v1, v2);

    float inv_denom = 1 / (dot00 * dot11 - dot01 * dot01);

    float u = (dot11 * dot02 - dot01 * dot12) * inv_denom;
    float v = (dot00 * dot12 - dot01 * dot02) * inv_denom;

    return (u >= 0) && (v >= 0) && (u + v < 1);
  }

  float area() const
  {
    float area = QVector3D::crossProduct(vertices[1].position_ - vertices[0].position_, vertices[2].position_ - vertices[0].position_).length() / 2;
    assert(area >= 0);
    return area;
  }
};

struct Sphere
{
  QVector3D position_;
  QVector3D color_diffuse_;
  QVector3D color_specular_;
  float shininess_;
  float radius_;

  bool intersect(const QVector3D &ray_origin, const QVector3D &unit_ray_direction, float &t) const
  {
    const float a = std::pow(unit_ray_direction.length(), 2); // this should be 1 if we've normalized it before
    const QVector3D v = ray_origin - position_;
    const float b = 2 * QVector3D::dotProduct(unit_ray_direction, v);
    const float c = v.lengthSquared() - std::pow(radius_, 2);
    const float temp = std::pow(b, 2) - 4 * a * c;
    if (temp < 0)
    {
      return false;
    }
    else
    {
      const float t0 = std::max((-b + std::sqrt(temp)) / 2, 0.0f);
      const float t1 = std::max((-b - std::sqrt(temp)) / 2, 0.0f);

      t = std::min(t0, t1);
      return (t > EPISILON);
    }

  }
};

struct Light
{
  QVector3D position_;
  QVector3D color_;
};

#endif // GEOMETRY_H
