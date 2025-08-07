/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbookpro <macbookpro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 05:50:01 by abouknan          #+#    #+#             */
/*   Updated: 2025/08/07 05:22:47 by macbookpro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

int	ft_pwd(t_expand_arg **args)
{
	char	buffer[4096];
	char	*cwd;
    
    if (args[1] && args[1]->value[0] == '-' && args[1]->value[1])
    {
        if (args[1]->value[1] != '\0' && args[1]->value[2] != '\0') 
        {
            write(2, "rsh: pwd: invalid option\n", 25);
            return (EXIT_FAILURE); 
        }
    }
	cwd = getcwd(buffer, sizeof(buffer));
	if (!cwd)
	{
		perror("rsh: pwd");
		return (EXIT_FAILURE);
	}

	write(1, cwd, ft_strlen(cwd));
	write(1, "\n", 1);
	return (EXIT_SUCCESS);
}
