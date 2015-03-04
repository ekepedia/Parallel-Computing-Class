 
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
double prb = 0.5;
char field[maxnum][maxnum];
int step = -2;
 
double myrand()
{
    return  (rand()%100)/100.0 ;
}
 
void print(){
    for( int i = 0 ; i < maxnum ; i++ ){
        for( int j = 0 ; j < maxnum ; j++ ){
            printf("%c ",field[i][j]);
        }
        printf("\n");
    }
    printf("%d\n",step);
}
 
void fillArr(){
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
    field[i][j] = '-';
    int u = i+1 , d = i-1, l = j-1, r = j + 1;
     
    if( valid(u,j) )
        field[u][j] = '+';
    if( valid(d,j) )
        field[d][j] = '+';
    if( valid(i,l) )
        field[i][l] = '+';
    if( valid(i,r) )
        field[i][r] = '+';
}  
 
void setFire(){
    for(int i = 0 ; i < maxnum ; i++ ){
        if(field[i][0] != '-')
        field[i][0] = '*';
    }
}
 
void confirmFire(){
    for( int i = 0 ; i < maxnum ; i++ ){
        for( int j = 0 ; j < maxnum ; j++ ){
            if( field[i][j] == '+' )
                field[i][j] = '*';
        }
    }
}
 
int fire(){
    int food = 1;
    step=-2;
    setFire();
    step++;
    while(food){
        step++;
        food = 0;
        for( int i = 0 ; i < maxnum ; i++ ){
            for( int j = 0 ; j < maxnum ; j++ ){
                if( field[i][j] == '*' ){
                    food = 1;
                    catchFire(i,j);
                }          
            }
        }
        confirmFire();
    }
    return step;
}
 
void main()
{
    FILE* out = fopen("out.txt","w");
    srand(time( NULL ));
    fillArr();
    double max = 0;
    double mac = 0;
    for(prb = 0; prb <= 1; prb += 0.001){
        int sum = 0;
        for(int i = 1000; i > -1 ; i--){
            fillArr();
            sum += fire();
        }
        double ave = (sum/1000.0)/(maxnum+0.0);
        fprintf(out,"%f\t%f\n",prb,ave);
        if(ave > max){
                max = ave;
                mac = prb;
        }
    }
    printf("%f \t %f",max,mac);
    fclose(out);
}
