#ifndef RAY_TRACER_H
#define RAY_TRACER_H

// uncomment to disable assert()
// #define NDEBUG
#include <cassert>
#include <fstream>

#include "geometry.h"

#include <QImage>

class RayTracer
{
public:
  RayTracer(const size_t &max_sample_depth = 1, const size_t &max_ray_depth_ = 1);
  RayTracer(const RayTracer& other);
  virtual ~RayTracer();

  void loadScene(const std::string &file_name);
  void drawImage();
  const QImage &getQImage();

private:
  enum ObjectType {triangle, sphere};

  // you may want to make these smaller for debugging purposes
  const static size_t WIDTH = 640;
  const static size_t HEIGHT = 480;

  // the field of view of the camera
  constexpr static float FOV_DEGREE = 60.0;
  constexpr static float FOV_RADIAN = M_PI * FOV_DEGREE / 180.0;

  constexpr static float ASPECT_RATIO = WIDTH / static_cast<float>(HEIGHT);

  // By default, the camera is at (0, 0, 0) and points in the negative z-direction
  const static QVector3D CAMERA_POS;
  constexpr static float X_UNIT = (2 * ASPECT_RATIO * std::tan(FOV_RADIAN / 2)) / WIDTH;
  constexpr static float Y_UNIT = (-2 * std::tan(FOV_RADIAN / 2)) / HEIGHT;

  size_t max_sample_depth_;
  // Recursive
  size_t max_ray_depth_;

  std::ifstream scene_file_;
  QImage image_;

  std::vector<Triangle> triangles_;
  std::vector<Sphere> spheres_;
  std::vector<Light> lights_;
  QVector3D ambient_light_;

  void parseCheck(const std::string &expected, const std::string &found);
  void parseVector3D(const std::string &check, QVector3D &v);
  void parseRad(float &r);
  void parseShi(float &shi);

  QVector3D superSampling(const float &center_xx, const float &center_yy, const size_t &depth);
  QVector3D trace(const QVector3D &ray_origin, const QVector3D &ray_direction, const size_t &ray_depth);
  float intersect(const QVector3D &ray_origin, const QVector3D &unit_ray_direction, ObjectType &object_type, size_t &intersection_index);

};

#endif // RAY_TRACER_H
