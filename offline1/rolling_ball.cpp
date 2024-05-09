#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <GL/glut.h>
#define PI 2 * acos(0.0)
#include <vector>
// we are going to build a magic cube
class Point
{
    double x;
    double y;
    double z;

public:
    Point()
    {
        x = 0;
        y = 0;
        z = 0;
    }

    Point(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    double getX() const
    {
        return x;
    }

    double getY() const
    {
        return y;
    }

    double getZ() const
    {
        return z;
    }

    Point operator+(const Point _vector);
    Point operator-(const Point _vector);
    Point operator*(const double scalar);
    Point operator^(const Point _vector);
    double operator/(const Point _vector);

    ~Point()
    {
        x = y = z = 0.0;
    }
};
double Point::operator/(const Point vector)
{
    //this is the dot product
    return (x * vector.x + y * vector.y + z * vector.z);

}
Point Point::operator+(const Point vector)
{
    return Point(x + vector.x, y + vector.y, vector.z + z);
}
Point Point::operator-(const Point vector)
{
    return Point(x - vector.x, y - vector.y, z - vector.z);
}
Point Point::operator*(const double vector)
{
    return Point(x * vector, y * vector, z * vector);
}
Point Point ::operator^(const Point vector)
{
    // this is vector cross product
    return Point(y * vector.z - z * vector.y, z * vector.x - x * vector.z, x * vector.y - y * vector.x);
}
Point position_camera;
Point look;
Point up;
Point right;
Point direction;
bool flag = false;
bool isCollisionScheduled = false;
double collisionTime = 0.0;

