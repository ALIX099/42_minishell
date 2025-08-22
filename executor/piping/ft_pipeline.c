/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipeline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 00:50:52 by abouknan          #+#    #+#             */
/*   Updated: 2025/08/22 01:45:26 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		free_ast(t_ast *ast);

static void	ft_single_left(t_ast *left, int fds[2])
{
	int	exit_status;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	close(fds[0]);
	if (dup2(fds[1], STDOUT_FILENO) < 0)
	{
		perror("dup2");
		exit(1);
	}
	left->exec->is_child = 1;
	exit_status = execute(left);
	close(fds[1]);
	free_exec(left->exec);
	free_ast(left);
	exit(exit_status);
}

static void	ft_single_right(t_ast *right, int fds[2])
{
	int	exit_status;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	close(fds[1]);
	if (dup2(fds[0], STDIN_FILENO) < 0)
	{
		perror("dup2");
		exit(1);
	}
	right->exec->is_child = 1;
	exit_status = execute(right);
	close(fds[0]);
	free_exec(right->exec);
	free_ast(right);
	exit(exit_status);
}

int	ft_pipeline(t_ast *ast)
{
	int		fds[2];
	pid_t	pid_left;
	pid_t	pid_right;

	if (pipe(fds) == -1)
		return (perror("pipe"), 1);
	pid_left = fork();
	if (pid_left == -1)
		return (perror("fork"), 1);
	if (pid_left == 0)
		ft_single_left(ast->left, fds);
	pid_right = fork();
	if (pid_right == -1)
		return (perror("fork"), 1);
	if (pid_right == 0)
		ft_single_right(ast->right, fds);
	(close(fds[0]), close(fds[1]));
	waitpid(pid_left, NULL, 0);
	return (handle_child_status(pid_right));
}
