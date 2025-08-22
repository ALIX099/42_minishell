/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   r_heredoc_parent.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 03:16:54 by abouknan          #+#    #+#             */
/*   Updated: 2025/08/22 03:38:38 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	should_expand(t_redirect *r, t_exec *data)
{
	if (r->type == REDIRECT_HEREDOC && !r->heredoc->quoted)
		expand_heredoc(r->heredoc, data);
}

static void	read_heredoc_content(int pipe_fd, t_redirect *curr)
{
	char	buffer[4096];
	int		bytes_read;
	char	*tmp;

	if (curr->heredoc->raw_body)
		free(curr->heredoc->raw_body);
	curr->heredoc->raw_body = ft_strdup("");
	bytes_read = read(pipe_fd, buffer, 4095);
	while (bytes_read > 0)
	{
		buffer[bytes_read] = '\0';
		tmp = curr->heredoc->raw_body;
		curr->heredoc->raw_body = ft_strjoin(curr->heredoc->raw_body, buffer);
		free(tmp);
		bytes_read = read(pipe_fd, buffer, 4095);
	}
}

int	handle_heredoc_parent(int pipe_fd[2], t_redirect *curr, int status,
		t_exec *data)
{
	close(pipe_fd[1]);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		close(pipe_fd[0]);
		return (1);
	}
	read_heredoc_content(pipe_fd[0], curr);
	close(pipe_fd[0]);
	should_expand(curr, data);
	return (0);
}
