#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define STR_LEN 1024
#define	MAX_TOKENS 10

int cmdProcessing(void);	//입력받고 처리해주는 함수
void init_myShell(char *);	//프로그램 초기 경로 설정
int cmd_cd(int argc, char *argv[]);	//chdir를 위한 함수
int cmd_set(int argc, char * argv[]);
char path[STR_LEN];	//구현한 명령어들의 경로

int main(int argc, char *argv[]){
	int isExit;
	init_myShell(argv[0]);
	while(1){	//한 명령어가 끝나도 입력을 계속 받기위한 루프
		isExit= cmdProcessing();
		if(isExit){
			break;
		}
	}
	return 0;
}


int cmdProcessing(void){
	char cmdLine[STR_LEN];	//입력된 매게변수 원문
	char *cmdTokens[MAX_TOKENS];	//mycd bin을mych,bin으로 각각 저장할 변수
	char delim[]= " \t\n\r";	// mycd bin 과 같은 명령어들을 mycd와 bin으로 자르기 위한 문자들
	char *token;	// 입력된 매게변수를 cmdTokens에 넣어주기 위한 임시변수
	pid_t child;	// fork()로 열게될 자식프로세스 변수
	int tokenNum;	//입력된 매개변수의 수 예) mycd bin 은 2
	int state;

	/* 입력할때 나오는 현재 디렉토리를 출력하기 위한 소스*/
	char buf[STR_LEN];
	char dir[STR_LEN];
	char * dire;
	strcpy(dir, getenv("PWD"));
	dire=strrchr(dir,'/');
	dire[0]=' ';
	strcpy(buf,"[20082747");
	strcat(buf, dire);
	strcat(buf, "]$");
	fputs(buf, stdout);
	fgets(cmdLine, STR_LEN, stdin);
	
	
	/* 입력된 매개변수들 자르기*/
	tokenNum=0; 
	token=strtok(cmdLine, delim);
	while(token){
		cmdTokens[tokenNum++]=token;
		token=strtok(NULL, delim);
	}
	cmdTokens[tokenNum]=NULL;

	/* 아무것도 입력안됬으면 다시입력받음*/
	if(	tokenNum == 0 )
		return 0;

	if(strcmp(cmdTokens[0], "mycd")==0)
		cmd_cd(tokenNum, cmdTokens);
	else if(strcmp(cmdTokens[0], "myset")==0){
		cmd_set(tokenNum, cmdTokens);
	}
        else if (strcmp(cmdTokens[0], "exit")==0){ //exit입력시 GoodBye~출력후 쉘 종료
		printf("GoodBye~\n");
               	return 1;
        }
	/* 자식프로세스 생성 */
	child= fork();
	if(child > 0){
		waitpid( child, &state, 0  );
		return 0;
	}
	else	if(child==0){
		/* 자식프로세스에서는 명령어를 탐색할 경로를 지정해주고 명령어에 맞는 프로그램을 열어줌*/
		char temp[STR_LEN];
		strcpy(temp,path);
		strcat(temp,"/");
		strcat(temp, cmdTokens[0]);
		if(!access(temp, X_OK) ){
			cmdTokens[0]= temp;
			execv( cmdTokens[0], cmdTokens );
		}
		else fputs("can't find command \n", stdout);
	}
	else
		printf("Can't create process. \n");

	return 0;
}

void init_myShell(char* arg){
	char temp[STR_LEN];
	int len;

	getcwd(temp, STR_LEN);
	strcat(temp, "/");
	strcat(temp, arg);
	len = strlen(temp);
	strcpy(temp+len-4, "bin:");
	strcpy(path,temp);
	len = strlen(temp);
	path[len-1]=0;
	strcat(temp, getenv("PATH") );
	setenv("PATH", temp, 1);
}
int cmd_cd(int argc, char *argv[]){
	char curpath[STR_LEN];
	int dest = 1;

	if( argc == 1 )
		chdir(getenv("HOME"));
	
	getcwd(curpath, STR_LEN);
	setenv("OLDPWD",curpath,1);

	chdir(argv[dest]);
	getcwd(curpath,	STR_LEN);
	setenv("PWD",curpath,1);

	return 0;

}
int cmd_set(int argc, char *argv[]){
	char *value;
        char *pa = "PATH";
        value = getenv("PATH");
        if(strcmp(argv[1],"PATH-n")==0)
        {
                setenv("PATH",argv[2],1);
        }

        if(strcmp(argv[1],"PATH-a")==0)
        {
		strcat(value,":");
                strcat(value,argv[2]);
                setenv("PATH",value,1);
        }
        return 0;

}
