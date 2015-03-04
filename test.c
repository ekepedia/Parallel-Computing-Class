
#include<stdio.h>

int setstuff( int h){
    
   h = 9;
   return h;     
}

void main(){
    
    int h = 8;
    h = setstuff(h);
    printf("%d\n",h);
    
}