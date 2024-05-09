#pragma once

#include <iostream>
#include <cmath>
using namespace std;
class Vector3D {
public:
    double x, y, z;

    // Default constructor
    Vector3D() : x(0), y(0), z(0) {}

    // Constructor with three parameters for 3D vector
    Vector3D(double x, double y, double z) : x(x), y(y), z(z) {}

    // Destructor
    ~Vector3D() {}
    //copy constructor
    Vector3D(const Vector3D &v) 
    {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
    }

    // Add two vectors
    Vector3D operator+(const Vector3D& v) const {
        return Vector3D(x + v.x, y + v.y, z + v.z);
    }

    // Subtract two vectors
    Vector3D operator-(const Vector3D& v) const {
        return Vector3D(x - v.x, y - v.y, z - v.z);
    }

    // Multiply vector by scalar
    Vector3D operator*(double scalar) const {
        return Vector3D(x * scalar, y * scalar, z * scalar);
    }

    // Calculate dot product
    float dot(const Vector3D& v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    // Calculate cross product
    Vector3D cross(const Vector3D& v) const {
        return Vector3D(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }

    // Calculate magnitude of the vector
    float magnitude() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    // Normalize the vector
    void normalize() {
        float m = magnitude();
        if (m > 0) {
            x /= m;
            y /= m;
            z /= m;
        }
    }

    // Utility function for printing the vector
    void print() const {
        std::cout << "(" << x << ", " << y << ", " << z << ")" << std::endl;
    }
};