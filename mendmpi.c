// 
// Torbert, 26 November 2013
// 
#include <stdio.h>
#include <GL/glut.h>
#include <time.h>
#include "mpi.h"
#include <stdlib.h>
#include <unistd.h>
// 
#define M 800 
#define N 600 
#define maxIt 100
// 
int count =  0 ; 
int ascii = 48 ; 
double globalscale = 1; 
int globalmax = 100;
double xpos = 0;
double ypos = 0;
int xxpos = 0;
int yypos = 0;
int        rank   ;
int        size   ;
MPI_Status status ;
int        tag    ;
int        k , j  ;
double     xpp;
int*       values;

int* worker(int xpp, int max);

int madle(int xpp,int ypp, int maxt, double scale, double xpos, double ypos){
        double xp = (2.0/scale)/(400.0)*(xpp )-((2/scale) - xpos) ;
        double yp = (1.5/scale)/(300.0)*(ypp)-((1.5/scale) - ypos) ;
        double zx = 0;
        double zy = 0;
        int x = maxt;
        while( x ){
            double oldzx = zx;
            double oldzy = zy;
            zx = (oldzx*oldzx)-(oldzy*oldzy) + xp;
            zy = 2*oldzx*oldzy + yp;
            if(((zx*zx)+(zy*zy)) > 4)
            {
                return maxt - x;
            }
            x--;
        }
        return maxt;
}

// 
void idlefunc(void)
{
    ++count ;
    //
    if( count == 1000000 )
    {
        count = 0 ;
        //
        ++ascii ;
        //
        if( ascii ==  58 ) ascii = 65 ;
        if( ascii ==  91 ) ascii = 97 ;
        if( ascii == 123 ) ascii = 48 ;
        //
        glutPostRedisplay() ;
    }
}
//
void displayfunc(void)
{
    int xp , xpp , ypp , q ;
    //
    glClear(GL_COLOR_BUFFER_BIT);
    for( j = 1 ; j < size ; j++ )
      {
         int p = j - 1;
         MPI_Send( &p, 1 , MPI_INTEGER , j , tag , MPI_COMM_WORLD ) ;
         MPI_Send( &globalmax, 1 , MPI_INTEGER , j , tag , MPI_COMM_WORLD ) ;
         MPI_Send( &globalscale, 1 , MPI_DOUBLE , j , tag , MPI_COMM_WORLD ) ;
         MPI_Send( &xpos, 1 , MPI_DOUBLE , j , tag , MPI_COMM_WORLD ) ;
         MPI_Send( &ypos, 1 , MPI_DOUBLE , j , tag , MPI_COMM_WORLD ) ;
      }
    for( xp = size-2 ; xp < M ; xp++ )
    {
        int values[N];
        int p = N;
        while(p > 0 ){
                values[p-1] = 0;
                p--;
        }
        MPI_Recv( &xpp , 1 , MPI_INTEGER , MPI_ANY_SOURCE , tag , MPI_COMM_WORLD , &status ) ;
        j = status.MPI_SOURCE;
        MPI_Recv( values , N , MPI_INTEGER , j , tag , MPI_COMM_WORLD , &status ) ;
        for ( ypp = 0 ; ypp < 600 ; ypp++) {  
            int x = values[ypp];
            if (x == globalmax )
                glColor3f(1,1,1);
            else
                glColor3f(  0 +(x*.02), 0+ (x*.009) , 0 + (x*.0009) );
            glBegin(GL_POINTS);
            glVertex2f(xpp,ypp);
            glEnd();
        }
        MPI_Send( &xp, 1 , MPI_INTEGER , j , tag, MPI_COMM_WORLD ) ;
        MPI_Send( &globalmax, 1 , MPI_INTEGER , j , tag , MPI_COMM_WORLD ) ;
        MPI_Send( &globalscale, 1 , MPI_DOUBLE , j , tag , MPI_COMM_WORLD ) ;
        MPI_Send( &xpos, 1 , MPI_DOUBLE , j , tag , MPI_COMM_WORLD ) ;
        MPI_Send( &ypos, 1 , MPI_DOUBLE , j , tag , MPI_COMM_WORLD ) ;
    }
    for( q = 1 ; q < size ; q++ )
      {
        int values[N];
        int p = N;
        while(p > 0 ){
                values[p-1] = 0;
                p--;
        }
        MPI_Recv( &xpp , 1 , MPI_INTEGER , MPI_ANY_SOURCE , tag , MPI_COMM_WORLD , &status ) ;
        j = status.MPI_SOURCE;
        MPI_Recv( values , N , MPI_INTEGER , j , tag , MPI_COMM_WORLD , &status ) ;
        for ( ypp = 0 ; ypp < 600 ; ypp++) {  
            int x = values[ypp];
            glColor3f(  0 +(x*.01), 0+ (x*.001) , 0 + (x*.0001) );
            glBegin(GL_POINTS);
            glVertex2f(xpp,ypp);
            glEnd();
        }
      }
   
   glutSwapBuffers() ;
}
void mousefunc(int button,int state,int xscr,int yscr)
{
    if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
    {
            globalscale *= 1.99;
            printf("Zoom in 20. Scale : %f    %d\n",globalscale*100,xscr);
            xxpos =  400+xscr  ;
            yypos =  300-yscr  ;
            xpos += (2.0/globalscale)/(400.0)*(xscr)-((2/globalscale));
            ypos += (1.5/globalscale)/(300.0)*(yscr)-((1.5/globalscale));
            printf("\n%d %d %f %f\n",xxpos,yypos,xpos,ypos);
    }
    else if(button==GLUT_RIGHT_BUTTON && state==GLUT_DOWN)
    {
            globalscale /= 1.5;
            printf("Zoom out 20. Scale : %f\n",globalscale*100);
            xxpos =  400+xscr  ;
            yypos =  300-yscr  ;
            xpos += (2.0/globalscale)/(400.0)*(xscr)-((2/globalscale));
            ypos += (1.5/globalscale)/(300.0)*(yscr)-((1.5/globalscale));
            printf("\n%d %d %f %f\n",xxpos,yypos,xpos,ypos);
    }
}

