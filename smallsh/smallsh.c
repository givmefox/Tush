#include "smallsh.h"

char *prompt ="Command> ";

int main(void)
{
	while(userin(prompt) != EOF)
		procline();

	return 0;
}
