#include <stdio.h>
#include <string.h>
int main(){
    char str1[1000];
    char newString[10][10]; 
    int i,j,ctr;
    FILE *f;
    char fAddr[1000];
    printf(" Input  a string : ");
    fgets(fAddr, sizeof fAddr, stdin);	

    f = fopen(fAddr,"w+");

 
    j=0; ctr=0;
    for(i=0;i<=(f(fAddr));i++){
        // if space or NULL found, assign NULL into newString[ctr]
        if(f[i]==' '||f[i]=='\0'){
            newString[ctr][j]='\0';
            ctr++;  //for next word
            j=0;    //for next word, init index to 0
        }
        else{
            newString[ctr][j]=str1[i];
            j++;
        }
    }
    printf(" %s\n",newString[0]);
    return 0;
}


