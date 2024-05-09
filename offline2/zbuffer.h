#pragma once
#include <bits/stdc++.h>
using namespace std;
#include "Vector.h"
#include "bitmap_image.hpp"

// write a class for the color
class Color
{
private:
public:
    int r, g, b; // red green blue

    // constructor
    Color(int R = 0, int G = 0, int B = 0)
    {
        setColor(R, G, B);
    }

    void setColor(int red, int green, int blue)
    {
        this->r = red;
        this->g = green;
        this->b = blue;
    }
    void printColor()
    {
        cout << "The color is : Red = " << r << " Green = " << g
             << " Blue = " << b << endl;
    }
};
// triangle class using vector
class Triangle
{
public:
    Vector3D v1, v2, v3;
    Color color;
    //default constructor
    Triangle(){
        
    }
    Triangle(Vector3D v1, Vector3D v2, Vector3D v3, Color color)
    {
        this->v1 = v1;
        this->v2 = v2;
        this->v3 = v3;
        this->color = color;
    }
    void printTriangle()
    {
        cout << "The vertices of the triangle are : " << endl;
        v1.print();
        v2.print();
        v3.print();
        color.printColor();
    }
};
class zBuffer
{
    bitmap_image img;
    int screenWidth;
    int screenHeight;
    vector<Triangle> triangle_vector;
    // define others for zbuffer
    double **zbuffer;
    int z_min;
    int z_max;

    

    double x_left;
    double x_right;
    double y_up;
    double y_down;

    double dx;
    double dy;

public:
    // constructor
    zBuffer(vector<Triangle> triangle_v, int s_w, int s_h)
    {
        int x_left_max;
    int x_right_max;
    int y_up_max;
    int y_down_max;
        this->triangle_vector = triangle_v;
        this->screenWidth = s_w;
        this->screenHeight = s_h;

        this->zbuffer = new double *[this->screenWidth];
        this->z_max = 1;
        this->z_min = -1;
        for (int i = 0; i < screenWidth; i++)
        {
            this->zbuffer[i] = new double[this->screenHeight];
            for (int j = 0; j < screenHeight; j++)
            {
                this->zbuffer[i][j] = z_max;
            }
        }
        // create a pixel mapping
        x_left_max = y_down_max = -1;
        x_right_max = y_up_max = 1;
        dx = (x_right_max - x_left_max) / (double)screenWidth;
        dy = (y_up_max - y_down_max) / (double)screenHeight;

        x_left = x_left_max + (dx / 2.0);
        x_right = x_right_max - (dx / 2.0);
        y_up = y_up_max - (dy / 2.0);
        y_down = y_down_max +(dy / 2.0);
        img.setwidth_height(screenWidth, screenHeight);
    }
    ~zBuffer()
    {
        for (int i = 0; i < screenWidth; i++)
        {
            delete[] zbuffer[i];
        }
        delete[] zbuffer;
    }
    // find max for 2
    static inline double find_max(double &a, double &b)
    {
        return (b > a ? b : a);
    }

    // find_max for 3 doubles
    static double find_max3(double a, double b, double c)
    {
        double m = find_max(a, b);
        return find_max(m, c);
    }
    // find min for doubles
    static double find_min(double a, double b)
    {
        return (b < a ? b : a);
    }
    // find_min for 3 doubles
    static double find_min3(double a, double b, double c)
    {
        double m = find_min(a, b);
        return find_min(m, c);
    }
     void save_zbuffer_to_file(string fname)
    {
        ofstream outFile(fname);
        for(int i=0;i<screenHeight;i++)
        {
            for(int j=0;j<screenWidth;j++)
            {
                if(this->zbuffer[i][j]<this->z_max)
                {
                    outFile<<this->zbuffer[i][j]<<"\t";
                }
            }
            outFile<<endl;

        }
        cout<<"ZBuffer saved to "<<fname<<endl;
        outFile.close();

    }
    
    void saveImage(string fname)
    {
        img.save_image(fname);
        cout << "Image saved to " << fname << endl;
    }

