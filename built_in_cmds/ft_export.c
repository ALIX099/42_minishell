/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbookpro <macbookpro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 05:50:03 by abouknan          #+#    #+#             */
/*   Updated: 2025/08/10 19:10:16 by macbookpro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void    print_export(t_exec *data)
{
    int     i;
    char    **export_array;

    export_array = append_to_array(data->my_env, 1);
    if (!export_array)
        return;
    sort_env_array(export_array);
    i = 0;
    while (export_array[i])
    {
        printf("declare -x %s\n", export_array[i]);
        free(export_array[i]);
        i++;
    }
    free(export_array);
}


// static int valid_export(t_expand_arg **args)
// {
    
// }

int	ft_export(t_ast *ast, t_expand_arg **args)
{
    // int i;
    if (count_args(args) == 1)
        return(print_export(ast->exec), 0);
    else
        
}
