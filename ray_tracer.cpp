#include "ray_tracer.h"

#include <iostream>

#include <QPainter>

RayTracer::RayTracer()
: scene_file_(nullptr), image_(QSize(WIDTH, HEIGHT), QImage::Format_RGB888)
{

}

// TODO:
RayTracer::RayTracer(const RayTracer& other)
: image_(other.image_)
{
}

RayTracer::~RayTracer()
{
}

const QImage &RayTracer::getQImage()
{
  return image_;
}

void RayTracer::parseCheck(const std::string &expected, const std::string &found)
{
  if (expected != found)
  {
    std::cout << "Expected: " << expected << "found: "  << found << std::endl;
    std::cout << "Parse error, abnormal abortion" << std::endl;
    exit(0);
  }
}

void RayTracer::parse3doubles(const std::string &check, std::array<double, 3> &p)
{
  std::string str;
  scene_file_ >> str;
  parseCheck(check, str);
  scene_file_ >> p[0] >> p[1] >> p[2];
  std::cout << check << " " << p[0] << " " << p[1] << " " << p[2] << std::endl;
}

void RayTracer::parseRad(double &r)
{
  std::string str;
  scene_file_ >> str;
  parseCheck("rad:", str);
  scene_file_ >> r;
  std::cout << "rad: " << r << std::endl;
}

void RayTracer::parseShi(double &shi)
{
  std::string str;
  scene_file_ >> str;
  parseCheck("shi:", str);
  scene_file_ >> shi;
  std::cout << "shi: " << shi << std::endl;
}

void RayTracer::loadScene(const std::string &file_name)
{
  triangles_.clear();
  spheres_.clear();
  lights_.clear();

  scene_file_.open(file_name);

  int number_of_objects;
  scene_file_ >> number_of_objects;
  std::cout << "number of objects: " << number_of_objects << std::endl;
  assert(number_of_objects >= 0);

  parse3doubles("amb:", ambient_light_);
  for (size_t i = 0; i < static_cast<size_t>(number_of_objects); i++)
  {
    std::string type;
    scene_file_ >> type;
    std::cout << type << std::endl;
    if (type == "triangle")
    {
      std::cout << "found triangle" << std::endl;
      Triangle t;
      for (size_t j = 0; j < 3; j++)
      {
        parse3doubles("pos:", t.vertices[j].position_);
        parse3doubles("nor:", t.vertices[j].normal_);
        parse3doubles("dif:", t.vertices[j].color_diffuse_);
        parse3doubles("spe:", t.vertices[j].color_specular_);
        parseShi(t.vertices[j].shininess_);
      }

      triangles_.push_back(t);
    }
    else if (type == "sphere")
    {
      std::cout << "found sphere" << std::endl;
      Sphere s;
      parse3doubles("pos:", s.position_);
      parseRad(s.radius_);
      parse3doubles("dif:", s.color_diffuse_);
      parse3doubles("spe:", s.color_specular_);
      parseShi(s.shininess_);

      spheres_.push_back(s);
    }
    else if (type == "light")
    {
      std::cout << "found light" << std::endl;
      Light l;
      parse3doubles("pos:", l.position_);
      parse3doubles("col:", l.color_);

      lights_.push_back(l);
    }
    else
    {
      std::cout << "unknown type in scene description:" << std::endl << type << std::endl;
      exit(0);
    }
  }
}

void RayTracer::drawImage()
{
  QPainter qPainter;
  qPainter.begin(&image_);
  qPainter.setBrush(Qt::NoBrush);

  // -> x
  // |
  // y
  for (size_t x = 0; x < WIDTH / 2; x++) {
    for (size_t y = 0; y < HEIGHT/2; y++) {
      // compute view ray direction
      qPainter.setPen(QColor(255, 0, 255));
      qPainter.drawPoint(x, y);

      bool hit_triangle = true;
      size_t intersection_index = std::numeric_limits<size_t>::max();
      double min_distance = std::numeric_limits<double>::max();
      for (size_t i = 0; i < triangles_.size(); i++) {
//        double distance = Distance(eyePosition, pHit);
        double distance;
        if (distance < min_distance) {
            intersection_index = i;
            min_distance = distance; // update min distance
        }
      }

      for (size_t i = 0; i < spheres_.size(); i++) {
        double distance;
        if (distance < min_distance) {
            intersection_index = i;
            min_distance = distance; // update min distance
            hit_triangle = false;
        }
      }

      // The ray hits
      if (intersection_index != std::numeric_limits<size_t>::max()) {
        // compute illumination
        // shadow ray
        if (hit_triangle)
        {

        }
        else
        {

        }
      }

    }
  }
  qPainter.end();
}
