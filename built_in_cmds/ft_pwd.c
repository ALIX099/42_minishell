/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 05:50:01 by abouknan          #+#    #+#             */
/*   Updated: 2025/08/07 21:13:18 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

int	ft_pwd(t_expand_arg **args)
{
	char	buffer[4096];
	char	*cwd;

	(void)args;
	if (count_args(args) > 1 && args[1]->value[0] == '-')
		return (write(2, "rsh: pwd: options are invalid\n", 30), 1);
	cwd = getcwd(buffer, sizeof(buffer));
	if (!cwd)
	{
		perror("rsh: pwd");
		return (1);
	}
	write(1, cwd, ft_strlen(cwd));
	write(1, "\n", 1);
	return (0);
}
