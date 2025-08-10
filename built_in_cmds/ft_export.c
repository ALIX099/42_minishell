/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 05:50:03 by abouknan          #+#    #+#             */
/*   Updated: 2025/08/10 22:52:37 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void	print_export(t_exec *data)
{
	int		i;
	char	**export_array;

	export_array = append_to_array(data->my_env, 1);
	if (!export_array)
		return ;
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

static int	valid_export(t_expand_arg **args)
{
    int j;
	int	i;

	i = 0;
	while (args[++i])
	{
        j = 0;
        while (args[i]->value[++j])
        {
            if (!ft_isalpha(args[i]->value[0]) || args[i]->value[0] != '_')
			    return (write(2, "rsh : bash: export: ", 20), write(2,
					args[1]->value, ft_strlen(args[1]->value)), write(2,
					": not a valid identifier\n", 25), 0);
            if (!ft_isalnum(args[i]->value[i]))
                return (write(2, "rsh : bash: export: ", 20), write(2,
					args[1]->value, ft_strlen(args[1]->value)), write(2,
					": not a valid identifier\n", 25), 0);
        }
	}
}

int	ft_export(t_ast *ast, t_expand_arg **args)
{
	if (count_args(args) == 1)
		return (print_export(ast->exec), 0);
	if (!valid_export(args))
		return (1);
    
	return (0);
}
