/* ���� ���� include ȭ�� */
#include "smallsh.h"

/* ���α׷� ���� �� �۾��� �����͵� */
static char inpbuf[MAXBUF], tokbuf[2*MAXBUF], *ptr = inpbuf, *tok = tokbuf;

/* ������Ʈ������Ʈ�ϰ� �� ���� �д´�. */
int userin(char *p)
{
	int c, count;

	/* ���� ��ƾ���� ���� �ʱ�ȭ */
	ptr = inpbuf;
	tok = tokbuf;

	/* ������Ʈ�� ǥ���Ѵ�. */
	printf("%s",p);

	count = 0;

	while(1)
	{
		if((c = getchar()) == EOF)
			return(EOF);

		if(count < MAXBUF)
			inpbuf[count++] = c;

		if(c == '\n' && count < MAXBUF)
		{
			inpbuf[count] = '\0';
			return count;
		} /* End of if */

		/* ���̳ʹ� ��� ������Ѵ�. */
		if(c == '\n')
		{
			printf("smallsh : input line too long \n");
			count = 0;
			printf("$s",p);
		} /* End of if */
	} /* End of while */
} /* End of userin */


/* ��ū�� �����ͼ� tokbuf�� �ִ´�. */
int gettok(char **outptr)
{
	int type;

	/* outptr ���ڿ��� tok�� �����Ѵ�. */
	*outptr = tok;

	/* ��ū�� �����ϰ� �ִ� ���۷κ��� ������ �����Ѵ�. */
	while(*ptr == ' ' || *ptr == '\t')
		ptr++;

	/* ��ū �����͸� ���۳��� ù��ū���� �����Ѵ�. */
	*tok++ = *ptr;

	/* ���۳��� ��ū�� ���� ���� ������ �����Ѵ�. */
	switch(*ptr++) {
		case '\n' :
			type = EOL;
			break;
		case '&' :
			type = AMPERSAND;
			break;
		case ';' :
			type = SEMICOLON;
			break;
		default :
			type = ARG;

			/* ��ȿ�� ���빮�ڵ��� ��� �д´�. */
			while(inarg(*ptr))
				*tok++ = *ptr++;
	} /* End of which */

	*tok++ = '\0';
	return type;
} /* End of gettok */