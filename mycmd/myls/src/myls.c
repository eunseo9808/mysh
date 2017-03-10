#include"cmd.h"
#include"builtin.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <time.h>

#define LSO_LIST 1
#define LSO_ALL 2
#define LSO_FORMAT 4
#define LSO_RECURSIVE 8
#define LSO_BLOCK 16
#define MAX_ENTRY 2048	

int argProcessing(int argc, char *argv[], char**, int* );
int myls(char* target, int option);
void list(struct stat buf);
	

int main(int argc, char *argv[]){
	char *target[MAX_TOKENS-1];
	int i;
	char *p;
	static int option=0;
	struct stat buf;
	
	if(argc==2 && !strcmp(argv[1],"--help")){
                printf("파일의정보를포함하는목록출력\n");
                printf("	출력형식은 ls –l 명령어와동일\n");
		printf("myls[FILENAME]\n");
		printf("	FILENAME은파일또는디렉토리\n");
		printf("–a: 숨김파일출력\n");
                return 0;
        }	
	argProcessing(argc, argv, target, &option);

	if( target[0] == NULL ){
		myls(NULL, option);
	}
	else {
		//인자가 하나인 경우.
		if( target[1] == NULL){

			stat(target[0], &buf);
			if( buf.st_mode & S_IFDIR){
				chdir(target[0]);
				myls(NULL, option);
				chdir(getenv("PWD") );
			}
			else{
				if(access(target[0],F_OK)){
					fprintf(stdout, "ls: %s에 접근할 수 없습니다. \n", target[0]);
					return -1;
				}
				fprintf(stdout, "%s\n", target[0]);
			}

		}
		else{
			//복수의 인자.
			for(i=0 ; target[i] != NULL ; ++i){

				stat(target[i], &buf);
				if( buf.st_mode & S_IFDIR){

					fputs(target[i], stdout);
					fputs(":\n",stdout);
					chdir(target[i]);
					myls(NULL, option);
					chdir(getenv("PWD") );

					if( target[ i + 1 ] != NULL){
						fputc('\n', stdout);
					}
					else 
						break;
				}
				else{
					if(access(target[i],F_OK)){
						fprintf(stdout, "ls: %s에 접근할 수 없습니다. \n", target[i]);
						continue;
					}
					fprintf(stdout, "%s\n", target[i]);
				}
			}
		}
	}
	return 0;
}