void motionfunc(int xscr,int yscr)
{
    printf("Motion ( %d , %d ).\n" , xscr , yscr ) ;
}
void keyfunc(unsigned char key,int xscr,int yscr)
{
    printf("Key %c pressed at %d %d.\n" , key, xscr, yscr);
}
void specialfunc(int key,int xscr,int yscr)
{
    
    if( key == GLUT_KEY_UP )
    {
        globalmax  *= 2;
        printf("Max interations = %d\n",globalmax);
    }if( key == GLUT_KEY_DOWN )
    {
        globalmax  /= 2;
        if (globalmax == 0)
            globalmax = 1;
        printf("Max interations = %d\n",globalmax);
    }
}
void closefunc(void)
{
    printf("Window closed.\n");
}

int main( int argc , char* argv[] )
{
   //
   // MPI variables
   //
   
   //
   // other variables
   //
   //
   // boilerplate
   //
   MPI_Init(      &argc          , &argv ) ;
   MPI_Comm_size( MPI_COMM_WORLD , &size ) ; // same
   MPI_Comm_rank( MPI_COMM_WORLD , &rank ) ; // different
   //
   // manager has rank = 0
   //
   if( rank == 0 )
   {
        glutInit(&argc,argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
        glutInitWindowSize(M,N);
        glutInitWindowPosition(100,50);
        glutCreateWindow("");
        glClearColor(1.0,1.0,1.0,0.0);
        glShadeModel(GL_SMOOTH);
        //
        glViewport(0,0,(GLsizei)M,(GLsizei)N); // reshape
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0.0,1.0*M,1.0*N,0.0); // invert y-coords
        glMatrixMode(GL_MODELVIEW);
        //
        glutIdleFunc(idlefunc);
        glutDisplayFunc(displayfunc);
        glutReshapeFunc(NULL);
        glutMouseFunc(mousefunc);
        glutMotionFunc(motionfunc);
        glutKeyboardFunc(keyfunc);
        glutSpecialFunc(specialfunc);
        glutWMCloseFunc(closefunc);
        //
        glutMainLoop();
        //
        return 0;
   }
   //
   // workers have rank > 0
   //
   else
   {
      while(1){
            int xpp, maxt;
            double scale,xpos,ypos;
            MPI_Recv( &xpp , 1 , MPI_INTEGER , 0 , tag , MPI_COMM_WORLD , &status ) ;
            MPI_Recv( &maxt , 1 , MPI_INTEGER , 0 , tag , MPI_COMM_WORLD , &status ) ;
            MPI_Recv( &scale , 1 , MPI_DOUBLE , 0 , tag , MPI_COMM_WORLD , &status ) ;
            MPI_Recv( &xpos , 1 , MPI_DOUBLE , 0 , tag , MPI_COMM_WORLD , &status ) ;
            MPI_Recv( &ypos , 1 , MPI_DOUBLE , 0 , tag , MPI_COMM_WORLD , &status ) ;
            int boogie[N];
            int ypp;
            for(  ypp = 0 ; ypp < N ; ypp++ ){
                boogie[ypp] = madle(xpp,ypp, maxt ,scale, xpos, ypos);
            }
            int x;
            MPI_Send(&xpp , 1 , MPI_INTEGER , 0 , tag , MPI_COMM_WORLD ) ;
            MPI_Send(boogie , N , MPI_INTEGER , 0 , tag , MPI_COMM_WORLD ) ;
            
      }
   }
   //
   // boilerplate
   //
   //
   return 0;
}
// 
// end of file
// 

