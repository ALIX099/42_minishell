// Description: This is a mock main.c file to test parsing.
//To Be replaced with the actual main.c file

#include "minishell_parser.h"

int main(void)
{
	char	*line;
	t_ast	*cmds;

	while (1)
	{
		line = readline("shell> ");
		cmds = parse(line);
		(void)cmds;
	}
}