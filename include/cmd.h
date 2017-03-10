#ifndef _CMD_H_
#define	_CMD_H_ 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STR_LEN 1024
#define	MAX_TOKENS 10

struct CMD{
	char *name;
	char *desc;
	int (*cmd)(int argc, char *agrv[]);
};

int cmdProcessing(void);
extern int cmd_cd(int argc, char *argv[]);
extern int cmd_exit(int argc, char *argv[]);
extern int cmd_pwd(int arfc, char *argv[]);
extern const int builtins;
extern struct CMD builtin[];

#endif
