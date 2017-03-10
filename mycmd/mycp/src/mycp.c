#include "cmd.h"
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#define CPO_REMOVAL 1
#define CPO_RECURSIVE 2


int argProcessing(int argc, char *argv[], char**,char**, int* );
int mycp(char* target,char* dest, int option);

int main(int argc, char *argv[]){
	char *target[MAX_TOKENS-1];
	char *dest;
	int i;
	int target_cnt;
	char *p;
	static int option=0;
	int dfile;
	struct stat buf;
	if(argc==2 && !strcmp(argv[1],"--help")){
                printf("mycp[SOURCE] [DEST]\n");
                printf("	SOURCE 파일의내용을 DEST의이름의파일로복사\n");
		printf("	DEST에해당하는파일이있는경우실패\n");
                return 0;
        }
	target_cnt= argProcessing(argc, argv, target, &dest, &option);

	if( (target[0] == NULL) || (dest == NULL) || argc<3 ){
		fputs("help: mycp target... dest \n", stdout);
		return -1;
	}
	
	if( target_cnt > 1){
		stat(dest, &buf);
		if( !(buf.st_mode & S_IFDIR)){
			fprintf(stdout,"mycp: 대상 '%s' 은 디렉터리가 아님\n",dest);
			return -1;
		}
	}

	if( option & CPO_RECURSIVE ){
		mode_t mask;
		mask = umask(0);
		umask(mask);
		stat(dest, &buf);
		mkdir(dest, 0777 ^ mask); 

	}

	for( i= 0; target[i] != NULL ; ++ i){
		mycp(target[i], dest, option);
	}
	return 0;
}

int mycp(char* target, char* dest, int option){
	struct stat buf;
	char path[STR_LEN];
	char despath[STR_LEN];
	DIR *pdir;
	struct dirent *dentry;
	char ans= 'n';
	int ftarget, fdest;
	char dbuf[4096];
	int byte;
	int permission;

//목표파일 체크
	if( !stat(target, &buf) ){
		if(buf.st_mode & S_IFDIR){
			pdir = opendir(target);
			if( option & CPO_RECURSIVE){
				while( dentry = readdir(pdir) ){
					if( !strcmp(dentry->d_name, ".") || !strcmp(dentry->d_name, "..") )
						continue;
					mkdir(dest, buf.st_mode & 00000777);
					strcpy(path, target);
					strcat(path, "/");
					strcat(path, dentry->d_name);
					strcpy(despath, dest);
					strcat(despath, "/");
					strcat(despath, dentry->d_name);

					
					mycp(path, despath, option);
				}
			}
			else
				fprintf(stdout, "mycp: '%s'를 생략중 \n",  target);
			return -1;
		}
		else{
			if( (ftarget= open(target, O_RDONLY ))==-1){
				fprintf(stdout, "mycp: '%s'를 열 수 없습니다\n", target);
				return -1;
			}
			stat(target, &buf);
			permission= buf.st_mode &00000777;
		}
	}
	else{
		printf("stat failed i 105 line \n");
		return 0;
	}
	//체크 끝

	//목적지 파일 체크
	strcpy(path, dest);

	if( !stat(dest, &buf) ){
		if(buf.st_mode & S_IFDIR){
			strcat(path, "/");
			strcat(path, target);
		}
	}

	fdest= open( path, O_CREAT | O_EXCL | O_RDONLY, 0777 );

	if(	(fdest == -1 )){
		if(option & CPO_REMOVAL ){
			fprintf(stdout,"mycp: '%s'를 덮어쓸까요? ", dest);
			fscanf(stdin,"%c",&ans);
			fgetc(stdin);
			if( ans != 'y' )
				return -1;
		}
		fdest = creat(path, 0777);
		if( fdest == -1 ){
			fprintf(stdout, "mycp: '%s'을 만들 수 없습니다\n", path);
			return -1;
		}
	}
	//목적지 파일 체크 끝 



	while(  byte= read(ftarget, dbuf,4096 )  ){
		write(fdest, dbuf,byte); 
	}

	chmod(path, permission);

	close(fdest);
	close(ftarget);

}


int argProcessing(int argc, char *argv[], char **target, char**dest, int* option ){
	int i, count=0;
	char *p;

	target[0]=NULL;
	for(i=1 ;i< argc ; ++i){
		p=argv[i];
		if( *p == '-' ){
			while( *(p++) != '\0' ){
				switch( *p ){
					case 'i':
						*option = *option | CPO_REMOVAL;
						break;
					case 'R':	
						*option = *option | CPO_RECURSIVE;
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
	target[count]= NULL;

	if( count < 1)
		return -1;

	--count;

	*dest =(char*)target[count];
	target[count] = NULL;
	
	return count;
}


