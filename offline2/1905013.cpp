
#include<bits/stdc++.h>
#include "Vector.h"
#include "zbuffer.h"

#include "matrix.h"

#include <random>
#include <chrono>
#define WIDTH_PRECISION(x, y) fixed<<setprecision(y)

using namespace std;
string congig_filename="";
string scene_filename="";
int generateRandomNumber() {
   static unsigned long int g_seed = 1;

g_seed = (214013 * g_seed + 2531011);
return (g_seed >> 16) & 0x7FFF;

}

// Function to generate random RGB values
void generateRandomRGB(int& r, int& g, int& b) {
    r = generateRandomNumber()%256;
    g = generateRandomNumber()%256;
    b = generateRandomNumber()%256;
}
//rodrigues formula

Vector3D rodriguesRotation(Vector3D v, Vector3D k, double angle)
{
    //convert angle to radians
    
    //calculate the first term
    Vector3D firstTerm=v*cos(angle);
    //calculate the second term
    Vector3D secondTerm=k.cross(v)*sin(angle);
    //calculate the third term
    Vector3D thirdTerm=k*(k.dot(v))*(1-cos(angle));
    //add all the terms
    Vector3D ans=firstTerm+secondTerm+thirdTerm;
    return ans;
}


Matrix generateViewMatrix(Vector3D eye,Vector3D look,Vector3D up)
{
    //generate the view matrix
    Vector3D l = look - eye;
    l.normalize();
    Vector3D r = l.cross(up);
    r.normalize();
    Vector3D u = r.cross(l);
    u.normalize();
    
    Matrix viewMatrix = Matrix::identity();
    viewMatrix.set(0,0,r.x);
    viewMatrix.set(0,1,r.y);
    viewMatrix.set(0,2,r.z);

    viewMatrix.set(1,0,u.x);
    viewMatrix.set(1,1,u.y);
    viewMatrix.set(1,2,u.z);

    viewMatrix.set(2,0,-l.x);
    viewMatrix.set(2,1,-l.y);
    viewMatrix.set(2,2,-l.z);
    //rotation matrix done
    //translation matrix
    Matrix translationMatrix = Matrix::identity();
    translationMatrix.set(0,3,-eye.x);
    translationMatrix.set(1,3,-eye.y);
    translationMatrix.set(2,3,-eye.z);

    Matrix ans=viewMatrix*translationMatrix;
    return ans;

}
Matrix generateProjectionMatrix(double fovY, double aspectRatio, double near, double far) {
        Matrix P;

        double fovx=fovY*aspectRatio;
        double t=near*tan((fovY/2)*(M_PI/180));
        double r=near * tan((fovx/2)*(M_PI/180));


        // Set the diagonal elements
         // Scale the x coordinates
        P.set(0,0,near/r);
        // Scale the y coordinates
        P.set(1,1,near/t);
        P.set(2,2,-(far + near) / (far - near)); // Scale and translate z coordinates
        // Scale and translate z coordinates
        P.set(2,3,-(2 * far * near) / (far - near));

        // Set perspective divide
        P.set(3,2,-1);

        return P;
    }
