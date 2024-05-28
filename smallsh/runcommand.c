#include "smallsh.h"

/* wait�� ���û������� �Ͽ� ����������Ѵ�. */
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

	/* �θ��� �ڵ� */
	/* ���Ϲ�׶��� ���μ����̸� ���μ��� �ĺ��ڸ� ����Ʈ�ϰ� �����Ѵ�. */
	if (where == BACKGROUND)
	{
		printf("[Process id %d]\n",pid);
		return (0);
	}

	/* ���μ��� pid�� ������ ������ ��ٸ���. */
	if(waitpid(pid, &status, 0) == -1)
		return (-1);
	else
		return (status);
} /* End of runcommadn */
