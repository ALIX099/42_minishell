/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mock_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 02:40:38 by macbookpro        #+#    #+#             */
/*   Updated: 2025/08/17 01:06:04 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_handler(int signal)
{
	if (signal == SIGINT)
	{
		(void)signal;
		write (1, "\n", 1);
		rl_on_new_line();	
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void populate_exec_tree(t_ast **ast, t_exec *exec)
{
	if (!*ast)
		return ;
	(*ast)->exec = exec;
	populate_exec_tree(&(*ast)->left, exec);
	populate_exec_tree(&(*ast)->right, exec);
}

int	ft_readline(char **line, t_ast **cmds, t_exec *exec)
{
	*line = readline("rsh> ");
	if (!*line)
		return(0);
	if (**line)
		add_history(*line);
	*cmds = parse(*line); // Output: Abstract Syntax Tree
	if (*cmds)
		populate_exec_tree(cmds, exec);
	return (1);
}

int	main(int ac, char **av, char **envp)
{
	char	*line;
	t_ast	*cmds;
	t_exec	exec;

	(void)ac;
	(void)av;
	signal(SIGINT, sig_handler);
    signal(SIGQUIT, SIG_IGN);
	ft_memset(&exec.is_child, 0, sizeof(int));
	exec = init_env(envp);
	while (ft_readline(&line, &cmds, &exec))
	{
		expand(cmds);
		execute(cmds);
		// free(line);
		// free_ast(cmds); // Free AST from parse()
	}
	rl_clear_history();
	// cleanup_env(&exec);
	return (0);
}
