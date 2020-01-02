//
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <sstream>
using namespace std;
int main(){
//1.atoi
char str[]="100";
int num=atoi(str);
printf("num=%d\n",num);

//sprintf/sscanf
int num2;
sscanf(str,"%d",&num2);
printf("num2=%d\n",num2);
char str2[10];
sprintf(str2,"%d",num2);
printf("str2=%s\n",str2);


}
