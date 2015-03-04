#include <stdio.h>
#include <stdlib.h>

int main(){
    FILE* file = fopen("huff.txt","r");
    char* buf;
    int array[258456];
    fgets(buf, 34, file);
    while ( fgets(buf, 20000, file) ){
        char ch = (char)buf[0];
        int x;
        for( x = 0 ; x < 10 ; x ++ )
            printf("%s \n",buf);
      
    }
    return 0;
}
