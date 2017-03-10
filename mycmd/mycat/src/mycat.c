#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
int mycat(int file,int num);

int main(int argc, char *argv[] ){
	int i;
	int file;
	struct stat buf;

	if( argc == 1 ){ // 인자가 옳지 않게 들어왔을때
                printf("입력에 오류가 있습니다");
                return 0;
        }
	if(argc==2 && !strcmp(argv[1],"--help")){
		printf("mycat [FILENAME] - 파일내용출력\n");
		printf("mycat [FILENAME] [NUMBER] - 파일의내용을파일의처음부터NUMBER의값에해당하는줄수만큼출력\n");
		return 0;
	}
        if( access(argv[1], R_OK) ) // 권한이 없을때
                fprintf(stdout, "mycat: %s: 허가거부 \n", argv[1]);
        stat( argv[1], &buf);
        if(buf.st_mode & S_IFDIR ){  // 열 파일이 디렉토리일때
                fprintf(stdout, "mycat: '%s'를 읽는 도중 오류 발생\n", argv[1]);
                return -1;
        }

        file = open( argv[1], O_RDONLY );  // 입력받은 파일 열기
        if( file == -1 ){
                printf("파일 읽기 오류!");
                return 0;
        }

	int num=-1;
	if(argc==3) num=atoi(argv[2]);  // 입력된 숫자 정수로 변환
	mycat(file,num);
	close(file);
	return 0;
}

int mycat(int file, int num){
	int count; // 개행의 갯수 새는 변수
	int i, j;
	char buf[4096]; //읽어들일 파일의 내용이 저장될 변수
	int state;  //읽어들인 파일 내용의 양을 저장하는 변수


	count = 0;
	for( i=1; ; ++i ){
		state = read(file, buf, 4096); //파일읽기
		for( j=0; j<state; j++ ){
			printf("%c",buf[j]);
			if( buf[j] == '\n' ){
				if( ++count == num )break;  //입력된 줄의 개수만큼 출력을 했을때 출력을 멈춤
			}
		}
		if(state<4096) break;
	}
	return 0;
}
