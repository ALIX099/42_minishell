/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbookpro <macbookpro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 00:08:23 by abouknan          #+#    #+#             */
/*   Updated: 2025/08/15 01:21:28 by macbookpro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

static int r_out(t_ast *ast, t_redirect *r)
{
    pid_t pid;
    int status;
    int fd;
    pid = fork();
    if (pid == -1)
        return(perror("fork"), 1);
    if (pid == 0)
    {
        fd = open(r->file->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0)
            return (perror("open"), 1);
        if (dup2(fd, STDOUT_FILENO) < 0)
            return (perror("dup2"), 1);
        close(fd);
        exit (execute_command(ast));
    }
    pid = waitpid(0, &status, 0);
    return (WEXITSTATUS(status));
}

static int	r_in(t_ast *ast, t_redirect *r)
{
	int		status;
	pid_t	pid;
	int		fd;

	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	if (pid == 0)
	{
		fd = open(r->file->value, O_RDONLY);
		if (fd < 0)
			return (perror("open"), 1);
		if (dup2(fd, STDIN_FILENO) < 0)
			return (perror("dup2"), 1);
		close(fd);
		exit(execute_command(ast));
	}
	pid = waitpid(0, &status, 0);
	return (WEXITSTATUS(status));
}

static int r_append(t_ast *ast, t_redirect *r)
{
    int status;
    pid_t pid;
    int fd;

    pid = fork();
    if (pid == -1)
        return(perror("fork"), 1);
    if (pid == 0)
    {
        fd = open(r->file->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd < 0)
            return (perror("open"), 1);
        if (dup2(fd, STDOUT_FILENO) < 0)
            return (perror("dup2"), 1);
        close(fd);
        exit(execute_command(ast));
    }
    pid = waitpid(0, &status, 0);
    return (WEXITSTATUS(status));
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
