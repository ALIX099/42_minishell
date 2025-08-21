/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_structs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikarouat <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 06:32:03 by ikarouat          #+#    #+#             */
/*   Updated: 2025/08/21 15:12:19 by ikarouat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_redirects(t_redirect *redirects)
{
	t_redirect	*curr;
	t_redirect	*next;

	curr = redirects;
	while (curr)
	{
		next = curr->next;
		if (curr->file)
		{
			free(curr->file->value);
			free(curr->file);
		}
		if (curr->heredoc)
		{
			free(curr->heredoc->delimeter);
			free(curr->heredoc->raw_body);
			free(curr->heredoc);
		}
		free(curr);
		curr = next;
	}
}

static void	free_segments(t_segment *segments)
{
	t_segment	*curr;
	t_segment	*next;

	curr = segments;
	while (curr)
	{
		next = curr->next;
		if (curr->seg_txt)
			free(curr->seg_txt);
		free(curr);
		curr = next;
	}
}

static void	free_argv(t_expand_arg **argv)
{
	int	i;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
	{
		free(argv[i]->value);
		free_segments(argv[i]->segments);
		free(argv[i]);
		i++;
	}
	free(argv);
}

void	free_ast(t_ast *ast)
{
	if (!ast)
		return ;
	free_ast(ast->left);
	ast->left = NULL;
	free_ast(ast->right);
	ast->right = NULL;
	if (ast->type == NODE_CMD)
	{
		free_argv(ast->argv);
		free_redirects(ast->redirects);
	}
	free(ast);
}

void free_token(t_token *token)
{
    if (!token)
        return;
        
    free(token->value);
    
    // Free segments
    t_segment *curr = token->segments;
    while (curr)
    {
        t_segment *next = curr->next;
        free(curr->seg_txt);
        free(curr);
        curr = next;
    }
    
    free(token);
}

void	free_env(t_exec *exec)
{
	t_env *curr;
    t_env *next;
    
    curr = exec->my_env;
    while (curr)
    {
        next = curr->next;
        free(curr->key);
        free(curr->value);
        free(curr);
        curr = next;
    }
    exec->my_env = NULL;
}
