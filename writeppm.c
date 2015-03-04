#include <stdio.h>
#include <math.h>
//
#define WIDTH 640
#define HIGHT 480
//

double power(double x , double y, int inverse){
     if( y == 0)
         return 1;
     x *= power(x,y-1,0);
     if (inverse)
         x = 1/x;
     return x;
}
int main(void)
{
   int rgb[HIGHT][WIDTH][3] ; // red-green-blue for each pixel
   //
   int y , x ;
   double ex=0.500000, ey=0.500000, ez=-1.000000;
   //
   FILE* fout ;
   double spheres[3][7];
   spheres[0][0] = 0.500000;
   spheres[0][1] = 0.500000;
   spheres[0][2] = 0.166667;
   spheres[0][3] = 0.166667;
   spheres[0][4] = 0;
   spheres[0][5] = 0;
   spheres[0][6] = 255;
   //
   spheres[1][0] = 0.833333;
   spheres[1][1] = 0.500000;
   spheres[1][2] = 0.500000;
   spheres[1][3] = 0.166667;
   spheres[1][4] = 0;
   spheres[1][5] = 255;
   spheres[1][6] = 0;
   //
   spheres[2][0] = 0.333333;
   spheres[2][1] = 0.666667;
   spheres[2][2] = 0.666667;
   spheres[2][3] = 0.333333;
   spheres[2][4] = 255;
   spheres[2][5] = 0;
   spheres[2][6] = 0;
   //
   for( y = 0 ; y < HIGHT ; y++ )
   {
      for( x = 0 ; x < WIDTH ; x++)
      {
         double px = 4.88281249635*power(10,13,1)*x*x + 0.00208333296875*x;
         double py = 0.00208333333333*(480-y);
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
         //
         double a, b ,c , cx ,cy, cz, r,tfinal = 1000000,sol,t;
         int red = 0 ,green = 0,blue = 0;
         for( int q = 0; q < 3 ; q++){
            cx= spheres[q][0];
            cy= spheres[q][1];
            cz= spheres[q][2];
            r= spheres[q][3];
            //
            a = rx*rx + ry*ry + rz*rz;
            b = 2*(ex*rx - cx*rx + ey*ry - cy*ry + ez*rz - cz*rz);
            c = ex*ex - 2*ex*cx + cx*cx + ey*ey - 2*ey*cy + cy*cy + ez*ez - 2*ez*cz + cz*cz - r*r;
            sol = b*b-4*a*c;
            if(sol >= 0){
                t = (-sqrt(sol) - b)/(2*a);
                if ( t < tfinal ){
                    red = spheres[q][4];
                    green = spheres[q][5];
                    blue = spheres[q][6];
                    tfinal = t;
                }
            }
         }
         if(tfinal == 1000000 & ry < 0){
            red = 25*rm;
            green = 47*rm;
            blue = 153*rm;
            printf("ASD\n");
         }
         //
            rgb[y][x][0] = red  ;
            rgb[y][x][1] = green ;
            rgb[y][x][2] = blue ;
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
   close( fout ) ;
   //
   return 0 ;
}

    


//
// end of file
//
