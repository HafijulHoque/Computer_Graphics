#include<bits/stdc++.h>
#include <math.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <map>
#include <set>
#include "1905013_Header.h"
#include "bitmap_image.hpp"
#include <chrono>
using namespace std;
double position_speed_factor=1;

extern vector<Object*> object_list;
extern vector<Pointlight *> pointLights; // Defined point lights in the scene
extern vector <Spotlight *> spotLights; 
double initial_floor_width=1000.0;
double initial_tile_width=20.0;
double initial_floor_amb=0.4;
double initial_floor_diff=0.2;
double initial_floor_spec=0.2;
double initial_floor_refl=0.2;
double initial_floor_shine=4;
extern int recursion_level;
int img_width,img_height;
int total_num_of_objects;
double view_angle=80;
double window_height=500;
double window_width=500;
int num_of_captured_img=0;

Vector3d camera_position,look,up,side;

void loadData()
{
    //will be written accroading to spec
    ifstream file;
    file.open("scene.txt");
    Object *temp_object;
    if(file.is_open())
    {
        file>>recursion_level;
        file>>img_height;
        img_width=img_height;
        //first read the objects
        file>>total_num_of_objects;

        for(int i=0;i<total_num_of_objects;i++)
        {
            string shape_name;
            file>>shape_name;

            if(shape_name=="triangle")
            {
                double x,y,z;
                file>>x>>y>>z;
                Vector3d a(x,y,z);
                file>>x>>y>>z;
                Vector3d b(x,y,z);
                file>>x>>y>>z;
                Vector3d c(x,y,z);

                temp_object=new Triangle(a,b,c);
                //temp_object->print();
                
            }
            else if(shape_name=="sphere")
            {
                double x,y,z,r;
                file>>x>>y>>z>>r;
                Vector3d center(x,y,z);
                temp_object=new Sphere(center,r);
                                //temp_object->print();

            }
            else if(shape_name=="general")
            {

               Vector3d ref_point;
               double l,w,h;
               double A,B,C,D,E,F,G,H,I,J;
               file>>A>>B>>C>>D>>E>>F>>G>>H>>I>>J;
               file>>ref_point.x>>ref_point.y>>ref_point.z>>l>>w>>h;
                temp_object=new General(A,B,C,D,E,F,G,H,I,J,ref_point,l,w,h);
                                //temp_object->print();

            }
            double r,g,b;
            file>>r>>g>>b;
            temp_object->setcolor(r,g,b);
            double amb,diff,spec,refl;
            file>>amb>>diff>>spec>>refl;
            temp_object->setcoEfficients(amb,diff,spec,refl);
            double shine;
            file>>shine;
            temp_object->setShine(shine);
            object_list.push_back(temp_object);


        }
        //now for the point lights
        int total_point_lights;
        int total_spot_lights;
        Pointlight *temp_pointlight;
        Spotlight *temp_spotlight;


        file>>total_point_lights;
        
        for(int i=0;i<total_point_lights;i++)
        {
            double x,y,z;
             double r,g,b;
            file>>x>>y>>z;
            Vector3d position(x,y,z);
           
            file>>r>>g>>b;
            Color color(r,g,b);

            temp_pointlight=new Pointlight(position,color);

            pointLights.push_back(temp_pointlight);
            // temp_pointlight->print();
        }
        //now for the spot lights
        file>>total_spot_lights;
        for(int j=0;j<total_spot_lights;j++)
        {
            double x,y,z;
            double r,g,b;
            double x1,y1,z1;
            double angle;


            file>>x>>y>>z;
            Vector3d position(x,y,z);
            file>>r>>g>>b;
            Color color(r,g,b);
            file>>x1>>y1>>z1;
            Vector3d direction(x1,y1,z1);
            file>>angle;
           
            temp_spotlight=new Spotlight(position,direction,angle,color);
            spotLights.push_back(temp_spotlight);
            // temp_spotlight->print();
        }
        //now at last read the floor parameters
        //dynamicaly calculate floor parameters from num of objects and lights
        

        temp_object=new Floor(initial_floor_width,initial_tile_width);
        temp_object->setcoEfficients(initial_floor_amb,initial_floor_diff,initial_floor_spec,initial_floor_refl);
        temp_object->setShine(initial_floor_shine);
        object_list.push_back(temp_object);

        file.close();
    }
}

