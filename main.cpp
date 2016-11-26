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
  QApplication app(argc, argv);
  RayTracer ray_tracer;
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
