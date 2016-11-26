#include "ray_tracer.h"

#include <iostream>

#include <QPainter>

const QVector3D RayTracer::CAMERA_POS = QVector3D(0.0, 0.0, 0.0);

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

void RayTracer::parseVector3D(const std::string &check, QVector3D &v)
{
  std::string str;
  scene_file_ >> str;
  parseCheck(check, str);
  scene_file_ >> v[0] >> v[1] >> v[2];
  std::cout << check << " " << v[0] << " " << v[1] << " " << v[2] << std::endl;
}

void RayTracer::parseRad(float &r)
{
  std::string str;
  scene_file_ >> str;
  parseCheck("rad:", str);
  scene_file_ >> r;
  std::cout << "rad: " << r << std::endl;
}

void RayTracer::parseShi(float &shi)
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

  parseVector3D("amb:", ambient_light_);
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
        parseVector3D("pos:", t.vertices[j].position_);
        parseVector3D("nor:", t.vertices[j].normal_);
        parseVector3D("dif:", t.vertices[j].color_diffuse_);
        parseVector3D("spe:", t.vertices[j].color_specular_);
        parseShi(t.vertices[j].shininess_);
      }

      triangles_.push_back(t);
    }
    else if (type == "sphere")
    {
      std::cout << "found sphere" << std::endl;
      Sphere s;
      parseVector3D("pos:", s.position_);
      parseRad(s.radius_);
      parseVector3D("dif:", s.color_diffuse_);
      parseVector3D("spe:", s.color_specular_);
      parseShi(s.shininess_);

      spheres_.push_back(s);
    }
    else if (type == "light")
    {
      std::cout << "found light" << std::endl;
      Light l;
      parseVector3D("pos:", l.position_);
      parseVector3D("col:", l.color_);

      lights_.push_back(l);
    }
    else
    {
      std::cout << "unknown type in scene description:" << std::endl << type << std::endl;
      exit(0);
    }
  }
}

float RayTracer::intersect(const QVector3D &ray_origin, const QVector3D &unit_ray_direction, ObjectType &object_type, size_t &intersection_index)
{
//  std::cout << "unit_ray_direction " << unit_ray_direction.length() << std::endl;
  object_type = sphere;
  intersection_index = -1;
  float min_t_near = std::numeric_limits<float>::max();
  for (size_t i = 0; i < spheres_.size(); i++) {
    float t = 0;
    if (spheres_[i].intersect(ray_origin, unit_ray_direction, t))
    {
      if (t < min_t_near)
      {
        min_t_near = t;
        intersection_index = i;
      }
    }
  }

  for (size_t i = 0; i < triangles_.size(); i++) {
    float t = 0;
    if (triangles_[i].intersect(ray_origin, unit_ray_direction, t))
    {
      if (t < min_t_near)
      {
        min_t_near = t;
        intersection_index = i;
        object_type = triangle;
      }
    }
  }

  return min_t_near;
}

