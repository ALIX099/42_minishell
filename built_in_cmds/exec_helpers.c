/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 08:54:22 by abouknan          #+#    #+#             */
/*   Updated: 2025/08/10 08:56:02 by abouknan         ###   ########.fr       */
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

