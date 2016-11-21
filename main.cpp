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
#include <QPainter>

const size_t MAX_TRIANGLES = 20000;
const size_t MAX_SPHERES = 100;
const size_t MAX_LIGHTS = 100;

// you may want to make these smaller for debugging purposes
const size_t WIDTH = 640;
const size_t HEIGHT = 480;

// the field of view of the camera
const double fov = 60.0;

struct Vertex
{
    double position[3];
    double color_diffuse[3];
    double color_specular[3];
    double normal[3];
    double shininess;
};

struct Triangle
{
    Vertex v[3];
};

struct Sphere
{
    double position[3];
    double color_diffuse[3];
    double color_specular[3];
    double shininess;
    double radius;
};

struct Light
{
    double position[3];
    double color[3];
};

Triangle triangles[MAX_TRIANGLES];
Sphere spheres[MAX_SPHERES];
Light lights[MAX_LIGHTS];
double ambient_light[3];

size_t num_triangles = 0;
size_t num_spheres = 0;
size_t num_lights = 0;


void parse_check(const char *expected, char *found)
{
    if(strcasecmp(expected,found))
    {
        std::cout << "Expected: " << expected << "found: "  << found << std::endl;
        std::cout << "Parse error, abnormal abortion" <<std::endl;
        exit(0);
    }
}

void parse_doubles(FILE* file, const char *check, double p[3])
{
    char str[100];
    fscanf(file,"%s",str);
    parse_check(check,str);
    fscanf(file,"%lf %lf %lf",&p[0],&p[1],&p[2]);
    printf("%s %lf %lf %lf\n",check,p[0],p[1],p[2]);
}

void parse_rad(FILE *file, double *r)
{
    char str[100];
    fscanf(file,"%s",str);
    parse_check("rad:",str);
    fscanf(file,"%lf",r);
    printf("rad: %f\n",*r);
}

void parse_shi(FILE *file, double *shi)
{
    char s[100];
    fscanf(file,"%s",s);
    parse_check("shi:",s);
    fscanf(file,"%lf",shi);
    printf("shi: %f\n",*shi);
}

int loadScene(char *argv)
{
    FILE * file = fopen(argv,"r");
    int number_of_objects;
    char type[50];
    Triangle t;
    Sphere s;
    Light l;
    fscanf(file,"%i", &number_of_objects);

    printf("number of objects: %i\n",number_of_objects);

    parse_doubles(file,"amb:",ambient_light);

    for(int i=0; i<number_of_objects; i++)
    {
        fscanf(file,"%s\n",type);
        printf("%s\n",type);
        if(strcasecmp(type,"triangle")==0)
        {
            printf("found triangle\n");
            for(int j=0;j < 3;j++)
            {
                parse_doubles(file,"pos:",t.v[j].position);
                parse_doubles(file,"nor:",t.v[j].normal);
                parse_doubles(file,"dif:",t.v[j].color_diffuse);
                parse_doubles(file,"spe:",t.v[j].color_specular);
                parse_shi(file,&t.v[j].shininess);
            }

            if(num_triangles == MAX_TRIANGLES)
            {
                printf("too many triangles, you should increase MAX_TRIANGLES!\n");
                exit(0);
            }
            triangles[num_triangles++] = t;
        }
        else if(strcasecmp(type,"sphere")==0)
        {
            printf("found sphere\n");

            parse_doubles(file,"pos:",s.position);
            parse_rad(file,&s.radius);
            parse_doubles(file,"dif:",s.color_diffuse);
            parse_doubles(file,"spe:",s.color_specular);
            parse_shi(file,&s.shininess);

            if(num_spheres == MAX_SPHERES)
            {
                printf("too many spheres, you should increase MAX_SPHERES!\n");
                exit(0);
            }
            spheres[num_spheres++] = s;
        }
        else if(strcasecmp(type,"light")==0)
        {
            printf("found light\n");
            parse_doubles(file,"pos:",l.position);
            parse_doubles(file,"col:",l.color);

            if(num_lights == MAX_LIGHTS)
            {
                printf("too many lights, you should increase MAX_LIGHTS!\n");
                exit(0);
            }
            lights[num_lights++] = l;
        }
        else
        {
            printf("unknown type in scene description:\n%s\n",type);
            exit(0);
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{    
    QApplication app(argc, argv);

    QImage myImage(QSize(WIDTH, HEIGHT), QImage::Format_RGB888);

    QLabel myLabel;


    QPainter qPainter;
    qPainter.begin(&myImage);
    qPainter.setBrush(Qt::NoBrush);
    qPainter.setPen(Qt::red);
    qPainter.drawRect(0,0,300,300);
    qPainter.end();

    myImage.save("ggg.jpg");

    myLabel.setPixmap(QPixmap::fromImage(myImage));
    myLabel.show();

    std::cout << "gg" <<std::endl;

    return app.exec();
}