int myls(char *target, int option){
	DIR * pdir;
	int cnt=0;
	int i,j;
	struct dirent *dentry[MAX_ENTRY], *temp;
	struct stat buf;
	struct passwd *usr;
	struct group *grp;
	char path_buf[1024];
	int rflag=1;
	
	if( target == NULL){
		pdir = opendir(".");
		if( option & LSO_RECURSIVE )
			fputs(".:\n",stdout);
	}
	else
		pdir = opendir(target);

	if( pdir == NULL ){
		fprintf(stdout, "'%s'디렉터리를 여는 데 실패했습니다. \n", target);
		return -1;
	}
		

	while(dentry[cnt++] = readdir(pdir));
	cnt--;

	for( i=0; i< cnt; ++i){
		for( j=0; j<cnt; ++j){
			if( strcmp(dentry[i]->d_name,dentry[j]->d_name) < 0 ){
				temp = dentry[i];
				dentry[i] = dentry[j];
				dentry[j] = temp;
			}
		}
	}

	for(i=0; i< cnt; ++i){

		if( !(option & LSO_ALL) & (dentry[i]->d_name[0] == '.') )
			continue;
		//get stat
		//경로생성
			if( target == NULL )
				strcpy( path_buf, ".");
			else 
				strcpy( path_buf, target );

			strcat( path_buf, "/");
			strcat( path_buf, dentry[i]->d_name);
			stat(path_buf, &buf);

			//blocks
			if( option & LSO_BLOCK ){
				fprintf( stdout,"%3d ", (int)buf.st_blocks/2 );
			}

		//list option
		if( option & LSO_LIST )
			list(buf);

		fputs(dentry[i]->d_name, stdout);

		if( option & LSO_FORMAT ){
			switch( buf.st_mode & 0777000 ){
				case S_IFDIR:
					fputc('/' , stdout);
					break;
				case S_IFIFO:
					fputc('|' , stdout);
					break;
				case S_IFLNK:
					fputc('@' , stdout);
					break;
				case S_IFSOCK:
					fputc('=' , stdout);
					break;
				case S_IFREG :
					fputc('*',stdout);
				default:
					break;
			}
		}

		if( option & (LSO_LIST) )
			fputc('\n', stdout);
		else
			fputc('\t',stdout);
	}

	if( option & LSO_RECURSIVE ){
		fputc('\n', stdout);
		for( i=0 ; i<cnt; ++ i){
		if( !(option & LSO_ALL) & (dentry[i]->d_name[0] == '.') )
			continue;
		if( !strcmp( dentry[i]->d_name, ".") | !strcmp( dentry[i]->d_name, ".."))
			continue;
			//경로생성
			if( target == NULL )
				strcpy( path_buf, ".");
			else 
				strcpy( path_buf, target );

			strcat( path_buf, "/");
			strcat( path_buf, dentry[i]->d_name);
			stat(path_buf, &buf);


			if( buf.st_mode & S_IFDIR ){	
			fputc('\n', stdout);
				fputs( path_buf, stdout);
				fputs(":\n", stdout);
				myls(path_buf, option);
			}
		}
	}else
		if( !(option & LSO_LIST  ) )
			fputc('\n', stdout);
	
	closedir(pdir);

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
					case 'l':
						*option = *option | LSO_LIST;
						break;
					case 'a':	
						*option = *option | LSO_ALL;
						break;
					case 'F':	
						*option = *option | LSO_FORMAT;
						break;
					case 'R':	
						*option = *option | LSO_RECURSIVE;
						break;
					case 's':
						*option = *option | LSO_BLOCK;
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

void list(struct stat buf){
	struct tm *ctime;
	char time_buf[100];

	switch( buf.st_mode & 0777000 ){
		case S_IFDIR:
			fputc('d' , stdout);
			break;
		case S_IFCHR:
			fputc('c' , stdout);
			break;
		case S_IFBLK:
			fputc('b' , stdout);
			break;
		case S_IFIFO:
			fputc('f' , stdout);
			break;
		case S_IFLNK:
			fputc('l' , stdout);
			break;
		case S_IFSOCK:
			fputc('s' , stdout);
			break;
		default:
			fputc('-', stdout);
			break;
	}
	if( buf.st_mode & 0000400 )
		fputc('r', stdout);
	else
		fputc('-', stdout);
	if( buf.st_mode & 0000200 )
		fputc('w', stdout);
	else
		fputc('-', stdout);
	if( buf.st_mode & 0000100 )
		fputc('x', stdout);
	else
		fputc('-', stdout);
	if( buf.st_mode & 0000040 )
		fputc('r', stdout);
	else
		fputc('-', stdout);
	if( buf.st_mode & 0000020 )
		fputc('w', stdout);
	else
		fputc('-', stdout);
	if( buf.st_mode & 0000010 )
		fputc('x', stdout);
	else
		fputc('-', stdout);
	if( buf.st_mode & 0000004 )
		fputc('r', stdout);
	else
		fputc('-', stdout);
	if( buf.st_mode & 0000002 )
		fputc('w', stdout);
	else
		fputc('-', stdout);
	if( buf.st_mode & 0000001 )
		fputc('x', stdout);
	else
		fputc('-', stdout);
	fputc(' ', stdout);
	fprintf(stdout, "%2d", buf.st_nlink);
	fputc(' ', stdout);
	fputs(getpwuid(buf.st_uid)->pw_name,  stdout);
	fputc(' ', stdout);
	fputs(getgrgid(buf.st_gid)->gr_name, stdout);
	fputc(' ', stdout);
	fprintf(stdout, "%6d", (int)buf.st_size);

	ctime= localtime(&buf.st_mtime);
	strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M",ctime);
	fputc(' ', stdout);
	fputs( time_buf, stdout);
	fputc(' ', stdout);

}
