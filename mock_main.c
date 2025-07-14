// Description: This is a mock main.c file to test parsing.
//To Be replaced with the actual main.c file

#include "minishell_parser.h"

int main()//int argc, char **argv, char **envp)
{
	char	*line;
	t_ast	*cmds;

	while (1)
	{
		line = readline("rsh> ");
		cmds = parse(line);//Output: Abstract Syntax Tree
		execute(cmds);
	}
}