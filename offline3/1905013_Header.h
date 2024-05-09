#include <bits/stdc++.h>
#include <GL/glut.h>
#include <vector>
#include <math.h>
#include <iostream>
using namespace std;
#define EPSILON 0.000001
double max(double a, double b)
{
    // implement a max for double
    if (a - b > EPSILON)
        return a;
    else
        return b;
}
int recursion_level;

class Vector3d
{
    //all ok
public:
    double x;
    double y;
    double z;
    Vector3d()
    {
        x = 0;
        y = 0;
        z = 0;
    }
    Vector3d(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    //copy constructor for vector
    Vector3d(const Vector3d &v)
    {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
    }
    Vector3d operator+(Vector3d v)
    {
        return Vector3d(x + v.x, y + v.y, z + v.z);
    }
    Vector3d operator-(Vector3d v)
    {
        return Vector3d(x - v.x, y - v.y, z - v.z);
    }
    Vector3d operator*(double d)
    {
        return Vector3d(x * d, y * d, z * d);
    }
    Vector3d operator/(double d)
    {
        return Vector3d(x / d, y / d, z / d);
    }
    double dot(Vector3d v)
    {
        return x * v.x + y * v.y + z * v.z;
    }
    Vector3d cross(Vector3d v)
    {
        return Vector3d(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }
    double length()
    {
        return sqrt(x * x + y * y + z * z);
    }
    Vector3d normalize()
    {
        return *this / length();
    }
    void print()
    {
        cout << x << " " << y << " " << z << endl;
    }
    // find angle between two vectors
    double find_angle(Vector3d v2)
    {
        double dot = this->dot(v2);
        double co_eff = (this->length() * v2.length());
        return dot / (co_eff);
    }
};
class Color
{
public:
    double r;
    double g;
    double b;
    Color()
    {
        r = 0;
        g = 0;
        b = 0;
    }
    Color(double r, double g, double b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
    }
    //copy constructor
    Color(const Color &c)
    {
        this->r = c.r;
        this->g = c.g;
        this->b = c.b;
    }
    //set color
    void set_color(double r, double g, double b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
    }
    Color operator+(Color c)
    {
        return Color(r + c.r, g + c.g, b + c.b);
    }
    Color operator*(Color c)
    {
        return Color(r * c.r, g * c.g, b * c.b);
    }
    Color operator*(double d)
    {
        return Color(r * d, g * d, b * d);
    }
    Color operator/(double d)
    {
        return Color(r / d, g / d, b / d);
    }
    void print()
    {
        cout << r << " " << g << " " << b << endl;
    }
    void fix_precision()
    {
        // set r,g,b between 0 to 1
        if (r > 1)
            r = 1;
        if (g > 1)
            g = 1;
        if (b > 1)
            b = 1;
        if (r < 0)
            r = 0;
        if (g < 0)
            g = 0;
        if (b < 0)
            b = 0;
    }
};

class Ray
{
   //this is the class to represent the ray
   //A ray is R(t)=R_0+t*R_d

public:
    Vector3d start;
    // start point of the ray R0
    Vector3d direction;
    // direction of the ray

    Ray() {
        start = Vector3d(0, 0, 0);
        direction = Vector3d(0, 0, 0);
    }
    //write a copy constructor
    Ray(const Ray &r)
    {
        this->start = r.start;
        this->direction = r.direction;
    }

    Ray(Vector3d start, Vector3d direction)
    {
        this->start = start;
        this->direction = direction;
        this->direction=direction.normalize();
        
    }
    void normalize_ray() { this->direction = this->direction.normalize(); }
    void print()
    {
        cout << "Start: ";
        start.print();
        cout << "Direction: ";
        direction.print();
        cout << endl;
    }
};

class Pointlight
{
    // ambient light diffuse light spot light
    // diffuse light is the light that comes from a source and falls on a surface
    // pontlight comes under diffuse light
    // it has a position and a color
    // light emitted from a point in all directions
    //[x y z 1]^T
public:
    Vector3d light_position;
    Color color;
    Pointlight()
    {
        light_position = Vector3d(0, 0, 0);
        color = Color(0, 0, 0);
    }
    Pointlight(Vector3d lp, Color c)
    {
        light_position = lp;
        color = c;
    }
    Pointlight(Vector3d lp)
    {
        light_position = lp;
    }
    void setcolor(int r, int g, int b)
    {
        this->color = Color(r, g, b);
    }
    void setposition(double x, double y, double z)
    {
        this->light_position = Vector3d(x, y, z);
    }
    void draw_quads(int stacks, int slices, Vector3d points[100][100])
    {
        // set the color of the light
        glColor3f(color.r, color.g, color.b);
        for (int i = 0; i < stacks; i++)
        {
            for (int j = 0; j < slices; j++)
            {
                glBegin(GL_QUADS);
                {

                     // lower hemisphere
                    // to draw lower hemisphere from the upper hemisphere just change the sign of the z coordinate
                    glVertex3f(points[i][j].x, points[i][j].y, -points[i][j].z);
                    glVertex3f(points[i + 1][j].x, points[i + 1][j].y, -points[i + 1][j].z);
                    glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, -points[i + 1][j + 1].z);
                    glVertex3f(points[i][j + 1].x, points[i][j + 1].y, -points[i][j + 1].z);
                    // upper hemisphere
                    glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
                    glVertex3f(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);
                    glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
                    glVertex3f(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
                   
                }
                glEnd();
            }
        }
    }

    void draw()
    {
        // when we are drawing point light we are drawing a sphere that represents it
        glPushMatrix();
        {
            // translate to the light position
            glTranslatef(this->light_position.x, this->light_position.y, this->light_position.z);

            Vector3d points[100][100];
            int stacks = 20;
            int slices = 24;
            double radius = 1.7;
            // now generate points
            for (int i = 0; i <= stacks; i++)
            {
                double h = radius * sin(((double)i / (double)stacks) * (M_PI / 2));
                double r = radius * cos(((double)i / (double)stacks) * (M_PI / 2));
                for (int j = 0; j <= slices; j++)
                {
                    points[i][j].x = r * cos(((double)j / (double)slices) * 2 * M_PI);
                    points[i][j].y = r * sin(((double)j / (double)slices) * 2 * M_PI);
                    points[i][j].z = h;
                }
            }
            // draw the quads
            draw_quads(stacks, slices, points);
        }
        glPopMatrix();
    }
    void print()
    {
        cout << "Light Position: ";
        light_position.print();
        cout << "Color: ";
        color.print();
    
    }
};
class Spotlight
{
    // spotlight is a point source that emits lughts in a restricted set of directions
    // it has a position and a direction and a cutoff angle (the angle at which the spotlight becomes fully dark)
    // Intensity depends on angle
public:
    Pointlight point_light;
    Vector3d light_direction;
    double cutoff_angle;
    Spotlight()
    {
        point_light = Pointlight();
        light_direction = Vector3d(0, 0, 0);
        cutoff_angle = 0;
    }
    Spotlight(Pointlight pl, Vector3d ld, double ca)
    {
        point_light = pl;
        light_direction = ld;
        cutoff_angle = ca;
    }
    Spotlight(Vector3d position,Vector3d direction,double angle,Color c)
    {
        this->point_light = Pointlight(position,c);
        this->light_direction = direction;
        this->cutoff_angle = angle;

    }
    void draw_quads(int stacks, int slices, Vector3d points[200][200])
    {
        // set the color of the light
        glColor3f(point_light.color.r, point_light.color.g, point_light.color.b);
        for (int i = 0; i < stacks; i++)
        {
            for (int j = 0; j < slices; j++)
            {
                glBegin(GL_QUADS);
                {
                    // upper hemisphere
                    glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
                    glVertex3f(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);
                    glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
                    glVertex3f(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
                    // lower hemisphere
                    glVertex3f(points[i][j].x, points[i][j].y, -points[i][j].z);
                    glVertex3f(points[i + 1][j].x, points[i + 1][j].y, -points[i + 1][j].z);
                    glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, -points[i + 1][j + 1].z);
                    glVertex3f(points[i][j + 1].x, points[i][j + 1].y, -points[i][j + 1].z);
                }
                glEnd();
            }
        }
    }
    void draw()
    {
        glPushMatrix();
        {
            // translate to the light position
            glTranslatef(this->point_light.light_position.x, this->point_light.light_position.y, this->point_light.light_position.z);
            // draw the light
            double radius = 0.2;
            int stacks = 20;
            int slices = 24;
            Vector3d points[200][200];
            for (int i = 0; i <= stacks; i++)
            {
                double h = radius * sin(((double)i / (double)stacks) * (M_PI / 2));
                double r = radius * cos(((double)i / (double)stacks) * (M_PI / 2));
                for (int j = 0; j <= slices; j++)
                {
                    points[i][j].x = r * cos(((double)j / (double)slices) * 2 * M_PI);
                    points[i][j].y = r * sin(((double)j / (double)slices) * 2 * M_PI);
                    points[i][j].z = h;
                }
            }
            // draw the quads
            draw_quads(stacks, slices, points);
        }
        glPopMatrix();
    }
    bool crossed_cuttoff_or_not(Ray &ray)
    {
        double angle = light_direction.find_angle(ray.direction);
        if (angle > cutoff_angle)
            return false;
        else
            return true;
    }
    void print()
    {
        cout << "Light Position: ";
        point_light.light_position.print();
        cout << "Light Direction: ";
        light_direction.print();
        cout << "Cutoff Angle: " << cutoff_angle << endl;
    
    }
};

class Object
{
    //this is the base class for all the objects
public:
    Vector3d reference_point;
    double height;
    double width;
    double length;
    Color color;
    // this are for illumination
    double coEfficients[4]; // ambient, diffuse, specular, reflection coefficients

    int shine; // exponent term of specular component
    Object()
    {
        cout << "base constructor for class object is called" << endl;
    }
    Object(Vector3d rp, double h, double w, double l)
    {
        reference_point = rp;
        height = h;
        width = w;
        length = l;
    }
    void setcolor(double r, double g, double b)
    {
        this->color = Color(r, g, b);
    }
    void setcoEfficients(double a, double d, double s, double r)
    {
        coEfficients[0] = a;
        coEfficients[1] = d;
        coEfficients[2] = s;
        coEfficients[3] = r;
    }
    void setShine(int shine)
    {
        this->shine = shine;
    }
    virtual void draw()
    {
    }
    virtual double get_parameter_t(Ray &ray)
    {
        // this function will be used to calculate the parameter t
        return -1;
    }

    virtual Vector3d get_normal(Ray &ray, Vector3d &point)
    {
        // this function will be used to calculate normal on the intersection point
        return Vector3d(0, 0, 0);
    }

    virtual double intersect(Ray &ray, Color &color, int level=0)
    {
        // this will be used to handle intersection
      //  cout<<"base"<<endl;

        return -1;
    }
    virtual void print()
    {

    }
};


std::vector<Object *> object_list;// Defined objects in the scene
std::vector<Pointlight *> pointLights; // Defined point lights in the scene
std::vector<Spotlight *> spotLights;   // Defined spot lights in the scene
class Triangle : public Object
{

public:
    // 3 points for triangle
    Vector3d a;
    Vector3d b;
    Vector3d c;
    Triangle()
    {
        a = Vector3d(0, 0, 0);
        b = Vector3d(0, 0, 0);
        c = Vector3d(0, 0, 0);
    }
    Triangle(Vector3d a, Vector3d b, Vector3d c)
    {
        this->a = a;
        this->b = b;
        this->c = c;
    }
    void draw()
    {
        //cout<<"Triangle draw called"<<endl;
        //we can draw the triangle easily by using the opengl function which uses just 3 points to draw the triangle
        glPushMatrix();
        {
            glColor3f(this->color.r, this->color.g, this->color.b);
            glBegin(GL_TRIANGLES);
            {
                glVertex3f(a.x, a.y, a.z);
                glVertex3f(b.x, b.y, b.z);
                glVertex3f(c.x, c.y, c.z);
            }
            glEnd();
        }
        glPopMatrix();
    }


    double get_parameter_t(Ray &ray)
    {
        double det_a = ((a.x - b.x) * ((a.y - c.y) * ray.direction.z - (a.z - c.z) * ray.direction.y))
   - ((a.y - b.y) * ((a.x - c.x) * ray.direction.z - (a.z - c.z) * ray.direction.x))
   + ((a.z - b.z) * ((a.x - c.x) * ray.direction.y - (a.y - c.y) * ray.direction.x));

  if (std::abs(det_a) < EPSILON) {
    return -1.0;
  }
  double det_t = ((a.x - b.x) * ((a.y - c.y) * (a.z - ray.start.z) - (a.z - c.z) * (a.y - ray.start.y)))
  - ((a.y - b.y) * ((a.x - c.x) * (a.z - ray.start.z) - (a.z - c.z) * (a.x - ray.start.x)))
   + ((a.z - b.z) * ((a.x - c.x) * (a.y - ray.start.y) - (a.y - c.y) * (a.x - ray.start.x)));
  double det_beta = ((a.x - ray.start.x) * ((a.y - c.y) * ray.direction.z - (a.z - c.z) * ray.direction.y))
   - ((a.y - ray.start.y) * ((a.x - c.x) * ray.direction.z - (a.z - c.z) * ray.direction.x))
   + ((a.z - ray.start.z) * ((a.x - c.x) * ray.direction.y - (a.y - c.y) * ray.direction.x));

  double det_gamma = ((a.x - b.x) * ((a.y - ray.start.y) * ray.direction.z - (a.z - ray.start.z) * ray.direction.y))
   - ((a.y - b.y) * ((a.x - ray.start.x) * ray.direction.z - (a.z -ray.start.z) * ray.direction.x))
   + ((a.z - b.z) * ((a.x - ray.start.x) * ray.direction.y - (a.y - ray.start.y) * ray.direction.x));



double beta=det_beta/det_a;
double gamma=det_gamma/det_a;
double t=det_t/det_a;

  if (beta < 0.0 || (beta - 1.0) > EPSILON)
    {
        return -1.0;
    }
  if(  gamma < 0.0 || (gamma - 1.0) > EPSILON)
    {
        return -1.0;
    }
  if( t < 0.0 || (beta + gamma - 1.0) > EPSILON) {
    return -1.0;
  }

  return t;
    }

    Vector3d get_normal(Ray &ray,Vector3d &point)
    {
        //cout<<"Normal called"<<endl;
        // Calculate the two vectors from the three points of the triangle
        Vector3d edge1 = b - a;
        Vector3d edge2 = c - a;

        // Use the cross product to find the normal vector of the triangle plane
        Vector3d normal = edge1.cross(edge2);

        // Normalize the normal vector
        normal=normal.normalize();
        // check the direction with the ray
        if (normal.dot(ray.direction) > 0.0)
            normal = normal * -1; // reverse the normal vector, so it's pointing towards the ray

        return normal;
    }

  
// void do_some_phong_calculations(double current_t,Ray &ray,Color &ans_color,Vector3d &intersection_point,Color &intersection_color,Vector3d& normal)
// {
     
//         ans_color = intersection_color * coEfficients[0]; // ambient color
//         ans_color.fix_precision();

//         for (auto *light : pointLights)
//         {

//             Ray toLight = Ray(light->light_position, intersection_point - light->light_position);
//             toLight.normalize_ray();
//             double t_minimum = INFINITY;
//             double t_current = 0;
//             // check for intersection with other objects
//             for (Object *obj : object_list)
//             {
//                 t_current = obj->intersect(toLight, this->color, 0);
//                 if (t_current > 0.0 && (t_minimum - t_current) > EPSILON)
//                 {
//                     t_minimum = t_current;
//                 }
//             }
//             if ((current_t - t_minimum) > EPSILON)
//             {
//                 continue;
//                 // it means the light is shadowed by another object
//             }
//             double lambert_value = normal.dot(toLight.direction * -1);
//             Ray ray_R = Ray(intersection_point, normal * 2 * lambert_value + toLight.direction);
//             ray_R.normalize_ray();
//             double phong_value = pow(ray_R.direction.dot(ray.direction * -1), shine);
//             ans_color = ans_color + (light->color * coEfficients[1] * max(lambert_value, 0) * intersection_color);
//             ans_color.fix_precision();
//             ans_color = ans_color + (light->color * coEfficients[2] * max(phong_value, 0) * intersection_color);
//             ans_color.fix_precision();
//         }
//         // now for spotlights the calculation will be same
//         for (Spotlight *spot_light : spotLights)
//         {
//             Ray ray_L = Ray(spot_light->point_light.light_position, intersection_point - spot_light->point_light.light_position);
//             ray_L.normalize_ray();
//             if (spot_light->crossed_cuttoff_or_not(ray_L))
//             {
//                 continue;
//             }
//             double t_minimum = INFINITY;
//             double t_current = 0;
//             for (Object *obj : object_list)
//             {
//                 t_current = obj->intersect(ray_L, color, 0);
//                 if (t_current > 0.0 && (t_minimum - t_current) > EPSILON)
//                 {
//                     t_minimum = t_current;
//                 }
//             }
//             if ((current_t - t_minimum) > EPSILON)
//             {
//                 continue;
//             }
//             // now calculate the lambert and phong value
//             double lambert_value = normal.dot(ray_L.direction * -1);
//             Ray ray_R = Ray(intersection_point, normal * (2 * lambert_value) + ray_L.direction);
//             ray_R.normalize_ray();
//             double phong_value = pow(ray_R.direction.dot(ray.direction * -1), shine);
//             ans_color = ans_color + (spot_light->point_light.color * coEfficients[1] * max(lambert_value, 0) * intersection_color);
//             ans_color.fix_precision();
//             ans_color = ans_color + (spot_light->point_light.color * coEfficients[2] * max(phong_value, 0) * intersection_color);
//             ans_color.fix_precision();
//         }
//         return;
        
//         // color.print();
//         // final_color.print();


// }


void pointlight_calculation(Vector3d &intersectionPoint,double &parameter_t,Color &final_color,Vector3d &normal,Ray &ray,Color &intersect_color)
{
     for (auto *light : pointLights)
        {
            double t_minimum = INFINITY;
            double t_current = 0;
            Vector3d dir=intersectionPoint-light->light_position;

            Ray toLight = Ray(light->light_position,dir);
            toLight.normalize_ray();
            
            // check for intersection with other objects
            for (Object *obj : object_list)
            {
                t_current = obj->intersect(toLight, this->color, 0);
                if (t_current > 0.0 && (t_minimum - t_current) > EPSILON)
                {
                    t_minimum = t_current;
                }
            }
            if ((parameter_t - t_minimum) > EPSILON)
            {
                continue;
                // it means the light is shadowed by another object
            }
            Vector3d temp=toLight.direction*-1;
            double lambert_value = normal.dot(temp);

            Vector3d ray_r_dir=normal*2*lambert_value+toLight.direction;

            Ray ray_R = Ray(intersectionPoint,ray_r_dir);

            ray_R.normalize_ray();
            Vector3d temp2=ray.direction*-1;
            double coeff_temp=ray_R.direction.dot(temp2);

            double phong_value = pow(coeff_temp, shine);
            final_color = final_color + (light->color * coEfficients[1] * max(lambert_value, 0) * intersect_color);
            final_color.fix_precision();
            final_color = final_color + (light->color * coEfficients[2] * max(phong_value, 0) * intersect_color);
            final_color.fix_precision();
        }
    
}

void spotlight_calculation(Vector3d &intersectionPoint,double &parameter_t,Color &final_color,Vector3d &normal,Ray &ray,Color &intersect_color)
{
     for (Spotlight *spot_light : spotLights)
        {
              double t_minimum = INFINITY;
            double t_current = 0;
            Vector3d dir=intersectionPoint-spot_light->point_light.light_position;
            Ray ray_L = Ray(spot_light->point_light.light_position, dir);
            ray_L.normalize_ray();
            if (spot_light->crossed_cuttoff_or_not(ray_L))
            {
                continue;
            }
          
            for (Object *obj : object_list)
            {
                t_current = obj->intersect(ray_L, color, 0);
                if (t_current > 0.0 && (t_minimum - t_current) > EPSILON)
                {
                    t_minimum = t_current;
                }
            }
            if ((parameter_t - t_minimum) > EPSILON)
            {
                continue;
            }
            // now calculate the lambert and phong value
            Vector3d temp=ray_L.direction*-1;
            double lambert_value = normal.dot(temp);
            Vector3d ray_r_dir=normal*2*lambert_value+ray_L.direction;
            Ray ray_R = Ray(intersectionPoint, ray_r_dir);
            ray_R.normalize_ray();
            Vector3d temp2=ray.direction*-1;
            double coeff=ray_R.direction.dot(temp2);

            double phong_value = pow(coeff, shine);
            final_color = final_color + (spot_light->point_light.color * coEfficients[1] * max(lambert_value, 0) * intersect_color);
            final_color.fix_precision();
            final_color = final_color + (spot_light->point_light.color * coEfficients[2] * max(phong_value, 0) * intersect_color);
            final_color.fix_precision();
        }
}



 
double intersect(Ray &ray, Color &color, int level = 0)
    {
        //cout<<"Triangle intersect called"<<endl;

        // normalize the ray
        ray.normalize_ray();

        // now get the parameter needed for futher calculation

        double parameter_t = get_parameter_t(ray); // Find intersection
        if (parameter_t < 0)
            return parameter_t; // No intersection
        if (level == 0)
            return parameter_t;

        Vector3d intersectionPoint = ray.start + ray.direction * parameter_t;
        Vector3d normal = this->get_normal(ray,intersectionPoint);
        Color intersect_color = this->color;
        Color final_color=color;
        final_color = intersect_color * coEfficients[0]; // ambient color
        final_color.fix_precision();


        // for (auto *light : pointLights)
        // {

        //     Ray toLight = Ray(light->light_position, intersectionPoint - light->light_position);
        //     toLight.normalize_ray();
        //     double t_minimum = INFINITY;
        //     double t_current = 0;
        //     // check for intersection with other objects
        //     for (Object *obj : object_list)
        //     {
        //         t_current = obj->intersect(toLight, this->color, 0);
        //         if (t_current > 0.0 && (t_minimum - t_current) > EPSILON)
        //         {
        //             t_minimum = t_current;
        //         }
        //     }
        //     if ((parameter_t - t_minimum) > EPSILON)
        //     {
        //         continue;
        //         // it means the light is shadowed by another object
        //     }
        //     double lambert_value = normal.dot(toLight.direction * -1);
        //     Ray ray_R = Ray(intersectionPoint, normal * 2 * lambert_value + toLight.direction);
        //     ray_R.normalize_ray();
        //     double phong_value = pow(ray_R.direction.dot(ray.direction * -1), shine);
        //     final_color = final_color + (light->color * coEfficients[1] * max(lambert_value, 0) * intersect_color);
        //     final_color.fix_precision();
        //     final_color = final_color + (light->color * coEfficients[2] * max(phong_value, 0) * intersect_color);
        //     final_color.fix_precision();
        // }
        pointlight_calculation(intersectionPoint,parameter_t,final_color,normal,ray,intersect_color);



        // now for spotlights the calculation will be same
        // for (Spotlight *spot_light : spotLights)
        // {
        //     Ray ray_L = Ray(spot_light->point_light.light_position, intersectionPoint - spot_light->point_light.light_position);
        //     ray_L.normalize_ray();
        //     if (spot_light->crossed_cuttoff_or_not(ray_L))
        //     {
        //         continue;
        //     }
        //     double t_minimum = INFINITY;
        //     double t_current = 0;
        //     for (Object *obj : object_list)
        //     {
        //         t_current = obj->intersect(ray_L, color, 0);
        //         if (t_current > 0.0 && (t_minimum - t_current) > EPSILON)
        //         {
        //             t_minimum = t_current;
        //         }
        //     }
        //     if ((parameter_t - t_minimum) > EPSILON)
        //     {
        //         continue;
        //     }
        //     // now calculate the lambert and phong value
        //     double lambert_value = normal.dot(ray_L.direction * -1);
        //     Ray ray_R = Ray(intersectionPoint, normal * (2 * lambert_value) + ray_L.direction);
        //     ray_R.normalize_ray();
        //     double phong_value = pow(ray_R.direction.dot(ray.direction * -1), shine);
        //     final_color = final_color + (spot_light->point_light.color * coEfficients[1] * max(lambert_value, 0) * intersect_color);
        //     final_color.fix_precision();
        //     final_color = final_color + (spot_light->point_light.color * coEfficients[2] * max(phong_value, 0) * intersect_color);
        //     final_color.fix_precision();
        // }
        spotlight_calculation(intersectionPoint,parameter_t,final_color,normal,ray,intersect_color);


        color=final_color;
        // color.print();
        // final_color.print();

        if(level>=recursion_level)
        return parameter_t;
        // now for reflection
          Vector3d reflected_ray_dir=ray.direction+(normal*2*(normal.dot(ray.direction*-1) ));
        
        Ray reflected_ray=Ray(intersectionPoint, reflected_ray_dir);
        reflected_ray.start=reflected_ray.start+(reflected_ray.direction*0.00001);
        reflected_ray.normalize_ray();
        double t_minimum_2 = INFINITY;
        double t_current_2 = 0;
        int object_number=-1;
        int index=0;
        Color reflected_color;
        for (Object *obj : object_list)
        {
            t_current_2 = obj->intersect(reflected_ray, reflected_color, 0);
            if (t_current_2 >= 0.0 && (t_minimum_2 - t_current_2) > EPSILON)
            {
                t_minimum_2 = t_current_2;
                object_number=index;
            }
            index++;
        }
        if(abs(t_minimum_2-INFINITY)>EPSILON)
        {
            // it means we have a reflection
            object_list[object_number]->intersect(reflected_ray,reflected_color,level+1);
            color=color+(reflected_color*coEfficients[3]);
            color.fix_precision();
        }
        return parameter_t;


    }



    void print()
    {
       //print vertices and color
      cout<<"A: ";
      a.print();
        cout<<"B: ";
        b.print();
        cout<<"C: ";
        c.print();
        cout<<"Color: ";
        color.print();



    }


};

class Sphere : public Object
{
    // sphere don't need any additional field,the center will be stored in reference point and the radius will be in length
public:
    Sphere(Vector3d c, double r)
    {
        this->reference_point = c;
        this->length = r;
    }
    void draw()
    {
        //cout<<"Sphere draw called"<<endl;
        glPushMatrix();
        {
            //as ref point is the center of the sphere
            glTranslatef(this->reference_point.x, this->reference_point.y, this->reference_point.z);
            double radius = this->length;
            int slices = 24;
            int stacks = 20;
            Vector3d points[100][100];
            for (int i = 0; i <= stacks; i++)
            {
                int h = radius * sin(((double)i / (double)stacks) * (M_PI / 2));
                double r = radius * cos(((double)i / (double)stacks) * (M_PI / 2));
                for (int j = 0; j <= slices; j++)
                {
                    points[i][j].x = r * cos(((double)j / (double)slices) * 2 * M_PI);
                    points[i][j].y = r * sin(((double)j / (double)slices) * 2 * M_PI);
                    points[i][j].z = h;
                }
            }
            for (int i = 0; i < stacks; i++)
            {
                glColor3f(this->color.r, this->color.g, this->color.b);
                for (int j = 0; j < slices; j++)
                {
                    glBegin(GL_QUADS);
                    {
                         // lower hemisphere
                        glVertex3f(points[i][j].x, points[i][j].y, -points[i][j].z);
                        glVertex3f(points[i][j + 1].x, points[i][j + 1].y, -points[i][j + 1].z);
                        glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, -points[i + 1][j + 1].z);
                        glVertex3f(points[i + 1][j].x, points[i + 1][j].y, -points[i + 1][j].z);
                        // upper hemisphere
                        glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
                        glVertex3f(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
                        glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
                        glVertex3f(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);
                       
                    }
                    glEnd();
                }
            }
        }

        glPopMatrix();
    }


    double get_parameter_t(Ray &ray)
    {
        //oc is basically R0
        Vector3d oc = ray.start - this->reference_point; // Vector from ray start to sphere center
        ray.normalize_ray();

        double a = ray.direction.dot(ray.direction);
        double b = 2.0 * oc.dot(ray.direction);
        double c = oc.dot(oc) - this->length * this->length;
        double discriminant = b * b - 4 * a * c;

        if (discriminant < 0)
            return -1.0; // No intersection

        // Find the nearest t value
        double t1 = (-b - sqrt(discriminant)) / (2 * a);
        double t2 = (-b + sqrt(discriminant)) / (2 * a);

        if (t1 > 0 && t2 > 0)
            return std::min(t1, t2);
        if (t1 > 0)
            return t1;
        if (t2 > 0)
            return t2;

        return -1.0; // Intersection behind the ray start
    }
    Vector3d get_normal(Ray &ray, Vector3d &intersection_point)
    {
        Vector3d normal = intersection_point - this->reference_point;
        normal=normal.normalize();
        // check the direction with the ray
        if((ray.start-this->reference_point).length()-this->length*this->length>EPSILON)
        {
            normal=normal*-1;
        }
        return normal;
    }

void pointlight_calculation(Vector3d &intersectionPoint,double &parameter_t,Color &final_color,Vector3d &normal,Ray &ray,Color &intersect_color)
{
     for (auto *light : pointLights)
        {
            double t_minimum = INFINITY;
            double t_current = 0;
            Vector3d dir=intersectionPoint-light->light_position;

            Ray toLight = Ray(light->light_position,dir);
            toLight.normalize_ray();
            
            // check for intersection with other objects
            for (Object *obj : object_list)
            {
                t_current = obj->intersect(toLight, this->color, 0);
                if (t_current > 0.0 && (t_minimum - t_current) > EPSILON)
                {
                    t_minimum = t_current;
                }
            }
            if ((parameter_t - t_minimum) > EPSILON)
            {
                continue;
                // it means the light is shadowed by another object
            }
            Vector3d temp=toLight.direction*-1;
            double lambert_value = normal.dot(temp);

            Vector3d ray_r_dir=normal*2*lambert_value+toLight.direction;

            Ray ray_R = Ray(intersectionPoint,ray_r_dir);

            ray_R.normalize_ray();
            Vector3d temp2=ray.direction*-1;
            double coeff_temp=ray_R.direction.dot(temp2);

            double phong_value = pow(coeff_temp, shine);
            final_color = final_color + (light->color * coEfficients[1] * max(lambert_value, 0) * intersect_color);
            final_color.fix_precision();
            final_color = final_color + (light->color * coEfficients[2] * max(phong_value, 0) * intersect_color);
            final_color.fix_precision();
        }
    
}

void spotlight_calculation(Vector3d &intersectionPoint,double &parameter_t,Color &final_color,Vector3d &normal,Ray &ray,Color &intersect_color)
{
     for (Spotlight *spot_light : spotLights)
        {
              double t_minimum = INFINITY;
            double t_current = 0;
            Vector3d dir=intersectionPoint-spot_light->point_light.light_position;
            Ray ray_L = Ray(spot_light->point_light.light_position, dir);
            ray_L.normalize_ray();
            if (spot_light->crossed_cuttoff_or_not(ray_L))
            {
                continue;
            }
          
            for (Object *obj : object_list)
            {
                t_current = obj->intersect(ray_L, color, 0);
                if (t_current > 0.0 && (t_minimum - t_current) > EPSILON)
                {
                    t_minimum = t_current;
                }
            }
            if ((parameter_t - t_minimum) > EPSILON)
            {
                continue;
            }
            // now calculate the lambert and phong value
            Vector3d temp=ray_L.direction*-1;
            double lambert_value = normal.dot(temp);
            Vector3d ray_r_dir=normal*2*lambert_value+ray_L.direction;
            Ray ray_R = Ray(intersectionPoint, ray_r_dir);
            ray_R.normalize_ray();
            Vector3d temp2=ray.direction*-1;
            double coeff=ray_R.direction.dot(temp2);

            double phong_value = pow(coeff, shine);
            final_color = final_color + (spot_light->point_light.color * coEfficients[1] * max(lambert_value, 0) * intersect_color);
            final_color.fix_precision();
            final_color = final_color + (spot_light->point_light.color * coEfficients[2] * max(phong_value, 0) * intersect_color);
            final_color.fix_precision();
        }
}

  double intersect(Ray &ray, Color &color, int level = 0)
    {
        //cout<<"Triangle intersect called"<<endl;

        // normalize the ray
        ray.normalize_ray();

        // now get the parameter needed for futher calculation

        double parameter_t = get_parameter_t(ray); // Find intersection
        if (parameter_t < 0)
            return parameter_t; // No intersection
        if (level == 0)
            return parameter_t;

        Vector3d intersectionPoint = ray.start + ray.direction * parameter_t;
        Vector3d normal = this->get_normal(ray,intersectionPoint);
        Color intersect_color = this->color;
        Color final_color=color;
        final_color = intersect_color * coEfficients[0]; // ambient color
        final_color.fix_precision();


        // for (auto *light : pointLights)
        // {

        //     Ray toLight = Ray(light->light_position, intersectionPoint - light->light_position);
        //     toLight.normalize_ray();
        //     double t_minimum = INFINITY;
        //     double t_current = 0;
        //     // check for intersection with other objects
        //     for (Object *obj : object_list)
        //     {
        //         t_current = obj->intersect(toLight, this->color, 0);
        //         if (t_current > 0.0 && (t_minimum - t_current) > EPSILON)
        //         {
        //             t_minimum = t_current;
        //         }
        //     }
        //     if ((parameter_t - t_minimum) > EPSILON)
        //     {
        //         continue;
        //         // it means the light is shadowed by another object
        //     }
        //     double lambert_value = normal.dot(toLight.direction * -1);
        //     Ray ray_R = Ray(intersectionPoint, normal * 2 * lambert_value + toLight.direction);
        //     ray_R.normalize_ray();
        //     double phong_value = pow(ray_R.direction.dot(ray.direction * -1), shine);
        //     final_color = final_color + (light->color * coEfficients[1] * max(lambert_value, 0) * intersect_color);
        //     final_color.fix_precision();
        //     final_color = final_color + (light->color * coEfficients[2] * max(phong_value, 0) * intersect_color);
        //     final_color.fix_precision();
        // }
        pointlight_calculation(intersectionPoint,parameter_t,final_color,normal,ray,intersect_color);



        // now for spotlights the calculation will be same
        // for (Spotlight *spot_light : spotLights)
        // {
        //     Ray ray_L = Ray(spot_light->point_light.light_position, intersectionPoint - spot_light->point_light.light_position);
        //     ray_L.normalize_ray();
        //     if (spot_light->crossed_cuttoff_or_not(ray_L))
        //     {
        //         continue;
        //     }
        //     double t_minimum = INFINITY;
        //     double t_current = 0;
        //     for (Object *obj : object_list)
        //     {
        //         t_current = obj->intersect(ray_L, color, 0);
        //         if (t_current > 0.0 && (t_minimum - t_current) > EPSILON)
        //         {
        //             t_minimum = t_current;
        //         }
        //     }
        //     if ((parameter_t - t_minimum) > EPSILON)
        //     {
        //         continue;
        //     }
        //     // now calculate the lambert and phong value
        //     double lambert_value = normal.dot(ray_L.direction * -1);
        //     Ray ray_R = Ray(intersectionPoint, normal * (2 * lambert_value) + ray_L.direction);
        //     ray_R.normalize_ray();
        //     double phong_value = pow(ray_R.direction.dot(ray.direction * -1), shine);
        //     final_color = final_color + (spot_light->point_light.color * coEfficients[1] * max(lambert_value, 0) * intersect_color);
        //     final_color.fix_precision();
        //     final_color = final_color + (spot_light->point_light.color * coEfficients[2] * max(phong_value, 0) * intersect_color);
        //     final_color.fix_precision();
        // }
        spotlight_calculation(intersectionPoint,parameter_t,final_color,normal,ray,intersect_color);


        color=final_color;
        // color.print();
        // final_color.print();

        if(level>=recursion_level)
        return parameter_t;
        // now for reflection
          Vector3d reflected_ray_dir=ray.direction+(normal*2*(normal.dot(ray.direction*-1) ));
        
        Ray reflected_ray=Ray(intersectionPoint, reflected_ray_dir);
        reflected_ray.start=reflected_ray.start+(reflected_ray.direction*0.00001);
        reflected_ray.normalize_ray();
        double t_minimum_2 = INFINITY;
        double t_current_2 = 0;
        int object_number=-1;
        int index=0;
        Color reflected_color;
        for (Object *obj : object_list)
        {
            t_current_2 = obj->intersect(reflected_ray, reflected_color, 0);
            if (t_current_2 >= 0.0 && (t_minimum_2 - t_current_2) > EPSILON)
            {
                t_minimum_2 = t_current_2;
                object_number=index;
            }
            index++;
        }
        if(abs(t_minimum_2-INFINITY)>EPSILON)
        {
            // it means we have a reflection
            object_list[object_number]->intersect(reflected_ray,reflected_color,level+1);
            color=color+(reflected_color*coEfficients[3]);
            color.fix_precision();
        }
        return parameter_t;


    }



    void print()
    {
        cout << "Center: ";
        reference_point.print();
        cout << "Radius: " << length << endl;
        cout << "Color: ";
        color.print();
    
    }
};

class General : public Object
{
    // this class is for the quadratic objects
public:
    double A, B, C, D, E, F, G, H, I, J;
    Vector3d cube_ref_point;

    General()
    {
    }
    General(double a, double b, double c, double d, double e, double f, double g, double h, double i, double j, Vector3d ref_point, double length, double width, double height)
    {
        A = a;
        B = b;
        C = c;
        D = d;
        E = e;
        F = f;
        G = g;
        H = h;
        I = i;
        J = j;
        cube_ref_point = ref_point;
        this->length = length;
        this->width = width;
        this->height = height;
    }
    double get_parameter_t(Ray &ray)
    {
        // Coefficients of the quadratic equation (At^2 + Bt + C = 0)
        double a, b, c;

        // Calculate the coefficients based on the general quadratic equation
        a = A * pow(ray.direction.x, 2) + B * pow(ray.direction.y, 2) + C * pow(ray.direction.z, 2) +
            D * ray.direction.x * ray.direction.y +
            E * ray.direction.x * ray.direction.z + F * ray.direction.y * ray.direction.z;

        b = 2.0 * (A * ray.start.x * ray.direction.x + B * ray.start.y * ray.direction.y + C * ray.start.z * ray.direction.z) +
            D * (ray.start.x * ray.direction.y + ray.start.y * ray.direction.x) +
            E * (ray.start.x * ray.direction.z + ray.start.z * ray.direction.x) +
            F * (ray.start.y * ray.direction.z + ray.start.z * ray.direction.y) +
            G * ray.direction.x + H * ray.direction.y + I * ray.direction.z;

        c = A * pow(ray.start.x, 2) + B * pow(ray.start.y, 2) + C * pow(ray.start.z, 2) +
            D * ray.start.x * ray.start.y + E * ray.start.x * ray.start.z + F * ray.start.y * ray.start.z +
            G * ray.start.x + H * ray.start.y + I * ray.start.z + J;

        // Calculate discriminant
        double discriminant = b * b - 4 * a * c;

        // No real roots, no intersection
        if (discriminant < 0)
            return -1;

        // Calculate the two possible values of t
        double t1 = (-b - sqrt(discriminant)) / (2 * a);
        double t2 = (-b + sqrt(discriminant)) / (2 * a);
        double ans=-1;

        // Return the smallest positive t, or -1 if no positive t exists
        if(t1>0.0 )
        {
            Vector3d i_p=ray.start+(ray.direction*t1);
            if ( (length>0.0 && ((this->cube_ref_point.x-i_p.x)>EPSILON  || (i_p.x-this->cube_ref_point.x-this->length)>EPSILON ) )
            || (width>0.0 && ((this->cube_ref_point.y-i_p.y)>EPSILON  || (i_p.y-this->cube_ref_point.y-this->width)>EPSILON ) )
            || (height>0.0 && ((this->cube_ref_point.z-i_p.z)>EPSILON  || (i_p.z-this->cube_ref_point.z-this->height)>EPSILON ) ) )
            {
               t1=-1;
            
            }
            else
            {
                ans=t1;

            }

        }
        if(t1<0.0 && t2>0.0)
        {
            Vector3d i_p=ray.start+(ray.direction*t2);
             if ( (length>0.0 && ((this->cube_ref_point.x-i_p.x)>EPSILON  || (i_p.x-this->cube_ref_point.x-this->length)>EPSILON ) )
            || (width>0.0 && ((this->cube_ref_point.y-i_p.y)>EPSILON  || (i_p.y-this->cube_ref_point.y-this->width)>EPSILON ) )
            || (height>0.0 && ((this->cube_ref_point.z-i_p.z)>EPSILON  || (i_p.z-this->cube_ref_point.z-this->height)>EPSILON ) ) )
            {
                ans=-1;
            
            }
            else
            {
                ans=t2;

            }

        }
        return ans;
        
      
    }
    Vector3d get_normal(Ray &ray,Vector3d &point)
    {
        Vector3d normal;
        normal.x = 2 * A * point.x + D * point.y + E * point.z + G;
        normal.y = 2 * B * point.y + D * point.x + F * point.z + H;
        normal.z = 2 * C * point.z + E * point.x + F * point.y + I;

        normal = normal.normalize(); // Ensure the normal is a unit vector
        if((ray.start-this->reference_point).dot(normal)>0.0)
        {
            normal=normal*-1;
        }

        return normal;
    }
    void pointlight_calculation(Vector3d &intersectionPoint,double &parameter_t,Color &final_color,Vector3d &normal,Ray &ray,Color &intersect_color)
{
     for (auto *light : pointLights)
        {
            double t_minimum = INFINITY;
            double t_current = 0;
            Vector3d dir=intersectionPoint-light->light_position;

            Ray toLight = Ray(light->light_position,dir);
            toLight.normalize_ray();
            
            // check for intersection with other objects
            for (Object *obj : object_list)
            {
                t_current = obj->intersect(toLight, this->color, 0);
                if (t_current > 0.0 && (t_minimum - t_current) > EPSILON)
                {
                    t_minimum = t_current;
                }
            }
            if ((parameter_t - t_minimum) > EPSILON)
            {
                continue;
                // it means the light is shadowed by another object
            }
            Vector3d temp=toLight.direction*-1;
            double lambert_value = normal.dot(temp);

            Vector3d ray_r_dir=normal*2*lambert_value+toLight.direction;

            Ray ray_R = Ray(intersectionPoint,ray_r_dir);

            ray_R.normalize_ray();
            Vector3d temp2=ray.direction*-1;
            double coeff_temp=ray_R.direction.dot(temp2);

            double phong_value = pow(coeff_temp, shine);
            final_color = final_color + (light->color * coEfficients[1] * max(lambert_value, 0) * intersect_color);
            final_color.fix_precision();
            final_color = final_color + (light->color * coEfficients[2] * max(phong_value, 0) * intersect_color);
            final_color.fix_precision();
        }
    
}
   void spotlight_calculation(Vector3d &intersectionPoint,double &parameter_t,Color &final_color,Vector3d &normal,Ray &ray,Color &intersect_color)
{
     for (Spotlight *spot_light : spotLights)
        {
              double t_minimum = INFINITY;
            double t_current = 0;
            Vector3d dir=intersectionPoint-spot_light->point_light.light_position;
            Ray ray_L = Ray(spot_light->point_light.light_position, dir);
            ray_L.normalize_ray();
            if (spot_light->crossed_cuttoff_or_not(ray_L))
            {
                continue;
            }
          
            for (Object *obj : object_list)
            {
                t_current = obj->intersect(ray_L, color, 0);
                if (t_current > 0.0 && (t_minimum - t_current) > EPSILON)
                {
                    t_minimum = t_current;
                }
            }
            if ((parameter_t - t_minimum) > EPSILON)
            {
                continue;
            }
            // now calculate the lambert and phong value
            Vector3d temp=ray_L.direction*-1;
            double lambert_value = normal.dot(temp);
            Vector3d ray_r_dir=normal*2*lambert_value+ray_L.direction;
            Ray ray_R = Ray(intersectionPoint, ray_r_dir);
            ray_R.normalize_ray();
            Vector3d temp2=ray.direction*-1;
            double coeff=ray_R.direction.dot(temp2);

            double phong_value = pow(coeff, shine);
            final_color = final_color + (spot_light->point_light.color * coEfficients[1] * max(lambert_value, 0) * intersect_color);
            final_color.fix_precision();
            final_color = final_color + (spot_light->point_light.color * coEfficients[2] * max(phong_value, 0) * intersect_color);
            final_color.fix_precision();
        }
}


       double intersect(Ray &ray, Color &color, int level = 0)
    {
        //cout<<"Triangle intersect called"<<endl;

        // normalize the ray
        ray.normalize_ray();

        // now get the parameter needed for futher calculation

        double parameter_t = get_parameter_t(ray); // Find intersection
        if (parameter_t < 0)
            return parameter_t; // No intersection
        if (level == 0)
            return parameter_t;

        Vector3d intersectionPoint = ray.start + ray.direction * parameter_t;
        Vector3d normal = this->get_normal(ray,intersectionPoint);
        Color intersect_color = this->color;
        Color final_color=color;
        final_color = intersect_color * coEfficients[0]; // ambient color
        final_color.fix_precision();

        // for (auto *light : pointLights)
        // {

        //     Ray toLight = Ray(light->light_position, intersectionPoint - light->light_position);
        //     toLight.normalize_ray();
        //     double t_minimum = INFINITY;
        //     double t_current = 0;
        //     // check for intersection with other objects
        //     for (Object *obj : object_list)
        //     {
        //         t_current = obj->intersect(toLight, this->color, 0);
        //         if (t_current > 0.0 && (t_minimum - t_current) > EPSILON)
        //         {
        //             t_minimum = t_current;
        //         }
        //     }
        //     if ((parameter_t - t_minimum) > EPSILON)
        //     {
        //         continue;
        //         // it means the light is shadowed by another object
        //     }
        //     double lambert_value = normal.dot(toLight.direction * -1);
        //     Ray ray_R = Ray(intersectionPoint, normal * 2 * lambert_value + toLight.direction);
        //     ray_R.normalize_ray();
        //     double phong_value = pow(ray_R.direction.dot(ray.direction * -1), shine);
        //     final_color = final_color + (light->color * coEfficients[1] * max(lambert_value, 0) * intersect_color);
        //     final_color.fix_precision();
        //     final_color = final_color + (light->color * coEfficients[2] * max(phong_value, 0) * intersect_color);
        //     final_color.fix_precision();
        // }
        pointlight_calculation(intersectionPoint,parameter_t,final_color,normal,ray,intersect_color);
        // now for spotlights the calculation will be same
        // for (Spotlight *spot_light : spotLights)
        // {
        //     Ray ray_L = Ray(spot_light->point_light.light_position, intersectionPoint - spot_light->point_light.light_position);
        //     ray_L.normalize_ray();
        //     if (spot_light->crossed_cuttoff_or_not(ray_L))
        //     {
        //         continue;
        //     }
        //     double t_minimum = INFINITY;
        //     double t_current = 0;
        //     for (Object *obj : object_list)
        //     {
        //         t_current = obj->intersect(ray_L, color, 0);
        //         if (t_current > 0.0 && (t_minimum - t_current) > EPSILON)
        //         {
        //             t_minimum = t_current;
        //         }
        //     }
        //     if ((parameter_t - t_minimum) > EPSILON)
        //     {
        //         continue;
        //     }
        //     // now calculate the lambert and phong value
        //     double lambert_value = normal.dot(ray_L.direction * -1);
        //     Ray ray_R = Ray(intersectionPoint, normal * (2 * lambert_value) + ray_L.direction);
        //     ray_R.normalize_ray();
        //     double phong_value = pow(ray_R.direction.dot(ray.direction * -1), shine);
        //     final_color = final_color + (spot_light->point_light.color * coEfficients[1] * max(lambert_value, 0) * intersect_color);
        //     final_color.fix_precision();
        //     final_color = final_color + (spot_light->point_light.color * coEfficients[2] * max(phong_value, 0) * intersect_color);
        //     final_color.fix_precision();
        // }
        spotlight_calculation(intersectionPoint,parameter_t,final_color,normal,ray,intersect_color);
        color=final_color;
        // color.print();
        // final_color.print();

        if(level>=recursion_level)
        return parameter_t;
         double t_minimum_2 = INFINITY;
        double t_current_2 = 0;
        // now for reflection
          Vector3d reflected_ray_dir=ray.direction+(normal*2*(normal.dot(ray.direction*-1) ));
        
        Ray reflected_ray=Ray(intersectionPoint, reflected_ray_dir);
        reflected_ray.start=reflected_ray.start+(reflected_ray.direction*0.00001);
        reflected_ray.normalize_ray();
       
        int object_number=-1,index=0;
        Color reflected_color;
        for (Object *obj : object_list)
        {
            t_current_2 = obj->intersect(reflected_ray, reflected_color, 0);
            if (t_current_2 >= 0.0 && (t_minimum_2 - t_current_2) > EPSILON)
            {
                t_minimum_2 = t_current_2;
                object_number=index;
            }
            index++;
        }
        if(abs(t_minimum_2-INFINITY)>EPSILON)
        {
            // it means we have a reflection
            object_list[object_number]->intersect(reflected_ray,reflected_color,level+1);
            color=color+(reflected_color*coEfficients[3]);
            color.fix_precision();
        }
        return parameter_t;


    }
           
    void print()
    {
        cout << "A: " << A << " B: " << B << " C: " << C << " D: " << D << " E: " << E << " F: " << F << " G: " << G << " H: " << H << " I: " << I << " J: " << J << endl;
        cout << "Reference Point: ";
        cube_ref_point.print();
        cout << "Length: " << length << " Width: " << width << " Height: " << height << endl;
        cout << "Color: ";
        color.print();
    
    }
};
class Floor : public Object
{
public:
    double floor_width;
    double tile_width;
    int num_tiles;

    Floor(double floor_width, double tileWidth)
    {
        this->reference_point = Vector3d(-floor_width / 2, -floor_width / 2, 0);
        this->length = floor_width;
        this->floor_width = floor_width;
        this->tile_width = tileWidth;
        this->num_tiles = floor_width / tileWidth;
    }
    void draw()
    {
        //cout<<"Floor draw called"<<endl;
        glPushMatrix();
        {
            for (int row = 0; row < this->num_tiles; row++)
            {
                for (int col = 0; col < this->num_tiles; col++)
                {
                    if ((row + col) % 2 == 0)
                    {
                        glColor3f(1, 1, 1);
                    }
                    else
                    {
                        glColor3f(0, 0, 0);
                    }
                    // Draw a rectangle as the floor
                    // for a sqaure the reference point will be always refpoint.x/y+tile_width*row/col
                    // for other points just add the tile_width to the reference point

                    glBegin(GL_QUADS);
                    {
                        glVertex3f((this->reference_point.x + row * this->tile_width), (this->reference_point.y + col * this->tile_width), 0);
                        glVertex3f((this->reference_point.x + row * this->tile_width), this->reference_point.y + (col + 1) * this->tile_width, 0);
                        glVertex3f(this->reference_point.x + (row + 1) * this->tile_width, this->reference_point.y + (col + 1) * this->tile_width, 0);
                        glVertex3f(this->reference_point.x + (row + 1) * this->tile_width, this->reference_point.y + col * this->tile_width, 0);
                    }

                    glEnd();
                }
            }
        }
        glPopMatrix();
    }
    double get_parameter_t(Ray &ray)
    {
        Vector3d normal=Vector3d(0.0,0.0,1.0);
        //because at that case ray and floor are parallel. 
        if(ray.direction.dot(normal)==0.0){
            return -1;
            }
        //altered
        double temp=-ray.start.dot(normal);
        double parameter_t=temp/ray.direction.dot(normal);
        Vector3d intersection_point=ray.start+ray.direction*parameter_t;

        if( (intersection_point.x>=this->reference_point.x && 
        intersection_point.x<=this->reference_point.x+this->floor_width ) &&
         (intersection_point.y>=this->reference_point.y && 
         intersection_point.y<=this->reference_point.y+this->floor_width) )
        {
            return parameter_t;
        }
        else
        return -1;

    }
    Vector3d get_normal(Ray &ray,Vector3d &intersection)
    {
        Vector3d normal_vector = Vector3d(0.0, 0.0, 1.0);
        if(ray.start.z < 0.0)
            normal_vector.z = normal_vector.z*-1.0;
        return normal_vector;

    }
     double intersect(Ray &ray, Color &color, int level = 0)
    {
        //cout<<"Triangle intersect called"<<endl;

        // normalize the ray
        ray.normalize_ray();

        // now get the parameter needed for futher calculation

        double parameter_t = get_parameter_t(ray); // Find intersection
        if (parameter_t < 0)
            return parameter_t; // No intersection
        if (level == 0)
            return parameter_t;

        Vector3d intersectionPoint = ray.start + ray.direction * parameter_t;
        Vector3d normal = this->get_normal(ray,intersectionPoint);
        Color temp_col;
         int i = (intersectionPoint.x - this->reference_point.x) / tile_width;
        int j = (intersectionPoint.y - this->reference_point.y) / tile_width;
        if((i+j) % 2 == 0) temp_col.set_color(1,1,1);
        else 
        temp_col.set_color(0,0,0);
        Color intersect_color = temp_col;
        Color final_color=color;
        final_color = intersect_color * coEfficients[0]; // ambient color
        final_color.fix_precision();

        for (auto *light : pointLights)
        {
               double t_minimum = INFINITY;
            double t_current = 0;

            Ray toLight = Ray(light->light_position, intersectionPoint - light->light_position);
            toLight.normalize_ray();
         
            // check for intersection with other objects
            for (Object *obj : object_list)
            {
                t_current = obj->intersect(toLight, this->color, 0);
                if (t_current > 0.0 && (t_minimum - t_current) > EPSILON)
                {
                    t_minimum = t_current;
                }
            }
            if ((parameter_t - t_minimum) > EPSILON)
            {
                continue;
                // it means the light is shadowed by another object
            }
            double lambert_value = normal.dot(toLight.direction * -1);
            Ray ray_R = Ray(intersectionPoint, normal * 2 * lambert_value + toLight.direction);
            ray_R.normalize_ray();
            double phong_value = pow(ray_R.direction.dot(ray.direction * -1), shine);
            final_color = final_color + (light->color * coEfficients[1] * max(lambert_value, 0) * intersect_color);
            final_color.fix_precision();
            final_color = final_color + (light->color * coEfficients[2] * max(phong_value, 0) * intersect_color);
            final_color.fix_precision();
        }
        // now for spotlights the calculation will be same
        for (Spotlight *spot_light : spotLights)
        {
             double t_minimum = INFINITY;
            double t_current = 0;
            Ray ray_L = Ray(spot_light->point_light.light_position, intersectionPoint - spot_light->point_light.light_position);
            ray_L.normalize_ray();
            if (spot_light->crossed_cuttoff_or_not(ray_L))
            {
                continue;
            }
           
            for (Object *obj : object_list)
            {
                t_current = obj->intersect(ray_L, color, 0);
                if (t_current > 0.0 && (t_minimum - t_current) > EPSILON)
                {
                    t_minimum = t_current;
                }
            }
            if ((parameter_t - t_minimum) > EPSILON)
            {
                continue;
            }
            // now calculate the lambert and phong value
            double lambert_value = normal.dot(ray_L.direction * -1);
            Ray ray_R = Ray(intersectionPoint, normal * (2 * lambert_value) + ray_L.direction);
            ray_R.normalize_ray();
            double phong_value = pow(ray_R.direction.dot(ray.direction * -1), shine);
            final_color = final_color + (spot_light->point_light.color * coEfficients[1] * max(lambert_value, 0) * intersect_color);
            final_color.fix_precision();
            final_color = final_color + (spot_light->point_light.color * coEfficients[2] * max(phong_value, 0) * intersect_color);
            final_color.fix_precision();
        }

        color=final_color;
        // color.print();
        // final_color.print();

        if(level>=recursion_level)
        return parameter_t;
        // now for reflection

        Ray reflected_ray=Ray(intersectionPoint,ray.direction+(normal*2*(normal.dot(ray.direction*-1) )));
        reflected_ray.start=reflected_ray.start+(reflected_ray.direction*0.00001);
        reflected_ray.normalize_ray();
        double t_minimum_2 = INFINITY;
        double t_current_2 = 0;
        int object_number=-1,index=0;
        Color reflected_color;
        for (Object *obj : object_list)
        {
            t_current_2 = obj->intersect(reflected_ray, reflected_color, 0);
            if (t_current_2 >= 0.0 && (t_minimum_2 - t_current_2) > EPSILON)
            {
                t_minimum_2 = t_current_2;
                object_number=index;
            }
            index++;
        }
        if(abs(t_minimum_2-INFINITY)>EPSILON)
        {
            // it means we have a reflection
            object_list[object_number]->intersect(reflected_ray,reflected_color,level+1);
            color=color+(reflected_color*coEfficients[3]);
            color.fix_precision();
        }
        return parameter_t;


    }
   

};
