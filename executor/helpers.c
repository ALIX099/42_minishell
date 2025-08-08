/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 19:40:15 by macbookpro        #+#    #+#             */
/*   Updated: 2025/08/07 21:44:16 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

t_env	*env_new(const char *key, const char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	if (key)
		node->key = ft_strdup(key);
	if (value)
		node->value = ft_strdup(value);
	node->next = NULL;
	return (node);
}

t_env	*lst_last(t_env *env)
{
	if (!env)
		return (NULL);
	while (env->next)
		env = env->next;
	return (env);
}

void	add_back(t_env **env, t_env *to_add)
{
	t_env	*last;

	if (!env || !to_add)
		return ;
	if (*env == NULL)
		*env = to_add;
	else
	{
		last = lst_last(*env);
		last->next = to_add;
	}
}

char	*ft_strjoin_free(char *s1, const char *s2)
{
	char	*joined;

	joined = ft_strjoin(s1, s2);
	free(s1);
	return (joined);
}

int	get_env(t_ast *ast, const char *key)
{
	t_env	*tmp;

	tmp = ast->exec->my_env;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, key))
			return (1);
		tmp = tmp->next;
	}
	return (0);
}
