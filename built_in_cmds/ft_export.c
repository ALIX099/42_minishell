/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 05:50:03 by abouknan          #+#    #+#             */
/*   Updated: 2025/08/11 00:45:13 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void	print_export(t_exec *data)
{
	int		i;
	char	**export_array;
	char	*equal_sign;

	export_array = append_to_array(data->my_env, 1);
	if (!export_array)
		return ;
	sort_env_array(export_array);
	i = 0;
	while (export_array[i])
	{
		write(1, "declare -x ", 11);
		equal_sign = ft_strchr(export_array[i], '=');
		if (equal_sign)
		{
			write(1, export_array[i], equal_sign - export_array[i] + 1);
			printf("\"%s\"\n", equal_sign + 1);
		}
		else
			printf("%s\n", export_array[i]);
		free(export_array[i]);
		i++;
	}
	free(export_array);
}

static int	valid_export(t_expand_arg **args)
{
	int	j;
	int	i;

	i = 0;
	while (args[++i])
	{
		if (!ft_isalpha(args[i]->value[0]) && args[i]->value[0] != '_')
			return (write(2, "rsh : bash: export: ", 20), write(2,
					args[1]->value, ft_strlen(args[1]->value)), write(2,
					": not a valid identifier\n", 25), 0);
		j = 0;
		while (args[i]->value[++j] && args[i]->value[j] != '=')
		{
			if (!ft_isalnum(args[i]->value[j]))
				return (write(2, "rsh : bash: export: ", 20), write(2,
						args[1]->value, ft_strlen(args[1]->value)), write(2,
						": not a valid identifier\n", 25), 0);
		}
	}
	return (1);
}

void	export_args(t_ast *ast, t_expand_arg **args)
{
	int		i;
	char	*key;
	char	*value;
	char	*equal_sign;

	i = 0;
	while (args[++i])
	{
		equal_sign = ft_strchr(args[i]->value, '=');
		if (equal_sign)
		{
			key = ft_substr(args[i]->value, 0, equal_sign - args[i]->value);
			value = ft_strdup(equal_sign + 1);
		}
		else
		{
			key = ft_strdup(args[i]->value);
			value = NULL;
		}
	}
	if (get_env(ast, key))
		update_env_value(ast->exec->my_env, key, value);
	else
		add_back(&ast->exec->my_env, env_new(key, value));
}

int	ft_export(t_ast *ast, t_expand_arg **args)
{
	if (count_args(args) == 1)
		return (print_export(ast->exec), 0);
	if (!ast || args[1]->value == 0)
		return (write(2, "rsh : bash: export: ", 20), write(2, args[1]->value,
				ft_strlen(args[1]->value)), write(2,
				": not a valid identifier\n", 25), 1);
	if (count_args(args) > 1 && args[1]->value[0] == '-')
		return (write(2, "rsh: export: options are invalid\n", 30), 1);
	if (!valid_export(args))
		return (1);
	export_args(ast, args);
	return (0);
}
