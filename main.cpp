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
  ray_tracer.loadScene(argv[1]);
  ray_tracer.drawImage();
  ray_tracer.getQImage().save("ggg.jpg");

  QLabel myLabel;
  myLabel.setPixmap(QPixmap::fromImage(ray_tracer.getQImage()));
  myLabel.show();

  std::cout << "gg" <<std::endl;

  return app.exec();
}
