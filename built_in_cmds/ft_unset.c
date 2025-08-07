/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 05:49:57 by abouknan          #+#    #+#             */
/*   Updated: 2025/08/07 21:18:09 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

static void	get_unset(t_env **head, const char *key)
{
	t_env	*tmp;
	t_env	*prev;

	if (!head || !*head || !key)
		return ;
	tmp = *head;
	prev = NULL;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
		{
			if (prev)
				prev->next = tmp->next;
			else
				*head = tmp->next;
			free(tmp->key);
			free(tmp->value);
			free(tmp);
			return ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

int	ft_unset(t_env **head, t_expand_arg **args)
{
	int	i;

	if (!head || !args)
		return (1);
	i = 1;
	if (count_args(args) > 1 && args[1]->value[0] == '-')
		return (write(2, "rsh: unset: options are invalid\n", 32), 1);
	while (args[i])
	{
		get_unset(head, args[i]->value);
		i++;
	}
	return (0);
}
