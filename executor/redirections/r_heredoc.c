/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   r_heredoc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 23:15:56 by abouknan          #+#    #+#             */
/*   Updated: 2025/08/20 23:17:25 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void	r_heredoc(t_redirect *r)
{
	int	pipefd[2];

	if (pipe(pipefd) < 0)
		return (perror("pipe"), exit(1));
	write(pipefd[1], r->heredoc->raw_body, ft_strlen(r->heredoc->raw_body));
	close(pipefd[1]);
	if (dup2(pipefd[0], STDIN_FILENO) < 0)
		return (perror("pipe"), exit(1));
	close(pipefd[0]);
}

/*
 * Collect all heredocs before executing commands.
 * Replace heredoc redirection node with a pipe read-end.
 */

static int	ft_do_while(char **line, char **content, t_redirect *r)
{
	char	*tmp;

	*line = readline("> ");
	if (!*line)
		return (free(*content), 2);
	if (!ft_strcmp(*line, r->heredoc->delimeter))
		return (free(*line), 1);
	tmp = *content;
	*content = ft_strjoin(*content, *line);
	free(tmp);
	tmp = *content;
	*content = ft_strjoin(*content, "\n");
	(free(tmp), free(*line));
	return (0);
}

int	prepare_heredoc(t_redirect *r)
{
	char	*line;
	char	*content;
	int		status;

	signal(SIGINT, SIG_DFL);
	while (r)
	{
		if (r->type == REDIRECT_HEREDOC)
		{
			content = ft_strdup("");
			while (1)
			{
				status = ft_do_while(&line, &content, r);
				if (status == 1) // delimiter
					break ;
				if (status == 2) // EOF or Ctrl+D
					return (1);
			}
			r->heredoc->raw_body = content;
		}
		r = r->next;
	}
	return (0);
}
