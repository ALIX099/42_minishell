#include "minishell.h"

int	ft_readline(char **line, t_ast **cmds, t_exec *exec)
{
	*line = readline("rsh> ");
	if (!*line) // Ctrl+D
		return (0);
	if (**line) // Only store non-empty commands
		add_history(*line);
	*cmds = parse(*line); // Output: Abstract Syntax Tree
	if (*cmds)
		(*cmds)->exec = exec;
	return (1);
}

int	main(int ac, char **av, char **envp)
{
	char	*line;
	t_ast	*cmds;
	t_exec	exec;

	(void)ac;
	(void)av;
	exec = init_env(envp);
	while (ft_readline(&line, &cmds, &exec))
	{
		expand(cmds);
		execute(cmds);
		free(line);
		// free_ast(cmds); // Free AST from parse()
	}
	rl_clear_history();
	// cleanup_env(&exec);
	return (0);
}
