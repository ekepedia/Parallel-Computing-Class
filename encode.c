//Eke Wokocha


#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct TreeNode{
    char chara;
    int freq;
    struct TreeNode* left;
    struct TreeNode* right;
}Node;

char paths[256][23];
int * makeZeroArray(int* b , int h){
    while( h > 0 ){
        b[h-1] = 0;
            --h;
        }   
        return b;
}
int countActiveSlots(int* arr, int size){
    int x = 0;
    for( int i = 0 ; i < size ; i++ ){
            if( arr[i] > 0 )
                x++;
     }
     return x;
        
}
void printArray(int* arr, int size){
     printf("\n\n Array : \n\n");
     for( int i = 0 ; i < size ; i++ ){
            if( arr[i] > 0 )
                printf("[%c] = %d \t",(char) i,arr[i]);
     }
     printf("\n\n");
}
void setFeq(int* arr, char* word){
        for( int i = 0 ; i < strlen(word) ; i++)
            arr[(int) word[i]]++;
        
}

void swap(int a, int b, Node** arr){
    
        Node* c = arr[b];
        arr[b] = arr[a];
        arr[a] = c;
        
}

int fmini(Node** arr, int size){
    if( size < 2 )
        return 0;
    int min = 0;
    for ( int i = 0; i < size ; i++){
        if( arr[i]->freq < arr[min]->freq )
            min = i;
  
    }
    return min;
}

int fminiskip(Node** arr, int size, int skip){
    if( size < 2 )
        return 0;
    int min = 0;
    if( skip == 0) 
        min = 1;
    for ( int i = 0; i < size ; i++){
        if( i == skip ) 
            continue;
        if( arr[i]->freq < arr[min]->freq )
            min = i;
  
    }
    return min;
}

void printArr(Node** nodes, int size){
    printf("\n\n[ ");
    for(int i =0 ; i < size ; i++){
        char v = '-';
        char b = '-';
        if( nodes[i]->right != NULL)
            b = nodes[i]->right->chara;
        if( nodes[i]->left != NULL)
            v = nodes[i]->left->chara;
        printf("%d%c l:%c  r:%c, ",nodes[i]->freq,nodes[i]->chara,v,b);
    }
    printf("]");
}


void condense(Node** nodes, int size, char place ){
    for(size ; size > 1 ; size--){

        int mini = fmini(nodes, size);
        int mini2 = fminiskip(nodes,size,mini);
        int freqq = nodes[mini]->freq + nodes[mini2]->freq;
        
        swap(mini, size -1 , nodes); 
        swap(mini2, size-2, nodes);

        printArr(nodes,size);
        Node* temp = malloc(sizeof(Node));
        temp->freq = nodes[size-2]->freq;
        temp->chara = nodes[size-2]->chara;
		temp->left = nodes[size-2]->left;
		temp->right = nodes[size-2]->right;
        nodes[size-2] = malloc(sizeof(Node));
        
        nodes[size-2]->chara = '*';
        nodes[size-2]->freq = freqq;
        nodes[size-2]->left = nodes[size-1];
        nodes[size-2]->right = temp;
        printArr(nodes,size-1);
    }
    
}


void walkTree(Node* node, char* path, char place){
    if(node != NULL){
        if(node->chara != place){
            printf("\n%c : %s \n", node->chara,path);
            char final[23];
            strcpy(path,final);
            paths[(int) node->chara] = final;
        }
        
        else{
			char path1[23] = "";
			strcpy(path1,path);
			strcat(path,"0");
			strcat(path1,"1");
            walkTree(node->left, path, place);
            
            walkTree(node->right, path1, place);
        }
    }
}


void main(){
   
//////////////////
    int fsize = 256 ;
    char* code = "Hello World";
    int charaNum = 0; 
    int frequency[fsize];
//////////////////
   
    makeZeroArray(frequency ,fsize );
    setFeq(frequency,code);
    charaNum = countActiveSlots(frequency,fsize);
    printArray(frequency,fsize);
    Node* nodes[charaNum];
   
    
    //////////
    for ( int i = 0, k = 0 ; i < charaNum & k < fsize ; k++){
        if( frequency[k] > 0 ){
             nodes[i] = malloc(sizeof(Node));
             nodes[i]->chara = (char) k;
             nodes[i]->freq = frequency[k];
             nodes[i]->left = NULL;
             nodes[i]->right = NULL;
             i++;
        }
  
    }
    ////////
    condense(nodes,charaNum,'*');
    char path[23] = "";
    
    walkTree(nodes[0], path, '*');
	
   
}
