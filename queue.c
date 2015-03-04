



#include <stdio.h>
#include <stdlib.h>

#define MAX 32
int queue[MAX*MAX*2+2];
int queue2[MAX*MAX*2+2];

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
    int c = MAX*MAX*2-1;
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
int main(){
    Qmake(queue);
    Qmake(queue2);
    Qprint(queue);
    Qadd(queue,2,5);
    Qadd(queue,2,5);
    Qadd(queue2,2,5);
    Qprint(queue);
    Qclear(queue);
    Qprint(queue);
    Qprint(queue2);
}

