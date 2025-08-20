/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   r_heredoc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 23:15:56 by abouknan          #+#    #+#             */
/*   Updated: 2025/08/20 23:48:31 by abouknan         ###   ########.fr       */
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
		return (write(1, *content, ft_strlen(*content)), free(*content), 2);
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

void	expand_heredoc(t_heredoc *heredoc, t_exec *data)
{
	char	*expanded;

	if (!heredoc->raw_body || heredoc->quoted)
		return ;
	expanded = expand_variables_in_str(heredoc->raw_body, data);
	free(heredoc->raw_body);
	heredoc->raw_body = expanded;
}

int	prepare_heredoc(t_redirect *r, t_exec *data)
{
	char	*line;
	char	*content;
	int		status;

	(signal(SIGINT, SIG_DFL), signal(SIGQUIT, SIG_IGN));
	while (r)
	{
		if (r->type == REDIRECT_HEREDOC)
		{
			content = ft_strdup("");
			while (1)
			{
				status = ft_do_while(&line, &content, r);
				if (status == 1)
					break ;
				if (status == 2)
					return (1);
			}
			r->heredoc->raw_body = content;
			if (r->type == REDIRECT_HEREDOC && !r->heredoc->quoted)
				expand_heredoc(r->heredoc, data);
		}
		r = r->next;
	}
	return (0);
}