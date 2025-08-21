/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 03:48:16 by ikarouat          #+#    #+#             */
/*   Updated: 2025/08/21 23:18:35 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

static int	is_delimiter_quoted(t_segment *seg)
{
	while (seg)
	{
		if (seg->state == S_QUOTED || seg->state == D_QUOTED)
			return (1);
		seg = seg->next;
	}
	return (0);
}

static void	init_heredoc(t_redirect *redir, t_token *token)
{
	redir->heredoc->delimeter = ft_strdup(token->value);
	redir->heredoc->quoted = is_delimiter_quoted(token->segments);
	redir->heredoc->fd = -1;
	redir->heredoc->raw_body = NULL;
}

static int	setup_heredoc_file(t_redirect *redir, t_token *token)
{
	redir->file = malloc(sizeof(t_expand_arg));
	if (!redir->file)
		return (0);
	redir->file->value = ft_strdup(token->value);
	redir->file->segments = deep_copy_segments(token->segments);
	return (1);
}

t_redirect	*setup_heredoc(t_token **tokens)
{
	t_redirect	*redir;
	t_token		*old;

	redir = malloc(sizeof(t_redirect));
	if (!redir)
		return (NULL);
	redir->type = REDIRECT_HEREDOC;
	old = *tokens;
	*tokens = (*tokens)->next;
	free_token(old);
	if (!*tokens || (*tokens)->type != IS_WORD)
	{
		syntax_error("expected delimiter after heredoc", *tokens);
		free(redir);
		return (NULL);
	}
	redir->heredoc = malloc(sizeof(t_heredoc));
	if (!redir->heredoc)
		return (free(redir), NULL);
	init_heredoc(redir, *tokens);
	if (!setup_heredoc_file(redir, *tokens))
		return (free(redir->heredoc), free(redir), NULL);
	return (redir);
}
