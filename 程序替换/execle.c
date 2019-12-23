#include <stdio.h>
#include <stdlib.h>

int main()
{
    printf("%s\n",getenv("AAA"));//无AAA环境变量，返回空指针，打印的时候会解引用，引发段吐核

    return 0;
}
