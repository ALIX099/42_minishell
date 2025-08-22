/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 00:08:23 by abouknan          #+#    #+#             */
/*   Updated: 2025/08/22 03:31:45 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void	r_out(t_redirect *r)
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

void	r_in(t_redirect *r)
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

void	r_append(t_redirect *r)
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

void	apply_redirections(t_redirect *curr)
{
	while (curr)
	{
		if (curr->type == REDIRECT_OUT)
			r_out(curr);
		else if (curr->type == REDIRECT_APPEND)
			r_append(curr);
		else if (curr->type == REDIRECT_IN)
			r_in(curr);
		else if (curr->type == REDIRECT_HEREDOC)
			r_heredoc(curr);
		curr = curr->next;
	}
}

int	ft_redirections(t_ast *ast, t_redirect *r)
{
	pid_t	pid;

	if (prepare_heredoc(r, ast->exec))
		return (1);
	pid = fork();
	if (pid < 0)
		return (perror("fork"), 1);
	if (pid == 0)
		child_process(ast, r);
	return (handle_child_status(pid));
}
