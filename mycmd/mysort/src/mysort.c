#include "cmd.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define REVERSE 1
#define SPACE_IGNORE 2

#define MAX_STR_LEN 2048

char* ltrim(char *s) { // 공백제거 함수
  char* begin;
  begin = s;

  while (*begin != '\0') {
    if (isspace(*begin))
      begin++;
    else {
      s = begin;
      break;
    }
  }

  return s;
}

int argProcessing(int argc, char *argv[], char **target, int* option );
int main(int argc, char *argv[] ){
	int i;
	int file;
	int count=0;
	int j=0;
	int k=0;
	char buf[1024];
	static int option=0; //입력된 옵션
	char **strings=(char **)malloc(sizeof(char *)*5); // 동적할당
	char * target[MAX_TOKENS-1]; //예) mysort -b a 입력 시 target[0]==a
	int state;
	int prev_index=0;
	if(argc==2 && !strcmp(argv[1],"--help")){
                printf("mysort [FILENAME] - 파일의내용을라인단위로정렬하여출력\n");
                printf("-b: 공백을무시하고정렬\n");
		printf("–r: 역순으로정렬\n");
		return 0;
        }
	if( argc == 1 ){
		printf("입력에 오류가 있습니다\n");
		return 0;
	}
	argProcessing(argc,argv,target,&option);
	file = open( target[0], O_RDONLY );
	if( file == -1 ){	
		printf("파일 읽기 오류!");
		return 0;
	}
        for( i=1; ; ++i ){
               	state = read(file, buf, 1024);
                if( state < -1)
			break;
                prev_index=-(j-prev_index);
                for( j=0; j<state; j++ ){
                        if( buf[j] == '\n' ){
                                char *string;
				if(j==prev_index) string = (char*)malloc(1); // 개행만 존재하는 줄일때
				else string = (char*)malloc(j-prev_index+1); // 그 줄에있는 문자의 개수만큼 동적할당

				for(k=0; k<=j-prev_index; k++){ //전 개행부분부터현 개행부분까지 값들을 동적할당한곳에 저장
					string[k]=buf[prev_index+k];
				}
				
                                prev_index=j+1;  // 개행문자가 한번더 들어가지 않기 위해 j+1을 prev_index에 저장
                                strings[count]=string; 
                                count++; // 줄 갯수
				
                                if(count%5==0){ //strings의 동적할당 용량을 초과했을때 새로 다시 동적할당
					char ** tmp=(char **)malloc(sizeof(char *)*(count+5));
					for(k=0; k<count; k++) tmp[k]=strings[k];
					free(strings);
					strings=tmp;
				}
                        }
                }
                if(state<1024) break;

        }
	
	for(i=0; i<count-1; i++){
		for(j=0; j<count-i-1; j++){
			char *j1;
			char *j2;
			if(option&SPACE_IGNORE){ //공백무시 옵션이 들어왔을때 
				j1=(char*)malloc(strlen(strings[j]));	//임시변수에 strings[j],strings[j+1]을 저장
				j2=(char*)malloc(strlen(strings[j+1]));
				strcpy(j1,strings[j]);
				strcpy(j2,strings[j+1]);
				ltrim(strings[j]); //공백제거(공백을 무시하며 sort하기 위해서)		
				ltrim(strings[j+1]);				
			}
			if(option&REVERSE){ //버블소트(내림차순)
				if(strcmp(strings[j], strings[j+1])<0){
	                        	char * tmp=strings[j];
                        		strings[j]=strings[j+1];
                	        	strings[j+1]=tmp;
				}
                	}
			else{ //버블소트(오름차순)
				if(strcmp(strings[j], strings[j+1])>0){
                                        char * tmp=strings[j];
                                        strings[j]=strings[j+1];
                                        strings[j+1]=tmp;
                                }
			}
			if(option&SPACE_IGNORE){
				free(strings[j]);
				free(strings[j+1]);
				strings[j]=j1;
				strings[j+1]=j2;
			}
        	}
	}
        for(i=0; i<count; i++){
		printf("%s",strings[i]); //정렬된 문자열들 출력
	}

	free(strings);
	
	close(file);

	return 0;
}

int argProcessing(int argc, char *argv[], char **target, int* option ){
        int i, count=0;
        char *p;

        target[0]=NULL;
        for(i=1 ;i< argc ; ++i){
                p=argv[i];
                if( *p == '-' ){
                        while( *(p++) != '\0' ){
                                switch( *p ){
                                        case 'b': //옵션에 -b가 들어왓을때
                                                *option = *option | SPACE_IGNORE;
                                                break;
                                        case 'r': // 옵션에 -r이 들어왔을때
                                                *option = *option | REVERSE;
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

