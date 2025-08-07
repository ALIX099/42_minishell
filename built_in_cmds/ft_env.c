/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbookpro <macbookpro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 18:22:47 by macbookpro        #+#    #+#             */
/*   Updated: 2025/08/07 03:09:44 by macbookpro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

int	ft_env(t_ast *ast)
{
    t_env *tmp;

    if (!ast || !ast->exec || !ast->exec->my_env)
		return (0);
    tmp = ast->exec->my_env;
    while (tmp)
    {
        if (tmp->value)
            printf ("%s=%s\n", tmp->key, tmp->value);
        tmp = tmp->next;
    }
    return (0);
}
