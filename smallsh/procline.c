#include "smallsh.h"

int procline (void)
{
	char *arg[MAXARG + 1];
	int toktype;
	int narg;
	int type;

	narg = 0;

	for(;;)
	{
		/* ��ū������ ���� �ൿ�� ���Ѵ�. */
		switch (toktype = gettok(&arg[narg])) {
			case ARG :
				if(narg < MAXARG)
					narg++;
				break;
			case EOL :
			case SEMICOLON :
				if (toktype == AMPERSAND)
					type = BACKGROUND;
				else
					type = FOREGROUND;

				if(narg != 0)
				{
					arg[narg] = NULL;
					runcommand(arg, type);
				} /* End of if */

				if(toktype == EOL)
					return;

				narg = 0;
				break;
		} /* End of switch */
	} /* End of for */
} /* End of procline */
