/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbookpro <macbookpro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 08:54:22 by abouknan          #+#    #+#             */
/*   Updated: 2025/08/10 18:55:03 by macbookpro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

char	**append_args(t_expand_arg **argv)
{
	int		count;
	int		i;
	char	**result;

	count = 0;
	while (argv[count])
		count++;
	result = malloc(sizeof(char *) * (count + 1));
	if (!result)
		return (NULL);
	i = 0;
	while (i < count)
	{
		result[i] = ft_strdup(argv[i]->value);
		i++;
	}
	result[i] = NULL;
	return (result);
}

char *get_env_value(t_env *env, const char *key)
{
	char **my_env;
	int len;
	int i;

	i = 0;
	my_env = append_to_array(env, 0);
	len = ft_strlen(key);
	while (my_env[i])
	{
		if (!ft_strncmp(my_env[i], key, len) && my_env[i][len] == '=')
			return (&my_env[i][len + 1]);
		i++;
	}
	return (NULL);
}

#include <stdlib.h>
#include <string.h>

static void	swap_strings(char **a, char **b)
{
	char *tmp = *a;
	*a = *b;
	*b = tmp;
}

void	sort_env_array(char **arr)
{
	int i;
	int j;

	if (!arr)
		return;
	i = 0;
	while (arr[i])
	{
		j = i + 1;
		while (arr[j])
		{
			if (ft_strcmp(arr[i], arr[j]) > 0)
				swap_strings(&arr[i], &arr[j]);
			j++;
		}
		i++;
	}
}
