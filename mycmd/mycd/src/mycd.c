#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define STR_LEN 1024

int main(int argc, char *argv[]){
	char curpath[STR_LEN]; //현재 주소를 받아오는 문자열
	int dest = 1;
	if(argc==2 && !strcmp(argv[1],"--help")){
                printf("mycd [FILENAME]\n");
                printf("	FILENAME에해당하는경로로이동\n");
		printf("	생략되거나 ~가입력될경우홈디렉토리로이동\n");
                return 0;
        }
	if( argc == 1 ) //인자를 입력하지 않고 그냥 mycd만 입력했을 때
		chdir(getenv("HOME")); //홈으로 이동
	getcwd(curpath, STR_LEN); //현재 위치 받아옴
	setenv("OLDPWD",curpath,1); //환경변수에 주소를 넣어줌

	chdir(argv[dest]); //받아온 인자주소로 디텍토리 이동
	getcwd(curpath,	STR_LEN); 
	setenv("PWD",curpath,1); //환경변수 변경
	return 0;

}