void capture()
{
        auto start = std::chrono::high_resolution_clock::now(); // Start timing
    bitmap_image *generated_img;

    generated_img=new bitmap_image(img_width,img_height);

    for(int i=0;i<img_height;i++)
    {
        for(int j=0;j<img_width;j++)
        {
            //height first and then width
            generated_img->set_pixel(i,j,0,0,0);
        }
    }

    double fovY_angle=tan(view_angle/2.00 * M_PI/180.0);
    double plane_distance=(window_height/2.00)/fovY_angle;

    Vector3d temp=look*plane_distance+up*(window_height/2.00)-(side*(window_height/2.00));

    Vector3d top_left=camera_position+temp;
    
     double dv=window_height/img_height;
    double du=window_width/img_width;
    Vector3d temp2=side*(du/2.00)-up*(dv/2.00);


    top_left=top_left+temp2;
           Vector3d current_pixel;

    for(int i=0;i<img_height;i++)
    {
        for(int j=0;j<img_width;j++)
        {
            double t_minimum=INFINITY;
            Color color(0,0,0);
            int nearest_object_index=-1;
            int object_index=0;

            Vector3d temp3=side*i*du-up*j*dv;

              current_pixel=top_left+temp3;

            Vector3d dir=current_pixel-camera_position;

            Ray casting_ray=Ray(camera_position,dir );
            casting_ray.normalize_ray();

            
            
            //print object list size
            for(Object *obj:object_list)
            {
                double current_t=obj->intersect(casting_ray,color,0);
                if(current_t>=0.0 )
                {
                    if((t_minimum-current_t)>EPSILON ){
                    t_minimum=current_t;
                    nearest_object_index=object_index;
                    }
                }
                object_index=object_index+1;
            }

            if(abs(t_minimum-INFINITY)>EPSILON)
            {
                object_list[nearest_object_index]->intersect(casting_ray,color,1);
               // color.print();
                generated_img->set_pixel(i,j,color.r*255,color.g*255,color.b*255);

            }

        }
        
    }
    cout<<"Image captured"<<endl;

    //save and then delete the image
    generated_img->save_image("Output_1"+to_string(num_of_captured_img)+".bmp");
    delete generated_img;
     auto stop = std::chrono::high_resolution_clock::now(); // End timing
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

    cout << "Time taken for ray tracing: " 
         << duration.count() << " milliseconds" << endl;

}
double camera_rotating_angle=3*M_PI/180.00;
void keyboardListener(unsigned char key,int x,int y)
{
    //pressing 1,2,3,4,5,6 performs camera functions
    //pressing 0 captures the image
    
    if(key=='1')
    {
        //rotating camera around up vector FOR left rotation
        look=look*cos(camera_rotating_angle)+(up.cross(look))*sin(camera_rotating_angle);
        side=side*cos(camera_rotating_angle)+(up.cross(side))*sin(camera_rotating_angle);

    }
    else if(key=='2')
    {
        //right rotation around up axis
        look=look*cos(-camera_rotating_angle)+(up.cross(look))*sin(-camera_rotating_angle);
        side=side*cos(-camera_rotating_angle)+(up.cross(side))*sin(-camera_rotating_angle);

    }
    else if (key=='3')
    {
        //keep side constant
        look=look*cos(camera_rotating_angle)+side.cross(look)*sin(camera_rotating_angle);
        up=up*cos(camera_rotating_angle)+side.cross(up)*sin(camera_rotating_angle);
    }
    else if(key=='4')
    {
        //keep side constant
        look=look*cos(-camera_rotating_angle)+side.cross(look)*sin(-camera_rotating_angle);
        up=up*cos(-camera_rotating_angle)+side.cross(up)*sin(-camera_rotating_angle);
    }
    else if(key=='5')
    {
        //keep look constant
        up=up*cos(camera_rotating_angle)+look.cross(up)*sin(camera_rotating_angle);
        side=side*cos(camera_rotating_angle)+look.cross(side)*sin(camera_rotating_angle);
       
    }
    else if(key=='6')
    {
        //keep look constant
        up=up*cos(-camera_rotating_angle)+look.cross(up)*sin(-camera_rotating_angle);
        side=side*cos(-camera_rotating_angle)+look.cross(side)*sin(-camera_rotating_angle);
      
    }
    else if(key=='0')
    {
        num_of_captured_img++;
        capture();
    }
    else if (key=='+')
    {
        position_speed_factor+=1;
    }
    else if(key=='-')
    {
        position_speed_factor-=1;
    }
    else if(key=='i')
    {
        //increase the camera rotatin factor
        camera_rotating_angle+=1;

    
    }
    else if(key=='d')
    {
        //decrease the camera rotatin factor
        camera_rotating_angle-=1;
    }
    else if(key == 'z') {
        view_angle = max(30.0, view_angle - 5); // Ensure minimum angle to avoid inversion
    }
    // Zoom out
    else if(key == 'x') {
        view_angle = min(120.0, view_angle + 5); // Ensure maximum angle to avoid too much distortion
    }
    else if(key=='r')
    {
        Vector3d new_up = up * cos(camera_rotating_angle) + side * sin(camera_rotating_angle);
            Vector3d new_side = side * cos(camera_rotating_angle) - up * sin(camera_rotating_angle);
            up = new_up.normalize();
            side = new_side.normalize();
    }
    else if(key=='n')
    {
           Vector3d new_up = up * cos(-camera_rotating_angle) + side * sin(-camera_rotating_angle);
            Vector3d new_side = side * cos(-camera_rotating_angle) - up * sin(-camera_rotating_angle);
            up = new_up.normalize();
            side = new_side.normalize();

    }
}
    void specialKeyListener(int key,int x,int y)
    {
        if(key==GLUT_KEY_DOWN)
        {
            camera_position=camera_position-look*position_speed_factor;
        }
        else if(key==GLUT_KEY_UP)
        {
            camera_position=camera_position+look*position_speed_factor;
        }
        else if(key==GLUT_KEY_RIGHT)
        {
            camera_position=camera_position+side*position_speed_factor;
        }
        else if(key==GLUT_KEY_LEFT)
        {
            camera_position=camera_position-side*position_speed_factor;
        }
        else if(key==GLUT_KEY_PAGE_UP)
        {
            camera_position=camera_position+up*position_speed_factor;
        }
        else if(key==GLUT_KEY_PAGE_DOWN)
        {
            camera_position=camera_position-up*position_speed_factor;
        }

    }


    void draw_all_shapes()
{
    for(Object *obj:object_list)
    {
        obj->draw();
    }
    for(Pointlight *pl:pointLights)
    {
        pl->draw();
    }
    for(Spotlight *sl:spotLights)
    {
        sl->draw();
    }

}

