/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbookpro <macbookpro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 19:40:15 by macbookpro        #+#    #+#             */
/*   Updated: 2025/08/05 20:00:51 by macbookpro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

t_env	*env_new(const char *key, const char *value)
{
	t_env *node;

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

void    add_back(t_env **env, t_env *to_add)
{
    t_env *last;
    
    if (!env || !to_add)
        return ;
    if (*env == NULL)
        *env = to_add;
    else
    {
        last = lst_last(*env);
        last ->next = to_add;
    }
}