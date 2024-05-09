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

    ~Point()
    {
        x = y = z = 0.0;
    }
};
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
Point up;
Point right;
Point look;
GLdouble variable_radius = 0.0;
GLdouble object_angle = 0.0;
GLdouble rotate_factor = 5.0;
void draw_axis(double axis_length)
{
    glBegin(GL_LINES);
    {
        glColor3f(1, 0, 0);
        glVertex3f(axis_length, 0, 0);
        glVertex3f(-axis_length, 0, 0);

        glColor3f(0, 1, 0);
        glVertex3f(0, -axis_length, 0);
        glVertex3f(0, axis_length, 0);

        glColor3f(0, 0, 1);
        glVertex3f(0, 0, -axis_length);
        glVertex3f(0, 0, axis_length);
    }
    glEnd();
}
//method to draw triangle
void draw_triangle(double a, double b, double c)
{
    // this draws a triangle centered at origin
    glBegin(GL_TRIANGLES);
    {
        //the base traingle vertices are given in the spec
        
        glColor3f(a, b, c);
        glVertex3f(1.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 1.0f);
    }
    glEnd();
}
void draw_pyramid()
{
   
    //initially , variable_radius is 0 and trans is 0

    // variable_scale is the 1 initially and as variable_radius increases
    // variable_scale decreases


//r=a/sqrt(2)

    GLfloat variable_scale = 1 - sqrt(2) * variable_radius;
    //this relation is derived from the fact that 
    GLfloat trans = (variable_radius) / sqrt(3);
    int index = 0;
    //we will draw a pyramid using 4 triangles
    //rotate the triangle around y axis to draw 4 triangles and build a pyramid
    //when trans increases , the pyramid becomes smaller and thats why we 
    //also we need to scale it when variable_radius increases
    //translation is done to make the pyramid stand on the ground
    while (index < 4)
    {
        glPushMatrix();
        {
            // rotate around y axis
            glRotatef(90 * index, 0, 1, 0);
            // translate along y axis
            glTranslated(trans, trans, trans);
            // scale
            glScalef(variable_scale, variable_scale, variable_scale);
            // draw triangle
            if (index == 0 || index == 2)
            {
                // draw triangle using cyan
                draw_triangle(0, 1, 1);
            }
            else
            {
                // draw triangle using majenta
                draw_triangle(1, 0, 1);
            }
            index++;
        }
        glPopMatrix();
    }
}
void draw_octahedron()
{
    draw_pyramid();
    glPushMatrix();
    {
        // rotate around x axis
        glRotatef(180, 1, 0, 0);
        draw_pyramid();
    }
    glPopMatrix();
}

