#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int mytail(int file,int num);

int main(int argc, char *argv[] ){
	int i;
	int file;
	struct stat buf;
	if(argc==2 && !strcmp(argv[1],"--help")){
                printf("mytail [FILENAME]\n");
		printf("	파일내용첫 5줄출력\n");
		printf("mytail [FILENAME] [NUMBER]\n");
		printf("	NUMBER는양의정수\n");
		printf("	파일의내용을파일의끝부터NUMBER의값에해당하는줄수만큼출력\n");
                return 0;
        }
	if( argc == 1 ){ 
		printf("입력에 오류가 있습니다"); 
		return 0;
	}
	if( access(argv[1], R_OK) )
		fprintf(stdout, "mytail: %s: 허가거부 \n", argv[1]);
	stat( argv[1], &buf);
	if(buf.st_mode & S_IFDIR ){
		fprintf(stdout, "mytail: '%s'를 읽는 도중 오류 발생\n", argv[1]);
		return -1;
	}

	file = open( argv[1], O_RDONLY );
	if( file == -1 ){	
		printf("파일 읽기 오류!");
		return 0;
	}
	int num=5;
	if(argc==3) num=atoi(argv[2]);
	mytail(file,num);
	close(file);

	return 0;
}

int mytail(int file, int num){
	int count;
	off_t pos;
	int i, j;
	char buf[4096];
	char buf2[4096];
	int state;


	count = 0;
	for( i=1; ; ++i ){
		pos = lseek( file, (off_t) - 4096*i, SEEK_END ); // 남은 읽을 파일내용을 뒤에서부터 읽어올 수 있게 커서를 설정
		if(pos < 0 )
			pos = lseek( file, (off_t)0, SEEK_SET ); // 남은 읽을 파일의 용량이 4096보다 작으면 커서를 제일 앞에둠

		state = read(file, buf, 4096); 
		if( state < -1)
			break;

		for( j=state-1; j>=0; --j ){
			if( buf[j] == '\n' ){ 
				if( ++count == num+1 )break; // 뒤에서부터 개행의 개수가 몇개인지 새서 입력한 양에 도달했을때 반복문 탈출
			}
		}

		if(count == num+1){
			pos = lseek( file, (off_t)-(state-j-1), SEEK_END ); // 밑에서부터 입력한 줄의 개수만큼을 출력하기 위해 
			break;
		}
		if(state<4096) break;
	}
	if(count!=num+1){
		pos=lseek(file, 0, SEEK_SET);  // 입력한 줄의 갯수가 파일의 줄의 갯수보다 적을때 커서를 맨앞으로 설정
	}
	while(1){
		state = read(file, buf2, 4096);
		if(state<1)
			break;
		fputs(buf2, stdout);
	}
	return 0;
}
