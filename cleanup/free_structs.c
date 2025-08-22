/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_structs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 06:32:03 by ikarouat          #+#    #+#             */
/*   Updated: 2025/08/22 02:52:13 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_argv(t_expand_arg **argv)
{
	int	i;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
	{
		if (argv[i]->value)
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
	free_ast(ast->right);
	if (ast->redirects)
		free_redirects(ast->redirects);
	if (ast->argv)
		free_argv(ast->argv);
	free(ast);
}

void	free_token(t_token *token)
{
	t_segment	*curr;
	t_segment	*next;

	if (!token)
		return ;
	free(token->value);
	curr = token->segments;
	while (curr)
	{
		next = curr->next;
		free(curr->seg_txt);
		free(curr);
		curr = next;
	}
	free(token);
}

void	free_token_list(t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	current = tokens;
	while (current)
	{
		next = current->next;
		free_token(current);
		current = next;
	}
}

void	free_exec(t_exec *exec)
{
	t_env	*curr;
	t_env	*next;

	curr = exec->my_env;
	while (curr)
	{
		next = curr->next;
		if (curr->key)
			free(curr->key);
		if (curr->value)
			free(curr->value);
		free(curr);
		curr = next;
	}
	exec->my_env = NULL;
}
