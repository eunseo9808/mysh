#include "cmd.h"
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define RMO_INTERACTIVE 1
#define RMO_RECURSIVE 2


int argProcessing(int argc, char *argv[], char**, int* );
int myrm(char* target, int option);

int main(int argc, char *argv[]){
	char *target[MAX_TOKENS-1]; // 받아올 타켓을 가르키는 문자열포인터
	int i; 
	char *p;
	static int option=0;
	if(argc==2 && !strcmp(argv[1],"--help")){
                printf("myrm [FILENAME]\n");
		printf("	FILENAME에해당하는파일삭제\n");
		printf("myrm–rf [FILENAME]\n");
		printf("	FILENAME에해당하는파일이디렉토리일경우에도삭제\n");
		printf("	FILENAME에해당하는디렉토리가비어있지않더라도내용물까지모두삭제\n");
                return 0;
        }
	argProcessing(argc, argv, target, &option); //처리 함수

	if( (target[0] == NULL)| argc < 2 ){ //인자를 써주지 않거나 NULL왔을 경우 처리
		fputs("help: myrm FILE... \n", stdout);
		return -1;
	} 
	for( i= 0; target[i] != NULL ; ++ i){
		myrm(target[i], option); // 인자를 받아서 함수로 넘겨줌
	}
	return 0;
}

int myrm(char* target, int option){ //옵션을 받아 rm 처리를 해주는 함수
	struct stat buf; //상태 버퍼
	char path[STR_LEN]; //공간잡아주는 문자열
	DIR *pdir; //디렉토리 가르키는 포인터
	struct dirent *dentry; //디렉토리 특징
	char ans= 'n';

	if(option & RMO_INTERACTIVE ){ //파일삭제를 하는데 물음이 필요 할 경우
		stat(target, &buf);
		if(buf.st_mode & S_IFDIR){
			fprintf(stdout, "myrm: 디렉터리 '%s'로 내려가시겠습니까?", target);
			fscanf(stdin,"%c",&ans);
			fgetc(stdin);
			if( ans != 'y' )
				return -1;
		}
		else{
			fprintf(stdout, "myrm: 파일 '%s'를 삭제하시겠습니까?", target);
			fscanf(stdin,"%c",&ans);
			fgetc(stdin);
			if( ans != 'y' )
				return -1;
		}
	}

	if( unlink(target) ){ //물음없이 삭제하는 경우
		if(option&RMO_RECURSIVE){
			pdir=opendir(target);
			while( dentry = readdir(pdir)){
				if( !strcmp( dentry->d_name, ".") | !strcmp( dentry->d_name, "..")) // ../나 ./를 사용했는지 여부검사
					continue;
				strcpy(path, target);
				strcat(path, "/");
				strcat(path, dentry->d_name);
			
				myrm(path, option);
			}
			if(option & RMO_INTERACTIVE ){
				fprintf(stdout, "myrm: 디렉터리 '%s'를 삭제하시겠습니까?", target);
				fscanf(stdin,"%c",&ans);
				fgetc(stdin);
				if( ans != 'y' )
					return -1;

			}
			remove(target);
		}
		else{
			fprintf(stdout, "myrm: '%s'를 삭제할 수 없습니다 \n", target);
		}
	}

}


int argProcessing(int argc, char *argv[], char **target, int* option ){ // 옵션을 설정해주는 함수
	int i, count=0;
	char *p;

	target[0]=NULL;
	for(i=1 ;i< argc ; ++i){
		p=argv[i];
		if( *p == '-' ){
			while( *(p++) != '\0' ){
				switch( *p ){   // switch문을 통해 뒤에 들어오는 인자를 걸러줌
					case 'i':
						*option = *option | RMO_INTERACTIVE;
						break;
					case 'r':	
						*option = *option | RMO_RECURSIVE;
						break;
					case 'f':
						*option = *option | RMO_RECURSIVE;
                                                break;
					case '\0':
						break;
					default :
						fputs("부적절한 옵션", stderr);
						fputc(*p, stderr);
						fputc('\n', stderr);
						exit(1);
						break;
				}
			}
		}
		else
			target[count++]=argv[i];
	}
	
	target[count]=NULL;		
	return 0;

}


