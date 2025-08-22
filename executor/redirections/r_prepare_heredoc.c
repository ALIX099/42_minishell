/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   r_prepare_heredoc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 03:17:55 by abouknan          #+#    #+#             */
/*   Updated: 2025/08/22 03:18:24 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	restore_signals(void (*old_sigint)(int), void (*old_sigquit)(int))
{
	signal(SIGINT, old_sigint);
	signal(SIGQUIT, old_sigquit);
}

static int	process_single_heredoc(t_redirect *curr, t_exec *data,
		void (*old_sigint)(int), void (*old_sigquit)(int))
{
	int		pipe_fds[2];
	pid_t	pid;
	int		status;

	if (pipe(pipe_fds) < 0)
		return (restore_signals(old_sigint, old_sigquit), perror("pipe"), 1);
	pid = fork();
	if (pid < 0)
		return (close(pipe_fds[0]), close(pipe_fds[1]),
			restore_signals(old_sigint, old_sigquit), perror("fork"), 1);
	if (pid == 0)
		handle_heredoc_child(pipe_fds, curr->heredoc);
	waitpid(pid, &status, 0);
	if (handle_heredoc_parent(pipe_fds, curr, status, data))
		return (restore_signals(old_sigint, old_sigquit), 1);
	return (0);
}

int	prepare_heredoc(t_redirect *r, t_exec *data)
{
	t_redirect	*curr;
	void		(*old_sigint)(int);
	void		(*old_sigquit)(int);

	curr = r;
	old_sigint = signal(SIGINT, SIG_IGN);
	old_sigquit = signal(SIGQUIT, SIG_IGN);
	while (curr)
	{
		if (curr->type == REDIRECT_HEREDOC)
		{
			if (process_single_heredoc(curr, data, old_sigint, old_sigquit))
				return (1);
		}
		curr = curr->next;
	}
	restore_signals(old_sigint, old_sigquit);
	return (0);
}
