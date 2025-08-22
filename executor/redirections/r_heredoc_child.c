/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   r_heredoc_child.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 03:16:21 by abouknan          #+#    #+#             */
/*   Updated: 2025/08/22 03:16:44 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*append_line_to_content(char *content, char *line)
{
	char	*tmp;
	char	*result;

	tmp = content;
	result = ft_strjoin(content, line);
	free(tmp);
	free(line);
	tmp = result;
	result = ft_strjoin(result, "\n");
	free(tmp);
	return (result);
}

void	handle_heredoc_child(int pipe_fd[2], t_heredoc *heredoc)
{
	char	*content;
	char	*line;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
	content = ft_strdup("");
	close(pipe_fd[0]);
	while (1)
	{
		line = readline("> ");
		if (!line)
			(free(content), close(pipe_fd[1]), exit(1));
		if (!ft_strcmp(line, heredoc->delimeter))
		{
			free(line);
			break ;
		}
		content = append_line_to_content(content, line);
	}
	write(pipe_fd[1], content, ft_strlen(content));
	(free(content), close(pipe_fd[1]), exit(0));
}