void draw_axis()
{
    //draw the axis with white color
    glBegin(GL_LINES);
    {

        glColor3f(1.0, 0, 0);
        glVertex3f( 100,0,0);
        glVertex3f(-100,0,0);

        glColor3f(0, 1.0, 0);
        glVertex3f(0,-100,0);
        glVertex3f(0, 100,0);

        glColor3f(0, 0, 1.0);
        glVertex3f(0,0, 100);
        glVertex3f(0,0,-100);
    }glEnd();
}



void display()
{
    //clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();
    //lookat
    gluLookAt(camera_position.x,camera_position.y,camera_position.z,
              camera_position.x+look.x,camera_position.y+look.y,camera_position.z+look.z,
              up.x,up.y,up.z);
    glMatrixMode(GL_MODELVIEW);
    draw_axis();
    draw_all_shapes();
    glutSwapBuffers();

}

void animate()
{
    //codes for any changes in Models, Camera
    glutPostRedisplay();
}
void init()
{
    
    camera_position=Vector3d(100,100,100);

    up=Vector3d(0.0,0.0,1.0);
    side=Vector3d(-1.0,1.0,0.0);
    side=side.normalize();
    look=Vector3d(-1.0,-1.0,0);
    look=look.normalize();

    loadData();

    glClearColor(0,0,0,0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80,1,1,1000.0);
}
int main(int argc, char **argv)
{
   glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(60, 60);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

    glutCreateWindow("Offline 3");
    init();
    glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(display);
    glutIdleFunc(animate);
    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutMainLoop();
    return 0;
}



