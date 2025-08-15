/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 05:50:03 by abouknan          #+#    #+#             */
/*   Updated: 2025/08/11 19:09:59 by abouknan         ###   ########.fr       */
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

static int	valid_export(char *value)
{
	int	j;

	if (!value)
		return (0);
	if (!ft_isalpha(value[0]) && value[0] != '_')
		return (write(2, "rsh: export: ", 13), write(2, value,
				ft_strlen(value)), write(2, ": not a valid identifier\n", 25),
			0);
	j = 0;
	while (value[++j] && value[j] != '=')
	{
		if (!ft_isalnum(value[j]) && value[j] != '_')
			return (write(2, "rsh: export: ", 13), write(2, value,
					ft_strlen(value)), write(2, ": not a valid identifier\n",
					25), 0);
	}
	return (1);
}

static void	export_args(t_expand_arg **args, t_exec *data)
{
	int		i;
	char	*sign;

	i = 0;
	while (args[++i] != 0 && args[i]->value)
	{
		if (!valid_export(args[i]->value))
			continue;
		sign = ft_strchr(args[i]->value, '=');
		if (sign)
		{
			data->key = ft_substr(args[i]->value, 0, sign
					- args[i]->value);
			data->value = ft_strdup(sign + 1);
		}
		else
		{
			data->key = ft_strdup(args[i]->value);
			data->value = NULL;
		}
		update_env_value(data->my_env, data->key, data->value);
		if (!get_env(data->my_env, data->key))
			add_back(&data->my_env, env_new(data->key, data->value));
		(free(data->key), free(data->value));
	}
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
	export_args(args, ast->exec);
	return (0);
}