  // Initial velocity
const double time_step = 0.05;

int variable_radius = 1;
int count=0;
double ball_velocity=0.5;
   GLfloat centers[6][3] =
        {(0, 0, 0),
         (0, 0, 0),
         (0, 0, 0),
         (0, 0, 0),
         (0, 0, 0),
         (0, 0, 0)};



void drawFloor()
{
    int size = 10; // Adjust the size of the checkerboard squares
    int numSquares = 20; // Adjust the number of squares in each direction

    for (int i = -numSquares; i < numSquares; ++i)
    {
        for (int j = -numSquares; j < numSquares; ++j)
        {
            if ((i + j) % 2 == 0)
                glColor3f(1.0, 1.0, 1.0); // White
            else
                glColor3f(0.0, 0.0, 0.0); // Black

            glBegin(GL_QUADS);
            {
                glVertex3d(i * size, j * size, -2);
                glVertex3d((i + 1) * size, j * size, -2);
                glVertex3d((i + 1) * size, (j + 1) * size, -2);
                glVertex3d(i * size, (j + 1) * size, -2);
            }
            glEnd();
        }
    }
}
int max_x = 40;
int max_y = 40;

void drawBilliardBoard()
{

    //purple color
    glColor3f(1.0, 0.0, 1.0);
    glLineWidth(2.0);

    glBegin(GL_LINES);
    {
        // Bottom boundary
        glVertex3d(-max_x, -max_y, 0);
        glVertex3d(max_x, -max_y, 0);

        // Top boundary
        glVertex3d(-max_x, max_y,0);
        glVertex3d(max_x, max_y, 0);

        // Left boundary
        glVertex3d(-max_x, -max_y, 0);
        glVertex3d(-max_x, max_y, 0);

        // Right boundary
        glVertex3d(max_x, -max_y, 0);
        glVertex3d(max_x, max_y, 0);
    }
    glEnd();

    glLineWidth(1.0);
}


void drawArrow()
{
    double factor = 2.0;
    glColor3f(0.0, 1.0, 0.0); // Green color for the arrow
    glLineWidth(5.0); // Set the line width to 5.0 (adjust as needed)

    // Calculate the end position of the arrow
    Point end = Point(centers[0][0], centers[0][1], centers[0][2]) + direction * factor;

    // Draw the arrow using a line
    glBegin(GL_LINES);
    {
        glVertex3d(centers[0][0], centers[0][1], centers[0][2]);
        glVertex3d(end.getX(), end.getY(), end.getZ());
    }
    glEnd();

    // Reset line width to default
    glLineWidth(1.0);
}




void draw_subsphere(double a, double b, double c)
{
    // latitudinal plane is rotated from -45 to 45 degree
    //latitudinal plane is horizontal plane 
    //longitudinal plane is vertical  plane from north pole to south pole


    //longitudinal plane is rotated from -45 to 45 degree

    // number of divisions for sphere
    int number_of_divisions = 7;
    // degree to radian conversion
    float degree_to_radian = acos(-1) / 180.00;
    // total number of points which is 2^(number_of_divisions)+1

    int total_points = (int)pow(2, number_of_divisions) + 1;
    // array of points
    Point sphere_points[total_points][total_points];

    Point normal_of_long_plane;
    Point normal_of_lat_plane;
    Point normal_of_intersection;
    double long_angle, lat_angle;

    // rotate latitudinal plane from -45 to 45 degree with respect to z axis
    for (int i = 0; i < total_points; ++i)
    {

        lat_angle = (45.0) - 90.0 * i / (total_points - 1);
        lat_angle *= degree_to_radian;
        normal_of_lat_plane = Point(-sin(lat_angle),
                                    cos(lat_angle), 0);

        for (int j = 0; j < total_points; ++j)
        {
            long_angle = (-45.0) + 90.0 * j / (total_points - 1);
            long_angle *= degree_to_radian;
            normal_of_long_plane = Point(-sin(long_angle),
                                         0, -cos(long_angle));

            normal_of_intersection = normal_of_long_plane ^ normal_of_lat_plane;
            normal_of_intersection = normal_of_intersection *
                                     (1.00 / sqrt(normal_of_intersection.getX() * normal_of_intersection.getX() + normal_of_intersection.getY() * normal_of_intersection.getY() + normal_of_intersection.getZ() * normal_of_intersection.getZ()));
            sphere_points[i][j] = normal_of_intersection;
        }
    }
    // now draw the sphere
    glBegin(GL_QUADS);
    {
        glColor3f(a, b, c);
        for (int i = 1; i < total_points; ++i)
        {
            for (int j = 1; j < total_points; ++j)
            {
                glVertex3d(sphere_points[i][j].getX(),
                 sphere_points[i][j].getY(), 
                 sphere_points[i][j].getZ());

                glVertex3d(sphere_points[i][j - 1].getX(),
                 sphere_points[i][j - 1].getY(),
                  sphere_points[i][j - 1].getZ());

                glVertex3d(sphere_points[i - 1][j - 1].getX(),
                 sphere_points[i - 1][j - 1].getY(), 
                 sphere_points[i - 1][j - 1].getZ());
                 
                glVertex3d(sphere_points[i - 1][j].getX(), 
                sphere_points[i - 1][j].getY(),
                 sphere_points[i - 1][j].getZ());
            }
        }
    }
    glEnd();
}

//angles were calculated by 
//0 60 120 180 240 300
int angles[6]={0,90,180,270,90,270};
void draw_sphere()
{
    


    GLfloat axis[6][3] = {{0, 1, 0},
                          {0, 1, 0},
                          {0, 1, 0},
                           {0,1,0},
                          {0, 0, 1},
                          {0, 0, 1}};

                      
 
    for (int i = 0; i < 6; i++)
    {
        glPushMatrix();
        {
            glTranslatef(centers[i][0],
                         centers[i][1], centers[i][2]);
            // glScalef(variable_radius,
            //  variable_radius, variable_radius);
             // Rotate based on movement direction

            // Update the rotation angle for rolling effect
            
            glRotatef(angles[(i)], axis[i][0], axis[i][1], axis[i][2]);
            // if(flag==true)
            // {
            //     glRotatef(30, axis[i][0], axis[i][1], axis[i][2]);
            //     //store the new rotation angle
            //     angles[i]=(angles[i]+30)%360;
            //     flag=false;
            // }
            if(i==0)
            {
                //green
                draw_subsphere(0,1,0);
            }
            else if (i==1)
            {
                //red 
                draw_subsphere(1,0,0);
                
            }
            else if(i==2)
            {
                //blue
                draw_subsphere(0,0,1);
            }
            else if(i==3)
            {
                //cyan
                draw_subsphere(0,1,1);
            }
            else if (i==4)
            {
                //yellow
                draw_subsphere(1,1,0);
            }
            else if(i==5)
            {
              //majenta
                draw_subsphere(1,0,1);
            }
            else if(i==8)
            {
                //cyan
                draw_subsphere(0,1,1);
                
            }
        }
        glPopMatrix();
    }

}
int temp=0;
void display()
{
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(position_camera.getX(), position_camera.getY(), position_camera.getZ(), position_camera.getX() + look.getX(), look.getY() + position_camera.getY(), look.getZ() + position_camera.getZ(), up.getX(), up.getY(), up.getZ());

    glPushMatrix();
    {
        glRotatef(0, 0, 1, 0);
        

       drawBilliardBoard();

         
       drawFloor();
       
       

        draw_sphere();

        drawArrow();
    }
    
    glPopMatrix();
    glutSwapBuffers();
}
void initialization()
{
    /* codes for initialization */

    position_camera = Point(2, 2, 2);
    up = Point(0.0, 0.0, 1.0);
	right = Point(-1.0/sqrt(2.0), 1.0/sqrt(2.0), 0.0);
	look = Point(-1.0/sqrt(2.0), -1.0/sqrt(2.0), 0.0);
    direction=Point(4,3,0);

    /* clear the screen */
    glClearColor(0, 0, 0, 0);

    /* setting up projection here */

    /* load the PROJECTION matrix */
    glMatrixMode(GL_PROJECTION);

    /* initialize the matrix */
    glLoadIdentity();

    /*
        give PERSPECTIVE parameters:
            1. field of view in the Y (vertically)
            2. aspect ratio that determines the field of view in the X direction (horizontally)
            3. near distance
            4. far distance
    */
    gluPerspective(80, 1, 1.0, 1000.0);
}
void keyboard1(unsigned char key,int x,int y)
{
    if(key=='1')
    {
        //rotate left means rotate around up vector in clockwise direction
        //so we need to rotate look and right vector
        right=right*cos(PI/60)+(up^right)*sin(PI/60);
        look=look*cos(PI/60)-(up^look)*sin(PI/60);

    }
    else if(key=='2')
    {
        right= right*cos(-PI/60)+(up^right)*sin(-PI/60);
        look=look*cos(-PI/60)-(up^look)*sin(-PI/60);


    }
    else if(key=='3')
    {
        //look up means rotate 
        //around right vector in clockwise direction
        look=look*cos(PI/60)+(right^look)*sin(PI/60);
        up=up*cos(PI/60)+(right^up)*sin(PI/60);


    }
    else if(key=='4')
    {
        look=look*cos(-PI/60)+(right^look)*sin(-PI/60);
        up=up*cos(-PI/60)+(right^up)*sin(-PI/60);
    }
    else if (key=='5')
    {
        //tilt clockwise means rotate around look vector in clockwise direction
        right=right*cos(PI/60)+(look^right)*sin(PI/60);
        up=up*cos(PI/60)+(look^up)*sin(PI/60);
        
    }
    else if(key=='6')
    {
        right=right*cos(-PI/60)+(look^right)*sin(-PI/60);
        up=up*cos(-PI/60)+(look^up)*sin(-PI/60);
    }
     else if (key == 'i')
    {
        int step_size = 1;
        // Move forward the centers of the spheres along the look vector
        for (int i = 0; i < 6; i++)
        {
            centers[i][0] += direction.getX() * step_size;
            centers[i][1] += direction.getY() * step_size;
            centers[i][2] += direction.getZ() * step_size;
        }
       //naive way for rolling effect
    //    count++;
    //when i pressed increment all the angles by 90 degree
       for(int i=0;i<4;i++)

       {
           angles[i]=(angles[i]+90)%360;
       }
       //for the last two angles increase by  180 degree
         for(int i=4;i<6;i++)
         {
              angles[i]=(angles[i]+180)%360;
         }



    
      
         
    }
    else if(key=='k')
    {
          int step_size = 0.4;
        // Move forward the centers of the spheres along the look vector
        for (int i = 0; i < 6; i++)
        {
            centers[i][0] -= direction.getX() * step_size;
            centers[i][1] -= direction.getY() * step_size;
            centers[i][2] -= direction.getZ() * step_size;
        }
        // count++;
          for(int i=0;i<4;i++)

       {
           angles[i]=(angles[i]+90)%360;
       }
       //for the last two angles increase by  180 degree
         for(int i=4;i<6;i++)
         {
              angles[i]=(angles[i]+180)%360;
         }

    }
    //when  pressed j direction will rotate clockwise
    else if(key=='j')
    {
        //rotate left means rotate around up vector in clockwise direction
        //so we need to rotate look and right vector
        direction=direction*cos(PI/60)+(up^direction)*sin(PI/60);
        

    }
    else if(key=='l')
    {
        direction=direction*cos(-PI/60)+(up^direction)*sin(-PI/60);
    }
        

  

}
void keyboard2(int key,int x,int y)
{
    if(key==GLUT_KEY_UP)
    {
        position_camera=position_camera+look;


    }
    else if(key==GLUT_KEY_DOWN)
    {
        position_camera=position_camera-look;
        
    }
    else if (key==GLUT_KEY_LEFT)
    {
        position_camera=position_camera-right;
        
    }
    else if(key==GLUT_KEY_RIGHT)
    {
        position_camera=position_camera+right;
    }
    else if (key==GLUT_KEY_PAGE_UP)
    {
        position_camera=position_camera+up;
    }
    else if(key==GLUT_KEY_PAGE_DOWN)
    {
        position_camera=position_camera-up;
    }
    else if(key==27)
    {
        exit(0);
    }
    
}

