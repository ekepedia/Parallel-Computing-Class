#include <stdio.h>
#include <math.h>
int main(){

	FILE* file;
	file = fopen("huff.txt","r");
	char  ch;
	char ch1;
    int count1 = 0;
    int count2 = 0;
	int n = 28;
	int x = 0;
    double shannon = 0;
	char arr[10000];
    int let[256];
    
    for(int i = 0 ; i < sizeof(arr) ; i++) arr[i] = '~';
    for(int i = 0 ; i < sizeof(let)/sizeof(int); i++) let[i] = 0;
  
	while(n > 0){
		fread(&ch,1,1,file);
	//	printf("%c  \n",ch);
		if( x == 0 ){
			x = 1;
			ch1 = ch;
			continue;

		}
		if( ch == '0' ){
			x = x *2;
			continue;
		}
		if( ch == '1' ){
			x =( x *2 ) +1;
			continue;
		}
		if ( ch == '\n' ){
			printf("%c    x: %d\n ",ch1,x);
		 	
			arr[x] = ch1;
            x = 0;
			--n;
			continue;
		}
		
	}
	
	
    
    x = 1;
	n = 1;
	while ( x > 0 ){
		
       	x = fread(&ch,1,1,file);
        count1++;
       // printf("%c\n",ch);
		if( ch == '0' ){
			n = n *2;
		//	continue;ls
      
			}
		if( ch == '1' ){
			n = (n * 2) + 1;
		//	continue;
			}
       //printf("x: %d     n: %d     ARR : %c     ch: %c \n",x,n,arr[n],ch);
		if(  arr[n] != '~' ){
			printf("%c",arr[n]);
            let[(int) arr[n] ] += 1;
            count2+=8;
			n = 1;
		}
	}
	
	for(int i = 0 ; i < 256 ; i++) if( let[i] != 0 ){
        printf( "\n %c : %d  \tprob : %4.2f%%", (char) i , let[i] ,( (double) let[i] / ((double) count2/8) )*100 );
        shannon += let[i] * (-1) * (log((double) let[i] / ((double) count2/8)) / log(2) );
    }
	
	double ratio = (((double) count2- (double)  count1)/ (double) count2);
	printf("\n\nEncoded bits: %d\nDecoded bit: %d\nRatio: %f\nshannon: %f\n\n",count1,count2, ratio,shannon);
	return 0 ;

}
