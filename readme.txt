Assignment #3: Ray tracing

FULL NAME: Kai-Chieh Ma


MANDATORY FEATURES
------------------

<Under "Status" please indicate whether it has been implemented and is
functioning correctly.  If not, please explain the current status.>

Feature:                                 Status: finish? (yes/no)
-------------------------------------    -------------------------
1) Ray tracing triangles                  yes

2) Ray tracing sphere                     yes

3) Triangle Phong Shading                 yes

4) Sphere Phong Shading                   yes

5) Shadows rays                           yes

6) Still images                           yes
   
7) Antialiasing using super sampling      yes 
  Please execute the program with arguments. 
  For example: 
  Super sampling with depth "2": 
  "./RayTracer.exe test2.scene 2"

8) Recursive reflection				      yes
  Please execute the program with arguments. 
  For example: 
  Recursive reflection with depth "2": 
  "./RayTracer.exe test2.scene 1 2"
  
Images:
001.jpg: test1.scene
002.jpg: test2.scene
003.jpg: table.scene
004.jpg: spheres.scene
005.jpg: SIGGRAPH.scene
006.jpg: test2.scene with 2-depth super sampling . Compared to 002.jpg, the edges are smoother.
007.jpg: table.scene with 2-depth super sampling. Compared to 003.jpg, the edges are smoother.
008.jpg: spheres.scene with 2-depth super sampling. Compared to 004.jpg, the edges are smoother.
009.jpg: test2.scene with 2-depth recursive reflection. The lower part of sphere is hit by the reflected rays from the table.
010.jpg: table.scene with 2-depth recursive reflection. The lower parts of the boxes are hit by the reflected rays from the table.