   int xy=0;
void scheduleCollisionEvent(double t) {
    isCollisionScheduled = true;
    collisionTime = t;
}
void update_ball_position()
{
    printf("updating ball position\n");
    //print ball position
    printf("ball position is %lf %lf %lf\n",centers[0][0],centers[0][1],centers[0][2]);
    for (int current_center = 0; current_center < 6; current_center++)
    {
         centers[current_center][0] += direction.getX() * ball_velocity;
        centers[current_center][1] += direction.getY() * ball_velocity;
        
        centers[current_center][2] += direction.getZ() * ball_velocity;
    }
          for(int i=0;i<4;i++)

       {
           angles[i]=(angles[i]+90)%360;
       }
       //for the last two angles increase by  180 degree
         for(int i=4;i<6;i++)
         {
              angles[i]=(angles[i]+180)%360;
         }
         if(!isCollisionScheduled){
    double nextCollisionTimeY = (max_y - centers[0][1]) / (direction.getY()*ball_velocity);
    double nextCollisionTimeX = (max_x - centers[0][0]) / (direction.getX()*ball_velocity);
    //print the next collision time
    printf("next collision time is %lf\n",fmin(nextCollisionTimeX, nextCollisionTimeY));
    //check whether the  time is too small or not
    
    //check collision time positive or not
    
      if(fmin(nextCollisionTimeX, nextCollisionTimeY)>0){
    double nextCollisionTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0 +
                               fmin(nextCollisionTimeX, nextCollisionTimeY)/1000;
    //check which is minimum and set xy
    if(nextCollisionTimeX<nextCollisionTimeY)
    {
        xy=1;
    }
    else
    {
        xy=0;
    }
                        

    scheduleCollisionEvent(nextCollisionTime);
      }
      else
      {
            //again update the ball position
            for (int current_center = 0; current_center < 6; current_center++)
    {

        centers[current_center][0] += direction.getX() * ball_velocity;
        centers[current_center][1] += direction.getY() * ball_velocity;
        
        centers[current_center][2] += direction.getZ() * ball_velocity;

      }
        //update the angles
    for(int i=0;i<4;i++)

         {
              angles[i]=(angles[i]+90)%360;
         }
            //for the last two angles increase by  180 degree
            for(int i=4;i<6;i++)
            {
                    angles[i]=(angles[i]+180)%360;
            }


}
         
         }
}

void handleCollision()
{
    printf("handling collision\n");
    isCollisionScheduled = false;
    // Update the rotation angle for rolling effect
    Point normal;
    if(xy==0){
         normal = Point(0, 1, 0); // Assuming the wall is in the y-direction
    } else
    {
     normal = Point(1, 0, 0); // Assuming the wall is in the x-direction
    }
            double dotProduct = direction / normal;
            direction = direction - (normal * (2 * dotProduct));

//check xy and update the  6 centers to max_x or max_y
    if(xy==0)
    {
        //update y
        for (int current_center = 0; current_center < 6; current_center++)
        {
            centers[current_center][1] = max_y-2;
        }
    }
    else
    {
        //update x
        for (int current_center = 0; current_center < 6; current_center++)
        {
            centers[current_center][0] = max_x-2;
        }
    }
    //update the angles
    for(int i=0;i<4;i++)

       {
           angles[i]=(angles[i]+90)%360;
       }
       //for the last two angles increase by  180 degree
         for(int i=4;i<6;i++)
         {
              angles[i]=(angles[i]+180)%360;
         }

            


}
void updateEvent(int value)
{
    printf("update event is called\n");
      printf("collision time is %lf\n",collisionTime);
    printf("current time is %lf\n",glutGet(GLUT_ELAPSED_TIME) / 1000.0);
       if (isCollisionScheduled && glutGet(GLUT_ELAPSED_TIME) / 1000.0 >= collisionTime) {
        handleCollision();
    }
    update_ball_position();
    
    glutTimerFunc(1,updateEvent,0);
    glutPostRedisplay();  // Trigger a redraw

}
void animate()
{
    //control the time after animate will be called
    //printf("animate is called\n");
    
    //print collision time and current time
  

    
    //   double step_size = 0.4;
    //   //print direction
    // for (int current_center = 0; current_center < 6; current_center++)
    // {
    //      centers[current_center][0] += direction.getX() * step_size;
    //     centers[current_center][1] += direction.getY() * step_size;
        
    //     centers[current_center][2] += direction.getZ() * step_size;
        
    //     // Check for collisions with boundaries and update direction
    //     // Reflect the direction if the ball hits a wall (assuming walls are at z = 0 and z = max_z)
    //     //radius of the sphere is 1
    //     if (centers[current_center][1] < -max_y || centers[current_center][1] > max_y)
    //     {
    //         printf("updating direction of y \n");
    //         Point old_direction = direction;
    //         Point normal = Point(0, 1, 0); // Assuming the wall is in the y-direction
    //         double dotProduct = direction / normal;
    //         direction = direction - (normal * (2 * dotProduct));
    //         //cancel the old direction from all the previous updated centers
    //         for(int i=0;i<=current_center;i++)
    //         {
    //             centers[i][0]-=old_direction.getX()*step_size;
    //             centers[i][1]-=old_direction.getY()*step_size;
    //             centers[i][2]-=old_direction.getZ()*step_size;
    //             //and add the new direction
    //             centers[i][0]+=direction.getX()*step_size;
    //             centers[i][1]+=direction.getY()*step_size;
    //             centers[i][2]+=direction.getZ()*step_size;

    //         }
            
        
           

    //     }

    //     // Reflect the direction if the ball hits a wall (assuming walls are at x = 0 and x = max_x)
    //     if (centers[current_center][0] < -max_x || centers[current_center][0] > max_x)
    //     {
    //         printf("updating direction of x\n");
    //         Point old_direction = direction;
    //          Point normal = Point(1, 0, 0); // Assuming the wall is in the x-direction
    //         double dotProduct = direction / normal;
    //         direction = direction - (normal * (2 * dotProduct));
    //           for(int i=0;i<=current_center;i++)
    //         {
    //             centers[i][0]-=old_direction.getX()*step_size;
    //             centers[i][1]-=old_direction.getY()*step_size;
    //             centers[i][2]-=old_direction.getZ()*step_size;
    //             //and add the new direction
    //             centers[i][0]+=direction.getX()*step_size;
    //             centers[i][1]+=direction.getY()*step_size;
    //             centers[i][2]+=direction.getZ()*step_size;

    //         }



    //     }
       

    // }
    //    for(int i=0;i<4;i++)

    //    {
    //        angles[i]=(angles[i]+90)%360;
    //    }
    //    //for the last two angles increase by  180 degree
    //      for(int i=4;i<6;i++)
    //      {
    //           angles[i]=(angles[i]+180)%360;
    //      }

  

    glutPostRedisplay();  // Trigger a redraw

}
int main(int argc, char **argv)
{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Rolling ball");
    initialization();
    glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(display);
    
    glutIdleFunc(animate);
    glutKeyboardFunc(keyboard1);
    glutSpecialFunc(keyboard2);
    glutTimerFunc(1,updateEvent,0);
    glutMainLoop();
    return 0;
}