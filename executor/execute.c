/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 23:53:34 by ikarouat          #+#    #+#             */
/*   Updated: 2025/08/18 22:48:03 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_command(t_ast *ast)
{
	if (!ast->argv[0]->value[0])
		return (command_not_found(""), 127);
	if (!ft_strcmp(ast->argv[0]->value, "env"))
		return (ast->exec->exit_value = ft_env(ast));
	else if (!ft_strcmp(ast->argv[0]->value, "unset"))
		return (ast->exec->exit_value = ft_unset(&ast->exec->my_env,
				ast->argv));
	else if (!ft_strcmp(ast->argv[0]->value, "echo"))
		return (ast->exec->exit_value = ft_echo(ast, ast->argv));
	else if (!ft_strcmp(ast->argv[0]->value, "cd"))
		return (ast->exec->exit_value = ft_cd(ast, ast->argv));
	else if (!ft_strcmp(ast->argv[0]->value, "pwd"))
		return (ast->exec->exit_value = ft_pwd(ast->argv));
	else if (!ft_strcmp(ast->argv[0]->value, "export"))
		return (ast->exec->exit_value = ft_export(ast, ast->argv));
	else if (!ft_strcmp(ast->argv[0]->value, "exit"))
		return (ast->exec->exit_value = ft_exit(ast, ast->argv));
	else
		return (ast->exec->exit_value = ft_external_cmds(ast));
	return (0);
}

int	execute_subshell(t_ast *ast)
{
	pid_t	pid;
	int		value;

	if (!ast)
		return (0);
	pid = fork();
	if (pid == -1)
	return (perror("fork"), 1);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		value = execute(ast->left);
		// free_list();
		exit(value);
	}
	return (handle_child_status(pid));
}

int	execute_and_or(t_ast *ast)
{
	int	value;

	if (ast->type == NODE_AND)
	{
		value = execute(ast->left);
		if (!value)
			value = execute(ast->right);
	}
	else
	{
		value = execute(ast->left);
		if (value)
			value = execute(ast->right);
	}
	return (value);
}

int	execute(t_ast *ast)
{
	// print_ast("root", ast, 0);
	if (!ast)
		return (0);
	if (ast->type == NODE_CMD)
	{
		if (ast->redirects != NULL)
			return (ast->exec->exit_value = ft_redirections(ast,
					ast->redirects));
		return (ast->exec->exit_value = execute_command(ast));
	}
	if (ast->type == NODE_PIPE)
		return (ast->exec->exit_value = ft_pipeline(ast));
	if (ast->type == NODE_AND || ast->type == NODE_OR)
		return (ast->exec->exit_value = execute_and_or(ast));
	if (ast->type == NODE_SUBSHELL)
		return (ast->exec->exit_value = execute_subshell(ast));
	return (0); // Exit Status
}

int	handle_child_status(pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
		write(1, "Quit (core dumped)\n", 19);
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (WEXITSTATUS(status));
}