void draw_subsphere(double a, double b, double c)
{
    //we are using spherical coordinates to draw the sphere
    // latitudinal plane is rotated from -45 to 45 degree
    //latitudinal plane is horizontal plane 
    //longitudinal plane is vertical  plane from north pole to south pole


    //longitudinal plane is rotated from -45 to 45 degree

    // number of divisions for sphere
    int number_of_divisions = 3;
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

        lat_angle = 45.0 - 90.0 * i / (total_points - 1);
        lat_angle *= degree_to_radian;
        normal_of_lat_plane = Point(-sin(lat_angle),
                                    cos(lat_angle), 0);

        for (int j = 0; j < total_points; ++j)
        {
            long_angle = -45.0 + 90.0 * j / (total_points - 1);
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

    int angles[6] = {0, 180, 270, 90, 90, 270};

void draw_sphere()
{
    //when variable radius is 1/sqrt(2) , the v_radisu of the sphere is 0
    //it means the sphere center is at the center of the cube
    //when variable radius is 0 , the v_radius of the sphere is 1
    //it means the sphere center is at the corner of the cube
    //at the corner of the triangle
    GLfloat v_radius = 1 - sqrt(2) * variable_radius;


    GLfloat axis[6][3] = {{0, 1, 0},
                          {0, 1, 0},
                          {0, 1, 0},
                          {0, 1, 0},
                          {0, 0, 1},
                          {0, 0, 1}};
    GLfloat centers[6][3] =
        {(v_radius, 0, 0),
         (-v_radius, 0, 0),
         (0, 0, v_radius),
         (0, 0, -v_radius),
         (0, v_radius, 0),
         (0, -v_radius, 0)};
    for (int i = 0; i < 6; i++)
    {
        glPushMatrix();
        {
            glTranslatef(centers[i][0],
                         centers[i][1], centers[i][2]);
            glScalef(variable_radius,
             variable_radius, variable_radius);
            glRotatef(angles[i], axis[i][0], axis[i][1], axis[i][2]);
            if(i==0)
            {
                //green
                draw_subsphere(0,1,0);
            }
            else if (i==1)
            {
                //green
                draw_subsphere(0,1,0);
                
            }
            else if(i==2 || i ==3)
            {
                //red
                draw_subsphere(1,0,0);
            }
            else if(i==4 || i==5)
            {
                //blue
                draw_subsphere(0,0,1);
            }
        }
        glPopMatrix();
    }
}

void draw_cylinder_segment(double height, double radius, int segments)
{
    double cylinder_angle = 70.528779365509;
    double cylinder_angle_radian = (cylinder_angle / 2) * M_PI / 180.00;
    double cylinder_height = height;
    double cylinder_radius = radius;
    double cylinder_segments = 100;
    double cylinder_segment_angle = (cylinder_angle / cylinder_segments)
     * M_PI / 180.00;
           glColor3f(0.5, 0.5, 0.5);

    glBegin(GL_TRIANGLE_STRIP);
    {
                glColor3f(1, 1, 0);

        for (int i = 0; i <= cylinder_segments; i++)
        {
            double angle = -cylinder_angle_radian + 
            i * cylinder_segment_angle;

            glVertex3f(cylinder_radius * cos(angle),
             cylinder_radius * sin(angle), cylinder_height/2);

            glVertex3f(cylinder_radius * cos(angle), cylinder_radius * 
            sin(angle), -cylinder_height/2);
        }
    }
    glEnd();
}
void draw_cylinder()
{
    //there are 13 edges and they have to be drawn as cylinder
    GLfloat r = 1 - sqrt(2) * variable_radius;
    GLfloat h = sqrt(2) -2* variable_radius;
    // draw 4 cylinders in xz plane
    glColor3f(1.0, 1.0, 1.0);
    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();
        {
            glRotated(90 * i, 0, 1, 0);
            glTranslated(r/2, 0, r/2);
            glRotated(-45, 0, 1, 0);
            draw_cylinder_segment(h, variable_radius, 100);
        }
        glPopMatrix();
    }
    // draw 2 cylinders in yz plane
    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();
        {
            glRotated(90 * i, 0, 1, 0);
            glTranslated(0, r/2, r/2);
            glRotated(45, 1, 0, 0);
            glRotated(90, 0, 0, 1);
            draw_cylinder_segment(h, variable_radius, 100);
        }
        glPopMatrix();
    }
    // draw 2 cylinders in xy plane
    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();
        {
            glRotated(90 * i, 0, 1, 0);
            glTranslated(0, -r/2, r/2);
            glRotated(-45, 1, 0, 0);
            glRotated(-90, 0, 0, 1);
            draw_cylinder_segment(h, variable_radius, 100);
        }
        glPopMatrix();
    }
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
    else if(key=='.')
    {
        variable_radius=variable_radius-0.045;
        if(variable_radius<0)
        {
            variable_radius=0;
        }
    }
    else if(key==',')
    {
        variable_radius=variable_radius+0.045;
        if(variable_radius>1/sqrt(2))
        {
            variable_radius=1/sqrt(2);
        }
    }
    else if(key=='a')
    {
        object_angle=object_angle+rotate_factor;
    }
    else if(key=='d')
    {
        object_angle=object_angle-rotate_factor;
    }
    else if(key==27)
    {
        exit(0);
    
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
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(position_camera.getX(), position_camera.getY(), position_camera.getZ(), position_camera.getX() + look.getX(), look.getY() + position_camera.getY(), look.getZ() + position_camera.getZ(), up.getX(), up.getY(), up.getZ());

    glPushMatrix();
    {
        draw_axis(3);
        glRotatef(object_angle, 0, 0, 1);

        draw_cylinder();
        draw_octahedron();
        draw_sphere();
    }
    
    glPopMatrix();
    glutSwapBuffers();
}
void initialization()
{
    /* codes for initialization */

    position_camera = Point(2, 2, 2);
    up = Point(0.0, 0.0, 1.0);
    right = Point(-1.0 / sqrt(2.0), 1.0 / sqrt(2.0), 0.0);
    look = Point(-2.0 / sqrt(3.0), -2.0 / sqrt(3.0), -2.0 / sqrt(3.0));

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
void animate()
{
    // codes for any changes in Models, Camera
    glutPostRedisplay();
}
int main(int argc, char **argv)
{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Magic Cube");
    initialization();
    glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(display);
    glutIdleFunc(animate);
    glutKeyboardFunc(keyboard1);
    glutSpecialUpFunc(keyboard2);
    glutMainLoop();
    return 0;
}