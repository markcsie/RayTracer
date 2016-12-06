/* **************************
 * CSCI 420
 * Assignment 3 Raytracer
 * Name: <Kai-Chieh Ma>
 * *************************
*/

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <iostream>

#include <QApplication>
#include <QLabel>

#include "ray_tracer.h"

int main(int argc, char *argv[])
{    
  if (argc < 2)
  {
    std::cerr << "Please specify the scene file" << std::endl;
    return EXIT_FAILURE;
  }
  QApplication app(argc, argv);

  size_t max_sample_depth = 1;
  size_t max_ray_depth = 1;
  if (argc >= 3)
  {
    max_sample_depth = std::max(0, std::stoi(std::string(argv[2])));
  }
  if (argc >= 4)
  {
    max_ray_depth = std::max(0, std::stoi(std::string(argv[3])));
  }

  std::cout << "max_sample_depth " << max_sample_depth << std::endl;
  std::cout << "max_ray_depth " << max_ray_depth << std::endl;
  RayTracer ray_tracer(max_sample_depth, max_ray_depth);
//  RayTracer ray_tracer;
  std::string scene_file_name(argv[1]);
  ray_tracer.loadScene(scene_file_name);

  std::string image_file_name = scene_file_name.substr(0, scene_file_name.length() - 5) + "jpg";
  ray_tracer.drawImage();
  ray_tracer.getQImage().save(QString(image_file_name.c_str()));

  QLabel myLabel;
  myLabel.setFixedSize(ray_tracer.getQImage().size());
  myLabel.setPixmap(QPixmap::fromImage(ray_tracer.getQImage()));
  myLabel.show();

  return app.exec();
}
