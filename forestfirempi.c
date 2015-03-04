// 
// Torbert, 27 October 2014
// 
// MPI Demo
//    mpicc mpiDemo.c
//    mpirun -np 4 a.out
// 
// Manager-Worker model for parallel processing.
// 
// time ... real ... user 
// 
// htop 
// 
#include <stdio.h>
#include "mpi.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
 
#define maxnum 30
char field[maxnum][maxnum];
int step = -2;
int queue[maxnum*maxnum*2+2];
int queue2[maxnum*maxnum*2+2];
int q = 0;

void Qadd(int* q, int x, int y){
    q[q[0]] = x;
    q[q[1]] = y;
    q[0] = q[0] + 2;
    q[1] = q[1] + 2;
}
void Qclear(int* q){
    int c = q[1];
    while( c >= 0 ){
      q[c] = -1;
        c--;
    }
    q[0] = 2;
    q[1] = 3;
}
void Qmake(int* q){
    int c = maxnum*maxnum*2-1;
    while( c >= 0 ){
      q[c] = -1;
        c--;
    }
    q[0] = 2;
    q[1] = 3;
}
void Qprint(int* q){
    int c = 2;
    while( c < q[0] ){
      printf("[%d,%d]", q[c] , q[c+1]);
        c+=2;
    }
    printf("\n");
}

void print(){
    for( int i = 0 ; i < maxnum ; i++ ){
        for( int j = 0 ; j < maxnum ; j++ ){
            printf("%c ",field[i][j]);
        }
        printf("\n");
    }

}
 
double myrand()
{
    return  (rand()%100)/100.0 ;
}
 

void fillArr(double prb){
    for( int i = 0 ; i < maxnum ; i++ ){
        for( int j = 0 ; j < maxnum ; j++ ){
            if( myrand() <= prb )
                field[i][j] = 'x';
            else
                field[i][j] = '-';
        }
    }
}
 
int valid(int i, int j ){
    if( i >= 0 && i < maxnum ){
        if( j >= 0 && j < maxnum )
            if( field[i][j] != '-' && field[i][j] != '*')
                return 1;
    }
    return 0;
}
 
void catchFire( int i , int j){
    int* que;
    if(!q)
        que = queue;
    else
        que = queue2;
    field[i][j] = '-';
    int u = i+1 , d = i-1, l = j-1, r = j + 1;
    if( valid(u,j) ){
        Qadd(que,u,j);
        field[u][j] = '*';
    }
    if( valid(d,j) ){
        Qadd(que,d,j);
        field[d][j] = '*';
    }
    if( valid(i,l) ){
        Qadd(que,i,l);
        field[i][l] = '*';
    }
    if( valid(i,r) ){
        Qadd(que,i,r);
        field[i][r] = '*';
    }
}   
 
void setFire(){
    for(int i = 0 ; i < maxnum ; i++ ){
        if(field[i][0] != '-'){
            field[i][0] = '*';
            Qadd(queue,i,0);
        }
    }
}
 
int fire(double prb){
    Qmake(queue);
    Qmake(queue2);
    int food = 1;
     step=-2;
    setFire();
    step++;
    while(food){
        step++;
        food = 0;
        int* que;
        if(!q){
            que = queue;
            q = 1;
        }
        else{
            que = queue2;
            q = 0;
        }  
        for( int i = 2 ; i < que[0] ; i+=2 ){
                if( field[que[i]][que[i+1]] == '*' ){
                    food = 1;
                    catchFire(que[i],que[i+1]);
                }          
        }
        Qclear(que);
    }
    
    return step;
}


 
double call(double prb, int rank)
{
    srand(time( NULL ) + rank);
    fillArr(prb);
    int sum = 0;
    for(int i = 1000; i > -1 ; i--){
        fillArr(prb);
        sum += fire(prb);
    }
    double ave = (sum/1000.0)/(maxnum+0.0);
    //printf("%f\t%f\n",prb,ave);
    return ave;

}

// 
int main( int argc , char* argv[] )
{
   //
   // MPI variables
   //
   int        rank   ;
   int        size   ;
   MPI_Status status ;
   int        tag    ;
   //
   // other variables
   //
   int        k , j  ;
   double     prb, result;
   FILE* out = fopen("out.txt","w");
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
      printf( "\n" ) ;
      //
      prb = 1 ; 
      //
      for( j = 1 ; j < size ; j++ )
      {
         MPI_Send( &prb , 1 , MPI_DOUBLE , j , tag , MPI_COMM_WORLD ) ;
         prb -= 0.001;
      }
      //
      for( prb ; prb >0 ; prb -= 0.001)
      {
         MPI_Recv( &result , 1 , MPI_DOUBLE , MPI_ANY_SOURCE , tag , MPI_COMM_WORLD , &status ) ;
         //
         j = status.MPI_SOURCE ;
         //
         fprintf(out,"%f %f\n", prb, result ) ;
         //
         MPI_Send( &prb , 1 , MPI_DOUBLE , j , tag , MPI_COMM_WORLD ) ;
      }
      //
      for( int k = 1 ; k < size ; k++ )
      {
         MPI_Recv( &result , 1 , MPI_DOUBLE , MPI_ANY_SOURCE , tag , MPI_COMM_WORLD , &status ) ;
         //
         j = status.MPI_SOURCE ;
         //
        
         fprintf(out,"%f %f\n", prb, result ) ;
         double end = -1.0;
         MPI_Send( &end , 1 , MPI_DOUBLE , j , tag , MPI_COMM_WORLD ) ;
      }
      
      //
      printf( "\n" );
   }
   //
   // workers have rank > 0
   //
   else
   {
      while(1){
      MPI_Recv( &result , 1 , MPI_DOUBLE , 0 , tag , MPI_COMM_WORLD , &status ) ;
      //printf( "%f\n" , result ) ;
      //
      if ( result < 0 ){
       
          printf("%d done\n",rank);
          break;
         
      }
      result = call(result,rank) ;
      //
      MPI_Send( &result , 1 , MPI_DOUBLE , 0 , tag , MPI_COMM_WORLD ) ;
      }
   }
   //
   // boilerplate
   //
   fclose(out);
   MPI_Finalize() ;
   //
   return 0;
}
// 
// end of file
// 
