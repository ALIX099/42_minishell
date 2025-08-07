// Description: This is a mock main.c file to test parsing.
// To Be replaced with the actual main.c file

#include "minishell.h"

int	main(int ac, char **av, char **envp)
{
	char	*line;
	t_ast	*cmds;
	t_exec	exec;

	(void)ac;
	(void)av;
	exec = init_env(envp);
	while (1)
	{
		line = readline("rsh> ");
		if (!line)
			break ;
		cmds = parse(line); // Output: Abstract Syntax Tree
		if (cmds)
			cmds->exec = &exec;
		expand(cmds);
		execute(cmds);
	}
}
