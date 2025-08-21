/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikarouat <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 23:57:35 by ikarouat          #+#    #+#             */
/*   Updated: 2025/08/21 06:57:15 by ikarouat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_redir_list(t_redirect **redirections, t_exec *data)
{
	t_redirect	*tmp;

	tmp = *redirections;
	while (tmp)
	{
		if (tmp->type != REDIRECT_HEREDOC)
			expand_node(&(tmp->file->value), tmp->file->segments, data);
		tmp = tmp->next;
	}
	return ;
}

void	expand(t_ast *ast)
{
	int	i;

	if (!ast)
		return ;
	if (ast->type == NODE_CMD)
	{
		i = 0;
		while (ast->argv[i])
		{
			expand_node(&ast->argv[i]->value, ast->argv[i]->segments,
				ast->exec);
			i++;
		}
		handle_word_splitting(ast);
		if (ast->redirects)
			expand_redir_list(&ast->redirects, ast->exec);
	}
	else
	{
		expand(ast->left);
		expand(ast->right);
		return ;
	}
}
