// Description: This is a mock main.c file to test parsing.
//To Be replaced with the actual main.c file

#include "parser/minishell_parser.h"

int main(void)
{
	char		*line;
	t_command	*cmds;

	while (1)
	{
		line = readline("shell> ");
		cmds = parse(line);
		//Execution ghydirha 3lii
	}
}