#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include<GL/glut.h>

#include<vector>



#define PI 2*acos(0.0)
class Point {
    double x;
    double y;
    double z;

public:
    Point() {
        x=0;
        y=0;
        z=0;
    }

    Point(double x, double y, double z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    double getX() const {
        return x;
    }

    double getY() const {
        return y;
    }

    double getZ() const {
        return z;
    }

    Point operator+(const Point _vector);
    Point operator-(const Point _vector);
    Point operator*(const double scalar);
    Point operator^(const Point _vector);

    ~Point() {
        x = y = z = 0.0;
    }


};
    Point Point::operator+(const Point vector)
{
    return Point(x+vector.x,y+vector.y,vector.z+z);

}
Point Point::operator-(const Point vector)
{
    return Point(x-vector.x,y-vector.y,z-vector.z);

}
Point Point::operator*(const double vector)
{
    return Point(x*vector,y*vector,z*vector);

}
Point Point :: operator^(const Point vector)
{
    //this is vector cross product
    return Point(y*vector.z-z*vector.y,z*vector.x-x*vector.z,x*vector.y-y*vector.x);

}
Point cross_multiplication(const Point vector1,const Point vector2)
{
    //this is vector cross product
    return Point(vector1.getY()*vector2.getZ()-vector1.getZ()*vector2.getY(),vector1.getZ()*vector2.getX()-vector1.getX()*vector2.getZ(),vector1.getX()*vector2.getY()-vector1.getY()*vector2.getX());


}


Point position_camera;// camera position
Point up; //a vector perpendicular to look vector
Point right;//a vector perpendicular to look vector and up vector
Point look;//a vector pointing towards the direction where the camera is looking at
double angle_factor=30;
double movement_factor=1;
void draw_axis(double axis_length)
{
    glColor3f(1.0,1.0,1.0);
    glBegin(GL_LINES);
    {
        glVertex3f(axis_length,0,0);
        glVertex3f(-axis_length,0,0);

        glVertex3f(0,-axis_length,0);
        glVertex3f(0,axis_length,0);

        glVertex3f(0,0,-axis_length);
        glVertex3f(0,0,axis_length);

    }
    glEnd();

}
void drawCube() {
    // Draw the front face (green)
    glBegin(GL_QUADS);
    glColor3f(0.0, 1.0, 0.0); // Green
    glVertex3f(-0.5, -0.5, 0.5);
    glVertex3f(0.5, -0.5, 0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(-0.5, 0.5, 0.5);
    glEnd();

    // Draw the back face (blue)
    glBegin(GL_QUADS);
    glColor3f(0.0, 0.0, 1.0); // Blue
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(-0.5, 0.5, -0.5);
    glEnd();

    // Draw the right face (red)
    glBegin(GL_QUADS);
    glColor3f(1.0, 0.0, 0.0); // Red
    glVertex3f(0.5, -0.5, 0.5);
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glEnd();

    // Draw the left face (yellow)
    glBegin(GL_QUADS);
    glColor3f(1.0, 1.0, 0.0); // Yellow
    glVertex3f(-0.5, -0.5, 0.5);
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(-0.5, 0.5, -0.5);
    glVertex3f(-0.5, 0.5, 0.5);
    glEnd();

    // Draw the top face (cyan)
    glBegin(GL_QUADS);
    glColor3f(0.0, 1.0, 1.0); // Cyan
    glVertex3f(-0.5, 0.5, 0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(-0.5, 0.5, -0.5);
    glEnd();

    // Draw the bottom face (magenta)
    glBegin(GL_QUADS);
    glColor3f(1.0, 0.0, 1.0); // Magenta
    glVertex3f(-0.5, -0.5, 0.5);
    glVertex3f(0.5, -0.5, 0.5);
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(-0.5, -0.5, -0.5);
    glEnd();
}
void keyboard_input_2(unsigned char key,int x,int y)
{
    //the assumption was the vector we rotating is perpendicular to the reference vector
    //then the formula is v=lcosA+(r*l)sinA
    if(key=='1')
    {
        //rotate left means rotate around up vector in clockwise direction
        //so we need to rotate look and right vector
        right=right*cos(PI/angle_factor)+(cross_multiplication(up,right))*sin(PI/angle_factor);

        look=look*cos(PI/angle_factor)-(cross_multiplication(up,look))*sin(PI/angle_factor);

    }
    else if(key=='2')
    {
        right= right*cos(-PI/angle_factor)+(cross_multiplication(up,right))*sin(-PI/angle_factor);
        look=look*cos(-PI/angle_factor)-(cross_multiplication(up,look))*sin(-PI/angle_factor);


    }
    else if(key=='3')
    {
        //look up means rotate 
        //around right vector in clockwise direction
        look=look*cos(PI/angle_factor)+(cross_multiplication(right,look))*sin(PI/angle_factor);
        up=up*cos(PI/angle_factor)+(cross_multiplication(right,up))*sin(PI/angle_factor);


    }
    else if(key=='4')
    {
        look=look*cos(-PI/angle_factor)+(right^look)*sin(-PI/angle_factor);
        up=up*cos(-PI/angle_factor)+(right^up)*sin(-PI/angle_factor);
    }
    else if (key=='5')
    {
        //tilt clockwise means rotate around look vector in clockwise direction
        right=right*cos(PI/angle_factor)+(look^right)*sin(PI/angle_factor);
        up=up*cos(PI/angle_factor)+(look^up)*sin(PI/angle_factor);
        
    }
    else if(key=='6')
    {
        right=right*cos(-PI/angle_factor)+(look^right)*sin(-PI/angle_factor);
        up=up*cos(-PI/angle_factor)+(look^up)*sin(-PI/angle_factor);
    }
    else if(key==27)
    {
        exit(0);
    }

}
void keyboard_input_1(int key,int x,int y)
{
    if(key==GLUT_KEY_UP)
    {
        position_camera=position_camera+(look*movement_factor);


    }
    else if(key==GLUT_KEY_DOWN)
    {
        position_camera=position_camera-(look*movement_factor);
        
    }
    else if (key==GLUT_KEY_LEFT)
    {
        position_camera=position_camera-(right*movement_factor);
        
    }
    else if(key==GLUT_KEY_RIGHT)
    {
        position_camera=position_camera+(right*movement_factor);
    }
    else if (key==GLUT_KEY_PAGE_UP)
    {
        position_camera=position_camera+(up*movement_factor);
    }
    else if(key==GLUT_KEY_PAGE_DOWN)
    {
        position_camera=position_camera-(up*movement_factor);
    }
    else if(key==27)
    {
        exit(0);
    }

}
//display function is called whenever the display needs to be updated   
void display()
{
    //clear the display
    //GL_COLOR_BUFFER_BIT: Bit mask to clear the color buffer
    //GL_DEPTH_BUFFER_BIT: Bit mask to clear the depth buffer
    //COlor buffer is responsible for the color of your object
    //Depth buffer is responsible for the depth
    // i.e how far your object is from you
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 0);  //color black
    //clearing again for safety
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //GL_MODELVIEW: Applies subsequent matrix
    // operations to the modelview matrix stack.
    //Basically what you are doing in the display function
    //is transforming the modelview matrix
    //so that the object can be displayed
    //according to the transformation
    //you have applied to the modelview matrix
    //and the projection matrix
    
    glMatrixMode(GL_MODELVIEW);

	//load the identity matrix FOR  MODELVIEW MATRIX
    // FOR YOUR REFERENCE
	glLoadIdentity();
    //gluLookAt() is used to specify the 
    //position of the camera in the world coordinate
    //where the camera is looking at
	gluLookAt(position_camera.getX(), position_camera.getY(), position_camera.getZ(), position_camera.getX()+look.getX(), position_camera.getY()+look.getY(), position_camera.getZ()+look.getZ(), up.getX(), up.getY(), up.getZ());

	/* again select MODEL-VIEW */
	glMatrixMode(GL_MODELVIEW);
    draw_axis(5);
    drawCube();
    //
    //without this doesn't work because of the previous matrix mode is projection
    glutSwapBuffers();

}
void animate() {
    //codes for any changes in Models, Camera
	glutPostRedisplay();
}
void initialization() {
	/* codes for initialization */

	position_camera = Point(10, 10, 10);
	up = Point(0.0, 0.0, 1.0);
	right = Point(-1.0/sqrt(2.0), 1.0/sqrt(2.0), 0.0);
	look = Point(-1.0/sqrt(2.0), -1.0/sqrt(2.0), 0.0);


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

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);  //Depth, Double buffer, RGB color

	glutCreateWindow("Fully Controllable Camera");

	initialization();  // initialization

	glEnable(GL_DEPTH_TEST);  //enable Depth Testing

	glutDisplayFunc(display);  //display callback function
	glutIdleFunc(animate);  //what you want to do in the idle time (when no drawing is occuring)
    glutKeyboardFunc(keyboard_input_2);
	glutSpecialFunc(keyboard_input_1);
	

	glutMainLoop();  //The main loop of OpenGL

	return 0;
}