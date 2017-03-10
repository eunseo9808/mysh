#include <stdlib.h>
#include <stdio.h>
#include <string.h>
extern char **environ;
int main(int argc,char **argv)
{
	char *value; // 현재 PATH를 가리킬 포인터
	char **env = environ; // 전체 환경변수를 출력할 포인터
	 if(argc==2 && !strcmp(argv[1],"--help")){
                printf("myenv\n");
                printf("	환경변수전체출력\n");
		printf("myenv [STRING]\n");
		printf("	STRING에해당하는환경변수출력\n");
                return 0;
        }
	if(argc == 2){	
		value = getenv(argv[1]); // 현재 환경변수를 받아옴	
		if(value !=NULL) // NULL인지 체크
		{
			printf("%s = %s\n",argv[1],value);
		}
	}
	else if(argc==1){ //인자가 없을 경우 환경변수 모두 출력
		while(*env){
			printf("%s\n",*env); // env가 증가하면서 환경변수 출력
			env++;
		}
	}	
	return 0;
}

