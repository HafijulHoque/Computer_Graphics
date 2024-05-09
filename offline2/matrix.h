#include<bits/stdc++.h>
using namespace std;


class Matrix {
    private:
        float mat[4][4];

    public:
        // Constructor to initialize a zero matrix
        Matrix() {
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    mat[i][j] = 0;
        }

        // Static method to create an identity matrix
        static Matrix identity() {
            Matrix m;
            for (int i = 0; i < 4; ++i)
                m.mat[i][i] = 1;
            return m;
        }

        // Method to set a value at a specific position
        void set(int row, int col, float value) {
            mat[row][col] = value;
        }

        // Method to get a value from a specific position
        float get(int row, int col) const {
            return mat[row][col];
        }

        // Overload the * operator for matrix multiplication
        Matrix operator*(const Matrix& other) const {
            Matrix result;
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 4; ++j) {
                    for (int k = 0; k < 4; ++k) {
                        result.mat[i][j] += mat[i][k] * other.mat[k][j];
                    }
                }
            }
            return result;
        }

        // Utility function for matrix translation
        static Matrix translate(float tx, float ty, float tz) {
            Matrix m = Matrix::identity();
            m.set(0, 3, tx);
            m.set(1, 3, ty);
            m.set(2, 3, tz);
            return m;
        }

        // Utility function for matrix scaling
        static Matrix scale(float sx, float sy, float sz) {
            Matrix m = Matrix::identity();
            m.set(0, 0, sx);
            m.set(1, 1, sy);
            m.set(2, 2, sz);
            return m;
        }

     
        // Method to print the matrix (for debugging purposes)
        void print() const {
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 4; ++j) {
                    cout << mat[i][j] << " ";
                }
                cout << endl;
            }
        }
};

