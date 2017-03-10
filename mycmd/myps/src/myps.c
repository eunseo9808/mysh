#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc,char* argv[]){	
	DIR *dir; //디렉토리 폴더
	char *ptr,ptr1; //잘라진 문자열을 가리킬 포인터
	int count=0; // stat를 빼오기 위한 카운트
        struct dirent *entry; // proc 구조체
        struct stat fileStat; // 프로세스 상태 표시 구조체
        int pid; //PID
        char cmdLine[256]; //문자열을 받을 공간
	char cmdLine1[256];
        char tempPath[256];
	char tempPath1[256];
        FILE *srcFp, *srcp; // 파일 입출력
        dir = opendir("/proc"); //프로세스 정보 디렉토리 오픈
	if(argc==2 && !strcmp(argv[1],"--help")){
                printf("현재실행중인프로세스정보출력\n");
		printf("<UID><PID><PPID><PGID><SID><STIME><TIME><CMD>\n");
                return 0;
        }
        if(dir == NULL)
                return 0;
	printf("PID\t\t\tUID\t\t\tPPID\t\t\tPGID\t\t\tSID\t\t\tSTIME\t\t\tTIME\t\t\tCMD\n");
        while ((entry = readdir(dir)) != NULL) { //proc directory 순차검색
                lstat(entry->d_name, &fileStat);
		count=0;
                if (!S_ISDIR(fileStat.st_mode))//디렉토리인지 판단
                        continue;
                pid = atoi(entry->d_name); // pid값을 얻음
                if (pid <= 0)
                        continue;
                sprintf(tempPath, "/proc/%d/stat", pid); //cmdline : 프로세스의 이름
                srcFp = fopen(tempPath, "r"); // tempPath을 읽음
                memset(cmdLine,'\0',sizeof(cmdLine)); 
                fgets(cmdLine,256,srcFp);
		fclose(srcFp);
		
		ptr = strtok(cmdLine," "); //받아온 문자열을 잘라냄
		
		while(ptr = strtok(NULL," "))
		{
			if(count==0||count==1||count==3||count==4||count==5||count==14||count==16) //원하는 정보 거르기
			printf("%s\t\t",ptr);
			count++;
		}

		sprintf(tempPath1, "/proc/%d/cmdline", pid); //cmdline : 프로세스의 이름
                srcp = fopen(tempPath1, "r");
                memset(cmdLine1,'\0',sizeof(cmdLine1)); // CMD 출력용
                fgets(cmdLine1,256,srcp);
                fclose(srcp);
		printf("%s",cmdLine1);
		printf("\n");

        }
        closedir(dir);
        return 1;
}
