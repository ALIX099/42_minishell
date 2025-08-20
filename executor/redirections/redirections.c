/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 00:08:23 by abouknan          #+#    #+#             */
/*   Updated: 2025/08/20 23:37:17 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

static void	r_out(t_redirect *r)
{
	int	fd;

	fd = open(r->file->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("open");
		exit(1);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		perror("dup2");
		exit(1);
	}
	close(fd);
}

static void	r_in(t_redirect *r)
{
	int	fd;

	fd = open(r->file->value, O_RDONLY);
	if (fd < 0)
	{
		perror("open");
		exit(1);
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		perror("dup2");
		exit(1);
	}
	close(fd);
}

static void	r_append(t_redirect *r)
{
	int	fd;

	fd = open(r->file->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror("open");
		exit(1);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		perror("dup2");
		exit(1);
	}
	close(fd);
}

int	ft_redirections(t_ast *ast, t_redirect *r)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (perror("fork"), 1);
	if (pid == 0)
	{
		(signal(SIGINT, SIG_DFL), signal(SIGQUIT, SIG_DFL));
		while (r)
		{
			if (r->type == REDIRECT_OUT)
				r_out(r);
			else if (r->type == REDIRECT_APPEND)
				r_append(r);
			else if (r->type == REDIRECT_IN)
				r_in(r);
			else if (r->type == REDIRECT_HEREDOC)
				(prepare_heredoc(r, ast->exec), r_heredoc(r));
			r = r->next;
		}
		exit(execute_command(ast));
	}
	else
		return (handle_child_status(pid));
}