    void performCalculations()
    {
        // we will perform z-buffer here
        
        for (auto tri : triangle_vector)
        {
             // compute the min y of three vectors in tri
            double min_y = find_min3(tri.v1.y, tri.v2.y, tri.v3.y);

            int temp = (int)round((find_max(min_y, y_down) - y_down) / dy);
            // this is where the triangle ends vertically
            int bottom_scanline = screenHeight - temp;
           

            double max_y = find_max3(tri.v1.y, tri.v2.y, tri.v3.y);
            // this is where the triangle starts vertically
            int top_scaneline = (int)round((y_up - find_min(y_up, max_y)) / dy);
           

            for (int r = top_scaneline; r <= bottom_scanline; r++)
            {
                // this is current scanline
                double scanline_y = y_up - r * dy;
                // where the triangle intersects the current scanline horizontally
                // min_x for leftmost and max_x for rightmost
                double min_x;
                double max_x;
                double z_start;
                double z_end;

                min_x = INT_MAX;
                max_x = -INT_MAX;
                for (int i = 0; i < 3; i++)
                {
                    Vector3D vector1;
                    Vector3D vector2;
                    if (i == 0)
                    {
                        vector1 = tri.v1;
                        vector2 = tri.v2;
                    }
                    else if (i == 1)
                    {
                        vector1 = tri.v2;
                        vector2 = tri.v3;
                    }
                    else
                    {
                        vector1 = tri.v3;
                        vector2 = tri.v1;
                    }
                    //where scanline intersects the current edge
        double x = (scanline_y - vector1.y) * (vector2.x - vector1.x) / (vector2.y - vector1.y) + vector1.x;
                    //check whether the intersection is within edge range or not
                    if (x >= min(vector1.x, vector2.x) && x <= max(vector1.x, vector2.x))
                    {
                        double z = (scanline_y - vector1.y) * (vector2.z - vector1.z) / (vector2.y - vector1.y) + vector1.z;
                        if (vector1.y == vector2.y)
                        {
                           size_t scanline_from_point=((vector1.y+1.0)/2.0)*screenHeight;
                           if(scanline_from_point==screenHeight)
                            {
                                scanline_from_point--;
                            }
                            

                            if (vector1.y == scanline_y)
                            {
                                if (vector1.x < min_x)
                                {
                                    min_x = vector1.x;
                                    z_start = vector1.z;
                                }
                                if (vector1.x > max_x)
                                {
                                    max_x = vector1.x;
                                    z_end = vector1.z;
                                }
                                if (vector2.x < min_x)
                                {
                                    min_x = vector2.x;
                                    z_start = vector2.z;
                                }
                                if (vector2.x > max_x)
                                {
                                    max_x = vector2.x;
                                    z_end = vector2.z;
                                }

                            }
                        }
                        else
                        {
                            if (x < min_x)
                            {
                                min_x = x;
                                z_start = z;
                            }
                            if (x > max_x)
                            {
                                max_x = x;
                                z_end = z;
                            }
                        }
                    }
                }
                //skip if no valid intersection
                if (min_x == INT_MAX || max_x == -INT_MAX)
                    continue;
                    //left and right intersecting column
                int r_inter_col = screenWidth - (int)round((x_right - find_min(x_right, max_x)) / dx);

                int l_inter_col = (int)round((find_max(x_left, min_x) - x_left) / dx);
                for (int col = l_inter_col; col <= r_inter_col; col++)
                {
                    double scanline_x = x_left + col * dx;
                    double z = (scanline_x - min_x) * (z_end - z_start) / (max_x - min_x) + z_start;

                
                    
                        if (z >= z_min && z <= z_max)
                        {
                            if (z < zbuffer[r][col])
                            {
                                zbuffer[r][col] = z;
                                img.set_pixel(col, r, tri.color.r, tri.color.g, tri.color.b);
                            }
                        }
                    
                }
            }
        }
    }
    
   
};
