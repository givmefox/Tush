/* 예를 위한 include 화일 */
#include "smallsh.h"

/* 프로그램 버퍼 및 작업용 포인터들 */
static char inpbuf[MAXBUF], tokbuf[2*MAXBUF], *ptr = inpbuf, *tok = tokbuf;

/* 프롬프트를프린트하고 한 줄을 읽는다. */
int userin(char *p)
{
	int c, count;

	/* 뒤의 루틴들을 위한 초기화 */
	ptr = inpbuf;
	tok = tokbuf;

	/* 프롬프트를 표시한다. */
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

		/* 줄이너무 길면 재시작한다. */
		if(c == '\n')
		{
			printf("smallsh : input line too long \n");
			count = 0;
			printf("$s",p);
		} /* End of if */
	} /* End of while */
} /* End of userin */


/* 토큰을 가져와서 tokbuf에 넣는다. */
int gettok(char **outptr)
{
	int type;

	/* outptr 문자열을 tok로 지정한다. */
	*outptr = tok;

	/* 토큰을 포함하고 있는 버퍼로부터 여백을 제거한다. */
	while(*ptr == ' ' || *ptr == '\t')
		ptr++;

	/* 토큰 포인터를 버퍼내의 첫토큰으로 지정한다. */
	*tok++ = *ptr;

	/* 버퍼내의 토큰에 따라 유형 변수를 지정한다. */
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

			/* 유효한 보통문자들을 계속 읽는다. */
			while(inarg(*ptr))
				*tok++ = *ptr++;
	} /* End of which */

	*tok++ = '\0';
	return type;
} /* End of gettok */
