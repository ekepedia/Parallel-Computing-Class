#include <stdio.h>
#include <math.h>
//
#define WIDTH 640
#define HIGHT 480
//
double ex=0.500000, ey=0.500000, ez=-1.000000;
//


struct position{
    double x;
    double y;
    double z;
};

struct color{
    int red;
    int green;
    int blue;    
};

struct sphere{
    struct position position;
    struct color   color;
    double radius;
};
typedef struct sphere Sphere;
typedef struct position Vector;

double power(double x , double y, int inverse);
double solutionSphere( Vector ray, Sphere sphere);
Vector findRay( int x, int y);
double findT( Vector ray, Sphere sphere );
Vector findNormal( Vector ray, Sphere sphere);
Vector lightVector( Vector ray);
double cosine(Vector ray, Vector ray2);

int main(void)
{
   int rgb[HIGHT][WIDTH][3] ; // red-green-blue for each pixel
   //
   int y , x ;
   //
   FILE* fout ;
   
   
   Sphere sphere1 = { { 0.5 , 0.5 , 0.166667 }, { 0 , 0 , 255 } ,  0.166667  };
   Sphere sphere2 = { { 0.833333 , 0.5 , 0.5 }, { 0 , 255 , 0 } ,  0.166667  };
   Sphere sphere3 = { { 0.333333 , 0.666667 , 0.666667 }, { 255 , 0 , 0 } ,  0.333333  };
   Sphere spheres[] = {sphere1, sphere2, sphere3};

   for( y = 0 ; y < HIGHT ; y++ )
   {
      for( x = 0 ; x < WIDTH ; x++)
      {
         //
         Vector ray = findRay(x,y);
         double a, b ,c , cx ,cy, cz, r,tfinal = 1000000,sol,t;
         int red = 0 ,green = 0,blue = 0, object = -1;
         for( int q = 0; q < 3 ; q++){
            sol = solutionSphere(ray , spheres[q]);
            //
            if( sol >= 0 ){
                //
                t = findT(ray , spheres[q]);
                if ( t < tfinal ){
                    red = spheres[q].color.red;
                    green = spheres[q].color.green;
                    blue = spheres[q].color.blue;
                    tfinal = t;
                    object = q;
                }
            }
         }
         double dot = 0;
         if( object + 1 ){
             Vector tempray = {(ex + tfinal*ray.x), (ey + tfinal*ray.y), (ez + tfinal*ray.z)};
             Vector posOnSphere = tempray;
             Vector lightray = lightVector(posOnSphere);
            Vector normal = findNormal(posOnSphere,spheres[object]);
             dot = cosine(lightray,normal);
         }
         else if(object & ray.y < 0){
            
            red = 25;
            green = 47;
            blue = 153;
            
         }
         
         //
            rgb[y][x][0] = red/3 + red/3*2*dot;
            rgb[y][x][1] = green/3 + green/3*2*dot;
            rgb[y][x][2] = blue/3 + blue/3*2*dot;
         //                
      }
   }
   //
   //
   //
   fout = fopen( "image.ppm" , "w" ) ;
   //
   fprintf( fout , "P3\n" ) ;
   fprintf( fout , "%d %d\n" , WIDTH , HIGHT ) ;
   fprintf( fout , "255\n" ) ;
   //
   for( y = 0 ; y < HIGHT ; y++ )
   {
      for( x = 0 ; x < WIDTH ; x++)
      {
         fprintf( fout , "%d %d %d\n" ,
          rgb[y][x][0] , rgb[y][x][1] , rgb[y][x][2] ) ;
      }
   }
   fclose( fout ) ;
   //
   return 0 ;
}

double power(double x , double y, int inverse){
     if( y == 0)
         return 1;
     x *= power(x,y-1,0);
     if (inverse)
         x = 1/x;
     return x;
}    
Vector findRay( int x, int y){
        double px = 4.88281249635*power(10,13,1)*x*x + 0.00208333296875*x;
        double py = 0.00200333333333*(480-y);
        double pz = 0;
        //
        double rx = px - ex;
        double ry = py - ey;
        double rz = pz - ez;
        //
        double rm = sqrt(rx*rx + ry*ry + rz*rz);
        //
        rx /= rm;
        ry /= rm;
        rz /= rm;
        Vector tempray = {rx , ry , rz };
        return tempray;
}
double solutionSphere( Vector ray, Sphere sphere){
    double rx = ray.x;
    double ry = ray.y;
    double rz = ray.z;
    double cx = sphere.position.x;
    double cy = sphere.position.y;
    double cz = sphere.position.z;
    double r = sphere.radius;
    double a = rx*rx + ry*ry + rz*rz;
    double b = 2*(ex*rx - cx*rx + ey*ry - cy*ry + ez*rz - cz*rz);
    double c = ex*ex - 2*ex*cx + cx*cx + ey*ey - 2*ey*cy + cy*cy + ez*ez - 2*ez*cz + cz*cz - r*r;
    double sol = b*b-4*a*c;
    return sol;
}
double findT( Vector ray, Sphere sphere ){
    double rx = ray.x;
    double ry = ray.y;
    double rz = ray.z;
    double cx = sphere.position.x;
    double cy = sphere.position.y;
    double cz = sphere.position.z;
    double r = sphere.radius;
    double a = rx*rx + ry*ry + rz*rz;
    double b = 2*(ex*rx - cx*rx + ey*ry - cy*ry + ez*rz - cz*rz);
    double c = ex*ex - 2*ex*cx + cx*cx + ey*ey - 2*ey*cy + cy*cy + ez*ez - 2*ez*cz + cz*cz - r*r;
    double sol = b*b-4*a*c;
    return ((-sqrt(sol) - b)/(2*a));
}
Vector findNormal( Vector ray, Sphere sphere){
    double cx = sphere.position.x;
    double cy = sphere.position.y;
    double cz = sphere.position.z;
    
    double nx = (ray.x - cx);
    double ny = (ray.y - cy);
    double nz = (ray.z - cz);
    
    double rm = sqrt(nx*nx + ny*ny + nz*nz);
    nx /= rm;
    ny /= rm;
    nz /= rm;
    Vector tempray = {nx,ny,nz};
    return tempray;
}
Vector lightVector( Vector ray){
    double lx=0.500000, ly=-0.500000, lz=0.500000;
    double rx = ray.x - lx;
    double ry = ray.y - ly;
    double rz = ray.z - lz;
    //
    double rm = sqrt(rx*rx + ry*ry + rz*rz);
    //
    rx /= rm;
    ry /= rm;
    rz /= rm;
    Vector tempray = {rx,ry,rz};
    return tempray;
}
double cosine(Vector ray, Vector ray2){
    double dot = ray.x*ray2.x + ray.y*ray2.y + ray.z*ray2.z;
    if (dot < 0)
        dot = 0;
    if (dot > 1)
        dot = 1;
    return dot;
}
//
// end of file
//
