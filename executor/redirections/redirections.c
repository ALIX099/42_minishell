/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 00:08:23 by abouknan          #+#    #+#             */
/*   Updated: 2025/08/20 05:13:35 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

static int	r_out(t_ast *ast, t_redirect *r)
{
	pid_t	pid;
	int		fd;

	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		fd = open(r->file->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
			return (perror("open"), 1);
		if (dup2(fd, STDOUT_FILENO) < 0)
			return (perror("dup2"), 1);
		close(fd);
		exit(execute_command(ast));
	}
	return (handle_child_status(pid));
}

static int	r_in(t_ast *ast, t_redirect *r)
{
	pid_t	pid;
	int		fd;

	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		fd = open(r->file->value, O_RDONLY);
		if (fd < 0)
			return (perror("open"), 1);
		if (dup2(fd, STDIN_FILENO) < 0)
			return (perror("dup2"), 1);
		close(fd);
		exit(execute_command(ast));
	}
	return (handle_child_status(pid));
}

static int	r_append(t_ast *ast, t_redirect *r)
{
	pid_t	pid;
	int		fd;

	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		fd = open(r->file->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd < 0)
			return (perror("open"), 1);
		if (dup2(fd, STDOUT_FILENO) < 0)
			return (perror("dup2"), 1);
		close(fd);
		exit(execute_command(ast));
	}
	return (handle_child_status(pid));
}

int	ft_redirections(t_ast *ast, t_redirect *r)
{
	t_redirect	*tmp;

	tmp = r;
	while (tmp)
	{
		if (tmp->type == REDIRECT_OUT)
			ast->exec->exit_value = r_out(ast, r);
		else if (tmp->type == REDIRECT_IN)
			ast->exec->exit_value = r_in(ast, r);
		else if (tmp->type == REDIRECT_APPEND)
			ast->exec->exit_value = r_append(ast, r);
		// else if (tmp->type == REDIRECT_HEREDOC)
		//     ast->exec->exit_value = r_herdoc(ast, r);
		tmp = tmp->next;
	}
	return (ast->exec->exit_value);
}
