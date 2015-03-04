 
/*
 * forestfire.c
 *
 * Copyright 2014 Eke Wokocha <2016ewokocha@oscar>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 *
 */
 
 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
 
#define maxnum 30

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
    q[0] = 0;
    q[1] = 1;
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

char field[maxnum][maxnum];
 
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
    if(q)
        que = queue;
    else
        que = queue2;
    field[i][j] = '-';
    int u = i+1 , d = i-1, l = j-1, r = j + 1;
    if( valid(u,j) )
        Qadd(que,u,j);
    if( valid(d,j) )
        Qadd(que,d,j);
    if( valid(i,l) )
        Qadd(que,i,l);
    if( valid(i,r) )
        Qadd(que,i,r);
}  
 
void setFire(){
    for(int i = 0 ; i < maxnum ; i++ ){
        if(field[i][0] != '-'){
        field[i][0] = '*';
        Qadd(queue,i,0);
        }
    }
}
 
int fire(){
    int food = 1;
    int step=-2;
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
    srand(time( NULL ));
    fillArr(prb);
    
    int sum = 0;
    for(int i = 1000; i > -1 ; i--){
        fillArr(prb);
        sum += fire();
    }
    double ave = (sum/1000.0)/(maxnum+0.0);
    printf("%f\t%f\n",prb,ave);
    

}

void main(){
   call(0.5,4); 
   }