QColor RayTracer::trace(const QVector3D &ray_origin, const QVector3D &ray_direction)
{
  // normalize the direction vector
  QVector3D unit_ray_direction = ray_direction.normalized();

  ObjectType object_type;
  size_t intersection_index;
  float t = intersect(ray_origin, unit_ray_direction, object_type, intersection_index);
  assert(t >= 0);

  // The ray hits
  if (intersection_index != -1) {
//    std::cout << "object_type " << object_type << std::endl;
//    std::cout << "intersection_index " << intersection_index << std::endl;
    const QVector3D intersection_point = ray_origin + t * unit_ray_direction;
//    std::cout << "intersection_point " << intersection_point[0] << " " << intersection_point[1] << " " << intersection_point[2] << std::endl;

    // compute illumination
    QVector3D illumination_value(0.0, 0.0, 0.0);
    for (size_t i = 0; i < lights_.size(); i++)
    {
//      std::cout << "lights_[i].position_ " << lights_[i].position_[0] << " " << lights_[i].position_[1] << " " << lights_[i].position_[2] << std::endl;
      QVector3D unit_l = (lights_[i].position_ - intersection_point).normalized();
      ObjectType dummy_object_type;
      size_t shadow_intersection_index;
      float light_t = (lights_[i].position_ - intersection_point).length();
      float t = intersect(intersection_point, unit_l, dummy_object_type, shadow_intersection_index); // TODO: optimize?
//      std::cout << "t " << t << std::endl;
      // shadow ray is not blocked, either there's no intersection, or the object is further from the light source
      if (shadow_intersection_index == -1 || t > light_t)
      {
        if (object_type == sphere)
        {
          QVector3D unit_v = (ray_origin - intersection_point).normalized();
          QVector3D unit_n = (intersection_point - spheres_[intersection_index].position_).normalized();
          float ln = std::max(QVector3D::dotProduct(unit_l, unit_n), 0.0f);
          QVector3D unit_r = 2 * ln * unit_n - unit_l;
          float rv = std::max(QVector3D::dotProduct(unit_r, unit_v), 0.0f);
          QVector3D kd = spheres_[intersection_index].color_diffuse_;
          QVector3D ks = spheres_[intersection_index].color_specular_;
          float sh = spheres_[intersection_index].shininess_;

          // TODO: attenuation
          float attenuation = 1.0;
          illumination_value += attenuation * lights_[i].color_ * (kd * (ln) + ks * std::pow(rv, sh));
//          std::cout << "illumination_value " << illumination_value[0] << " " << illumination_value[1] << " " << illumination_value[2] << std::endl;
        }
        else if (object_type == triangle)
        {
          Triangle alpha_triangle;
          alpha_triangle.vertices[0].position_ = intersection_point;
          alpha_triangle.vertices[1].position_ = triangles_[intersection_index].vertices[1].position_;
          alpha_triangle.vertices[2].position_ = triangles_[intersection_index].vertices[2].position_;

          Triangle beta_triangle;
          beta_triangle.vertices[0].position_ = triangles_[intersection_index].vertices[0].position_;
          beta_triangle.vertices[1].position_ = intersection_point;
          beta_triangle.vertices[2].position_ = triangles_[intersection_index].vertices[2].position_;

          Triangle gamma_triangle;
          gamma_triangle.vertices[0].position_ = triangles_[intersection_index].vertices[0].position_;
          gamma_triangle.vertices[1].position_ = triangles_[intersection_index].vertices[1].position_;
          gamma_triangle.vertices[2].position_ = intersection_point;

          float whole_area = triangles_[intersection_index].area();
          float alpha = alpha_triangle.area() / whole_area;
          float beta = beta_triangle.area() / whole_area;
          float gamma = gamma_triangle.area() / whole_area;
//          std::cout << "alpha " << alpha << " beta " << beta << " gamma " << gamma << std::endl;

          QVector3D unit_v = (ray_origin - intersection_point).normalized();
          QVector3D unit_n = (alpha * triangles_[intersection_index].vertices[0].normal_ + beta * triangles_[intersection_index].vertices[1].normal_ + gamma * triangles_[intersection_index].vertices[2].normal_).normalized();
          float ln = std::max(QVector3D::dotProduct(unit_l, unit_n), 0.0f);
          QVector3D unit_r = 2 * ln * unit_n - unit_l;
          float rv = std::max(QVector3D::dotProduct(unit_r, unit_v), 0.0f);

          QVector3D kd = alpha * triangles_[intersection_index].vertices[0].color_diffuse_ + beta * triangles_[intersection_index].vertices[1].color_diffuse_ + gamma * triangles_[intersection_index].vertices[2].color_diffuse_;
          QVector3D ks = alpha * triangles_[intersection_index].vertices[0].color_specular_ + beta * triangles_[intersection_index].vertices[1].color_specular_ + gamma * triangles_[intersection_index].vertices[2].color_specular_;
          float sh = alpha * triangles_[intersection_index].vertices[0].shininess_ + beta * triangles_[intersection_index].vertices[1].shininess_ + gamma * triangles_[intersection_index].vertices[2].shininess_;

          // TODO: attenuation
          illumination_value += lights_[i].color_ * (kd * (ln) + ks * std::pow(rv, sh));
        }
      }
    }
    illumination_value += ambient_light_;
//    std::cout << "illumination_value " << illumination_value[0] << " " << illumination_value[1] << " " << illumination_value[2] << std::endl;
    QColor final_color;
    final_color.setRedF(std::min(illumination_value[0], 1.0f));
    final_color.setGreenF(std::min(illumination_value[1], 1.0f));
    final_color.setBlueF(std::min(illumination_value[2], 1.0f));
//    std::cout << "final_color " << final_color.red() << " " << final_color.green() << " " << final_color.blue() << std::endl;
    return final_color;
  }

  // background color
  return QColor(255, 255, 255);
}

void RayTracer::drawImage()
{
  QPainter qPainter;
  qPainter.begin(&image_);
  qPainter.setBrush(Qt::NoBrush);
  std::cout << "ASPECT_RATIO " << ASPECT_RATIO << std::endl;
  std::cout << "X_UNIT " << X_UNIT << std::endl;
  std::cout << "Y_UNIT " << Y_UNIT << std::endl;
  std::cout << "lights_.size() " << lights_.size() << std::endl;

  // --> x
  // |
  // y
  std::cout << "progress: " << std::endl;
  size_t current_progress = -1;
  for (size_t x = 0; x < WIDTH; x++) {
    for (size_t y = 0; y < HEIGHT; y++) {
      // compute view ray direction
      float xx = -ASPECT_RATIO * std::tan(FOV_RADIAN / 2) + (x + 0.5) * X_UNIT;
      float yy = std::tan(FOV_RADIAN / 2) + (y + 0.5) * Y_UNIT;

      qPainter.setPen(trace(CAMERA_POS, {xx, yy, -1}));
      qPainter.drawPoint(x, y);
    }
    unsigned int progress = std::ceil((x / static_cast<float>(WIDTH)) * 100);
    if (progress % 10 == 0 && current_progress != progress)
    {
      current_progress = progress;
      std::cout << current_progress << "%" << std::endl;
    }
  }
  qPainter.end();
}
