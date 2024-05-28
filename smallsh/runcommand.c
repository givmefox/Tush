#include "smallsh.h"

/* wait를 선택사항으로 하여 명령을수행한다. */
int runcommand(char **cline, int where)
{
	pid_t pid;
	int status;

	switch(pid = fork()) {
		case -1 :
			perror("smallsh");
			return (-1);
		case 0 :
			execvp(*cline, cline);
			perror(*cline);
			exit(1);
	} /* End of switch */

	/* 부모의 코드 */
	/* 만일백그라운드 프로세스이면 프로세스 식별자를 프린트하고 퇴장한다. */
	if (where == BACKGROUND)
	{
		printf("[Process id %d]\n",pid);
		return (0);
	}

	/* 프로세스 pid가 퇴장할 때까지 기다린다. */
	if(waitpid(pid, &status, 0) == -1)
		return (-1);
	else
		return (status);
} /* End of runcommadn */
