#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(int argc, char*argv[]){  
                                                   
        DIR *dir; //디렉토리 포인터
        struct dirent *entry; 
        struct stat fileStat; //프로세스 상태구조체
        int pid,count=1; // PID
        char cmdLine[256]; // 문자열 공간
        char tempPath[256];
        FILE *srcFp; //파일입출력
        char status[100];
	if(argc==2 && !strcmp(argv[1],"--help")){
                printf("백그라운드로실행한프로세스목록출력\n");
                printf("	[실행된순서] [프로세스상태] [CMD]\n");
		printf("실행중백그라운드로들어간프로세스는출력하지않음\n");
		printf("실행이끝난뒤종료된프로세스출력하지않음\n");
		printf("실행순서번호는항상 1부터시작\n");
                return 0;
        }	
        dir = opendir("/proc"); // 프로세스가 들어있는 proc접근
        if(dir == NULL) // 디렉토리가 비어있는지 확인
                return false;
	printf("실행순서\t프로세스상태\tCMD\n");
        while ((entry = readdir(dir)) != NULL) {  //NULL이 아닌지 체크
                lstat(entry->d_name, &fileStat);
                if (!S_ISDIR(fileStat.st_mode)) // 디렉토리인지 체크
                        continue;
                pid = atoi(entry->d_name); // PID값 얻음
                if (pid <= 0)
                        continue;
                sprintf(tempPath, "/proc/%d/status",pid); //tmepPath에 상태 쓰기
                srcFp = fopen(tempPath,"r"); // tempPath 열기
                fgets(status,100,srcFp); // 상태 받아냄
                fgets(status,10,srcFp);
                if(status[7] == 'T'){  //process 의 상태가 ‘T’ (stop) 이라면 출력
			printf("%d\t\t",count); //프로세스  순번
			printf("%c\t\t",status[7]); //프로세스  상태
                        fclose(srcFp);
                        sprintf(tempPath, "/proc/%d/cmdline", pid); // cmd를 얻기위한 과정
                        srcFp = fopen(tempPath, "r");
                        memset(cmdLine,'\0',sizeof(cmdLine));
                        fgets(cmdLine,256,srcFp);
                        printf("%s\n",cmdLine);
			count++;
                }
                fclose(srcFp);
        }
        closedir(dir);
        return 0;
}
