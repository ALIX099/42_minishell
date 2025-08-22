/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mock_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 02:40:38 by abouknan          #+#    #+#             */
/*   Updated: 2025/08/21 23:56:11 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_exec	*g_exec;

void	init_vars(t_exec *data, char **envp)
{
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, SIG_IGN);
	ft_memset(&data->key, 0, sizeof(char *));
	ft_memset(&data->value, 0, sizeof(char *));
	ft_memset(&data->my_env, 0, sizeof(t_env));
	data->wait_input = 1;
	*data = init_env(envp);
	data->is_child = 0;
	data->exit_value = 0;
	g_exec = data;
}

void	sig_handler(int signal)
{
	if (signal == SIGINT && g_exec->wait_input)
	{
		(void)signal;
		g_exec->exit_value = 130;
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	populate_exec_tree(t_ast **ast, t_exec *exec)
{
	if (!*ast)
		return ;
	(*ast)->exec = exec;
	populate_exec_tree(&(*ast)->left, exec);
	populate_exec_tree(&(*ast)->right, exec);
}

int	ft_readline(char **line, t_ast **cmds, t_exec *exec)
{
	exec->wait_input = 1;
	if (exec->exit_value == 130)
		write(1, "\n", 1);
	*line = readline("rsh> ");
	exec->wait_input = 0;
	if (!*line)
	{
		write(1, "exit\n", 5);
		return (0);
	}
	if (**line)
		add_history(*line);
	*cmds = parse(*line);
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
	init_vars(&exec, envp);
	while (ft_readline(&line, &cmds, &exec))
	{
		expand(cmds);
		execute(cmds);
		free(line);
		free_ast(cmds);
	}
	free_exec(&exec);
	rl_clear_history();
	return (0);
}
