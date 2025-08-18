/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikarouat <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 23:57:35 by ikarouat          #+#    #+#             */
/*   Updated: 2025/08/18 01:18:09 by ikarouat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_node(char **arg)
{
	char	*dollar;
	char	*env_var;
	char	*env_val;
	char	*new_arg;
	size_t	i;
	size_t	var_len;
	size_t	prefix_len;

	if (!*arg)
		return ;
	dollar = strchr(*arg, '$');
	if (!dollar || (!ft_isalnum(*(dollar + 1)) || *(dollar + 1) == '_'))
		return ;
	var_len = 0;
	prefix_len = dollar - *arg;
	while ((ft_isalnum((dollar + 1)[var_len]) || (dollar + 1)[var_len] == '_'))
		var_len++;
	env_var = malloc(var_len + 1);
	ft_strlcpy(env_var, (dollar + 1), var_len + 1);
	env_val = getenv(env_var);
	if (!env_val)
		env_val = "";
	free(env_var);
	new_arg = malloc(prefix_len + ft_strlen(env_val) + ft_strlen((dollar + var_len)));
	i = -1;
	while (++i < prefix_len)
		new_arg[i] = (*arg)[i];
	new_arg[i] = 0;
	new_arg = ft_strjoin(new_arg, env_val);
	new_arg = ft_strjoin(new_arg, dollar + var_len + 1);
	free(*arg);
	*arg = NULL;
	*arg = new_arg;
}

void	expand_redir_list(t_redirect **redirections)
{
	t_redirect	*tmp;

	tmp = *redirections;
	while (tmp)
	{
		if (tmp->file->expandable == EXPAND && tmp->type != REDIRECT_HEREDOC)
			expand_node(&(tmp->file->value));
		tmp = tmp->next;
	}
	return ;
}

void	expand(t_ast *ast)//Checkpoint: To Be modified
{
	int	i;

	if (!ast)
		return ;
	if (ast->type == NODE_CMD)
	{
		i = 0;
		while (ast->argv[i])
		{
			if (ast->argv[i]->expandable == EXPAND)
				expand_node(&ast->argv[i]->value);
			i++;
		}
		if (ast->redirects)
			expand_redir_list(&ast->redirects);
	}
	else
	{
		expand(ast->left);
		expand(ast->right);
		return ;
	}
}
