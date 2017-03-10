#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]){
        char *buf; //문자열 가르킬 포인터
	if(argc==2 && !strcmp(argv[1],"--help")){
                printf("현재위치의절대경로출력\n");
                return 0;
        }        buf= getenv("PWD"); // 현재 위치받아옴
        fputs(buf,stdout); // 출력
        fputs("\n",stdout); // 엔터 출력

        return 0;
}

