/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbookpro <macbookpro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 05:49:30 by abouknan          #+#    #+#             */
/*   Updated: 2025/08/07 06:26:29 by macbookpro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

static int	count_args(t_expand_arg **args)
{
	int	count = 0;

	while (args && args[count])
		count++;
	return (count);
}

int	ft_cd(t_expand_arg **args)
{
	char	*path;
	char	*home;

	if (count_args(args) > 2)
	{
		write(2, "rsh: cd: too many arguments\n", 28);
		return (EXIT_FAILURE);
	}
	if (!args[1] || !args[1]->value || args[1]->value[0] == '\0')
	{
		home = getenv("HOME");
		if (!home)
		{
			write(2, "rsh: cd: HOME not set\n", 23);
			return (EXIT_FAILURE);
		}
		path = home;
	}
	else
		path = args[1]->value;
	if (chdir(path) == -1)
		return (perror("rsh: cd"), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
