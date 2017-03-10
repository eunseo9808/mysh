#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc,char **argv)
{
	char *value; //현재 path를 가리킬 포인터
	char *pa = "PATH";
	if(argc==2 && !strcmp(argv[1],"--help")){
                printf("set PATH–n [경로목록] –기존 PATH를새로운경로목록으로덮어씀\n");
                printf("set PATH–a [경로목록] –기존 PATH에새로운경로목록을추가함\n");
                return 0;
        }
	value = getenv("PATH");
	if(strcmp(argv[1],"PATH-n")==0) //옵션체크
	{
		setenv("PATH",argv[2],1); // setenv함수로 PATH경로에 받아온 인자를 넣어줌
	}
	
	if(strcmp(argv[1],"PATH-a")==0) //옵션체크
	{
		strcat(value,argv[2]); //현재 경로에 받아온 인자를 추가
		setenv("PATH",value,1); // setenv 함수로 PATH경로에 받아온 인자를 넣어줌
	}
	return 0;
}
