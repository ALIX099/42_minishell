/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbookpro <macbookpro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 05:50:23 by abouknan          #+#    #+#             */
/*   Updated: 2025/08/07 04:03:52 by macbookpro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

int check_echo_flag(t_expand_arg **args, int *start)
{
    int i = 1;
    int n_flag = 0;

    while (args[i] && args[i]->value[0] == '-')
    {
        if (args[i]->value[1] == '\0')
            break;
        int j = 1;
        while (args[i]->value[j] == 'n')
            j++;
        if (args[i]->value[j] != '\0')
            break;
        n_flag = 1;
        i++;
    }
    *start = i;
    return n_flag;
}

int ft_echo(t_expand_arg **args)
{
    int i;
    int n_flag;

    n_flag = check_echo_flag(args, &i);
    while (args[i])
    {
        write(1, args[i]->value, ft_strlen(args[i]->value));
        if (args[i + 1])
            write(1, " ", 1);
        i++;
    }
    if (!n_flag)
        write(1, "\n", 1);
    return 0;
}
