#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

#ifndef _MAX_PATH
#define _MAX_PATH 260
#endif
int main(int argc, char *argv[] ){
	int i;
	char *ptr,*pstrBuffer,strBuffer[_MAX_PATH],strBuf[100]; //문자열 보관 포인터, 문자열
	 if(!strcmp(argv[1],"--help")){ //--help 문 사용
                printf("mymkdir [DIRECTORY]\n");
		printf("	DIRECTORY에해당하는디렉토리생성\n");
		printf("mymkdir–p [PATH]\n");
		printf("	PATH에해당하는디렉토리모두생성\n");
                return 0;
        }
	if( argc == 1 ){ // 인자가 안들어왔을 때
			fputs("help: mymkdir [argument] \n", stdout );
			return -1;
	}
	if(argc ==2){ // 인자가 하나만들어왔을 때
		for( i= 1; i< argc; ++i){
			if( mkdir( argv[i], 0755) ){ //폴더생성 실패시 1반환
				fprintf(stdout, "mymkdir: 디렉터리 '%s'를 만들 수 없습니다. \n" ,argv[i]);
			}
			
		}
	}
	if(argc == 3) // 인자가 2개 들어왔을경우
	{
		strcpy(strBuf,argv[2]); //들어온 주소 복사해둠
		ptr = strtok(argv[2],"/"); // 문자열을 쪼개서 앞에 디렉토리를 가져옴
		pstrBuffer = getcwd(strBuffer,_MAX_PATH); //현재 위치를 받아옴
		if(mkdir(ptr,0755)) //1차 폴더생성 기존폴더가 없을 경우
		{
			mkdir(strBuf,0700); //2차 폴더생성
			strcat(pstrBuffer,"/"); 
			strcat(pstrBuffer,strBuf); //문자열 합침
			printf("생성경로 : %s\n",pstrBuffer);//생선된  위치 출력
		}
		if(!mkdir(ptr,0755)) //기존폴더가 있을 경우
		{
			mkdir(strBuf,0700);//2차 폴더생성
			strcat(pstrBuffer,"/"); 
			strcat(pstrBuffer,argv[2]); //문자열합침
			printf("생성경로 : %s\n",pstrBuffer); //생성된 위치 출력
		}
	}
	return 0;
}