Matrix handleTranslation(ifstream &file, Matrix modelMatrix)
{
    //read the parameters
    double tx,ty,tz;
    file>>tx>>ty>>tz;

    //create the translation matrix
    Matrix translationMatrix=Matrix::translate(tx,ty,tz);
    modelMatrix=modelMatrix*translationMatrix;
    return modelMatrix;

}
Matrix handleScaling(ifstream &file, Matrix modelMatrix)
{
    //read the parameters
    double sx,sy,sz;
    file>>sx>>sy>>sz;

    //create the scaling matrix
    Matrix scalingMatrix=Matrix::scale(sx,sy,sz);
    modelMatrix=modelMatrix*scalingMatrix;
    return modelMatrix;

}
Matrix handleRotation(ifstream &file, Matrix modelMatrix)
{
    //read the parameters
    double angle,ux,uy,uz;
    file>>angle>>ux>>uy>>uz;
    Vector3D axis(ux,uy,uz);
    axis.normalize();
    angle=angle*(M_PI/180.0);
    Vector3D X(1,0,0);
    Vector3D Y(0,1,0);
    Vector3D Z(0,0,1);
    Vector3D col1=rodriguesRotation(X,axis,angle);
    Vector3D col2=rodriguesRotation(Y,axis,angle);
    Vector3D col3=rodriguesRotation(Z,axis,angle);
    //create the rotation matrix
    Matrix rotationMatrix=Matrix::identity();
    rotationMatrix.set(0,0,col1.x);
    rotationMatrix.set(1,0,col1.y);
    rotationMatrix.set(2,0,col1.z);

    rotationMatrix.set(0,1,col2.x);
    rotationMatrix.set(1,1,col2.y);
    rotationMatrix.set(2,1,col2.z);

    rotationMatrix.set(0,2,col3.x);
    rotationMatrix.set(1,2,col3.y);
    rotationMatrix.set(2,2,col3.z);
    
    Matrix ans=modelMatrix*rotationMatrix;
    return ans;


    

}
void handleWrite(ostream &file,Matrix mat)
{
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            double normalized_val=mat.get(j,i)/mat.get(3,i);
            file<<WIDTH_PRECISION(10,7)<<normalized_val;
            
            // if(j!=2)
            file<<" ";
           
        }
        file<<endl;
    }
}
void graphicsPipeline(ifstream &file,Vector3D eye, Vector3D look, Vector3D up, double fovY, double aspectRatio, double near, double far)
{
    //open 3 files to write the output
   fstream file1,file2,file3;
    //create the files
    file1.open("output/stage1.txt",ios::out);
    file2.open("output/stage2.txt",ios::out);
    file3.open("output/stage3.txt",ios::out);


    
    //there is a stack of matrices
    stack<Matrix> stackOfMatrices;
    vector<Triangle> triangle_vector;

    //initialize the top of the stack with identity matrix
    Matrix identityMatrix=Matrix::identity();
    Matrix modelMatrix = Matrix::identity();
    Matrix viewMatrix=generateViewMatrix(eye,look,up);
    Matrix projectionMatrix=generateProjectionMatrix(fovY,aspectRatio,near,far);

    string str;
    int cnt=0;
  
    while(true)
    {
        //read the command
        file>>str;
        
        if(str=="translate")
        {
            modelMatrix=handleTranslation(file,modelMatrix);
            

        }
        else if (str=="scale")
        {
            modelMatrix=handleScaling(file,modelMatrix);
        }
        else if(str=="rotate")
        {
            modelMatrix=handleRotation(file,modelMatrix);

        }
        else if (str=="push")
        {
            stackOfMatrices.push(modelMatrix);
            
        }
        else if(str=="pop")
        {
            modelMatrix=stackOfMatrices.top();
            stackOfMatrices.pop();
        }
        else if(str=="triangle")
        {
            Matrix ans;
            //fill it with 1
            for(int i=0;i<4;i++)
            {
                for(int j=0;j<4;j++)
                {
                    ans.set(i,j,1);
                }
            }
            //read 3 vertices
            double x,y,z;
            file>>x>>y>>z;
            ans.set(0,0,x);
            ans.set(1,0,y);
            ans.set(2,0,z);
            file>>x>>y>>z;
            ans.set(0,1,x);
            ans.set(1,1,y);
            ans.set(2,1,z);
            file>>x>>y>>z;
            ans.set(0,2,x);
            ans.set(1,2,y);
            ans.set(2,2,z);
            //multiply the model view projection matrix
            ans=modelMatrix*ans;
            handleWrite(file1,ans);
            ans=viewMatrix*ans;
            handleWrite(file2,ans);
            ans=projectionMatrix*ans;
            handleWrite(file3,ans);

            Triangle triangle;
            //for v1
            Vector3D x1(ans.get(0,0)/ans.get(3,0),ans.get(1,0)/ans.get(3,0),ans.get(2,0)/ans.get(3,0));
            triangle.v1=x1;
            //for v2 and v3
            Vector3D x2(ans.get(0,1)/ans.get(3,1),ans.get(1,1)/ans.get(3,1),ans.get(2,1)/ans.get(3,1));
            triangle.v2=x2;
            Vector3D x3(ans.get(0,2)/ans.get(3,2),ans.get(1,2)/ans.get(3,2),ans.get(2,2)/ans.get(3,2));
            triangle.v3=x3;
            int r,g,b;
            generateRandomRGB(r,g,b);
            triangle.color.r=r;
            triangle.color.g=g;
            triangle.color.b=b;
            triangle_vector.push_back(triangle);


            cnt++;
            if(cnt==1)
            {
                //append endl in all files
                file1<<endl;
                file2<<endl;
                file3<<endl;
                cnt=0;

            }




        }
        else if(str=="end")
        {
            break;
        }



    }
    //close all the files
    file1.close();
    file2.close();
    file3.close();

    ifstream filex(congig_filename);
    int screenWidth,screenHeight;
    filex>>screenWidth>>screenHeight;
    //print 
    cout<<"screenWidth = "<<screenWidth<<" screenHeight = "<<screenHeight<<endl;
    zBuffer z(triangle_vector,screenWidth,screenHeight);
    z.performCalculations();
    z.save_zbuffer_to_file("output/z_buffer.txt");
    z.saveImage("output/out.bmp");




}




int main()
{
    scene_filename="3/scene.txt";
    congig_filename="3/config.txt";
    //create a folder called output
    string command="mkdir output";
    system(command.c_str());

 std::string filename = scene_filename;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return 1;
    }

    Vector3D eye, look, up;
    double fovY, aspectRatio, near, far;

    // Read parameters for gluLookAt
    file >> eye.x >> eye.y >> eye.z;
    file >> look.x >> look.y >> look.z;
    file >> up.x >> up.y >> up.z;

    // Read parameters for gluPerspective
    file >>fovY >> aspectRatio >> near >> far;

   

    // Print the read values (for verification)
    std::cout << "Eye: (" << eye.x << ", " << eye.y << ", " << eye.z << ")\n";
    std::cout << "Look: (" << look.x << ", " << look.y << ", " << look.z << ")\n";
    std::cout << "Up: (" << up.x << ", " << up.y << ", " << up.z << ")\n";
    std::cout << "Perspective Parameters: fovY = " << fovY
              << ", aspectRatio = " << aspectRatio
              << ", near = " << near
              << ", far = " << far << std::endl;
    //send everything to graphics pipeline
    graphicsPipeline(file,eye,look,up,fovY,aspectRatio,near,far);
   



   
}
