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
  RayTracer();
  RayTracer(const RayTracer& other);
  virtual ~RayTracer();

  void loadScene(const std::string &file_name);
  void drawImage();
  const QImage &getQImage();

private:
  // you may want to make these smaller for debugging purposes
  const static size_t WIDTH = 640;
  const static size_t HEIGHT = 480;

  // the field of view of the camera
  constexpr static double fov = 60.0;

  std::ifstream scene_file_;
  QImage image_;

  std::vector<Triangle> triangles_;
  std::vector<Sphere> spheres_;
  std::vector<Light> lights_;
  std::array<double, 3> ambient_light_;

  void parseCheck(const std::string &expected, const std::string &found);
  void parse3doubles(const std::string &check, std::array<double, 3> &p);
  void parseRad(double &r);
  void parseShi(double &shi);
};

#endif // RAY_TRACER_H
