/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   r_heredoc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 23:15:56 by abouknan          #+#    #+#             */
/*   Updated: 2025/08/22 03:35:45 by abouknan         ###   ########.fr       */
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

void	expand_heredoc(t_heredoc *heredoc, t_exec *data)
{
	char	*expanded;

	if (!heredoc->raw_body || heredoc->quoted)
		return ;
	expanded = expand_variables_in_str(heredoc->raw_body, data);
	free(heredoc->raw_body);
	heredoc->raw_body = expanded;
}
