/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables_in_str.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 04:05:07 by ikarouat          #+#    #+#             */
/*   Updated: 2025/08/22 03:51:20 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	get_var_info(char *str, char **var_name)
{
	size_t	var_len;

	var_len = 0;
	while (ft_isalnum(str[var_len]) || str[var_len] == '_')
		var_len++;
	*var_name = malloc(var_len + 1);
	ft_strlcpy(*var_name, str, var_len + 1);
	return (var_len);
}

static char	*allocate_new_result(char *result, char *dollar, char *env_val,
		size_t var_len)
{
	size_t	prefix_len;
	char	*new_result;
	size_t	new_len;

	prefix_len = dollar - result;
	new_len = prefix_len + ft_strlen(env_val) + ft_strlen(dollar + var_len + 1)
		+ 1;
	new_result = malloc(new_len);
	ft_strlcpy(new_result, result, prefix_len + 1);
	ft_strlcat(new_result, env_val, new_len);
	ft_strlcat(new_result, dollar + var_len + 1, new_len);
	return (new_result);
}

static char	*replace_variable(char *result, char *dollar, t_exec *data)
{
	char	*var_name;
	size_t	var_len;
	char	*env_val;
	char	*new_result;
	int		should_free;

	should_free = 1;
	var_len = get_var_info(dollar + 1, &var_name);
	env_val = get_env_value(data->my_env, var_name);
	if (!env_val)
	{
		env_val = "";
		should_free = 0;
	}
	new_result = allocate_new_result(result, dollar, env_val, var_len);
	free(var_name);
	if (should_free)
		free(env_val);
	free(result);
	return (new_result);
}

static char	*replace_exit_status(char *result, char *dollar, t_exec *data)
{
	char	*exit_status_str;
	char	*new_result;

	exit_status_str = ft_itoa(data->exit_value);
	new_result = allocate_new_result(result, dollar, exit_status_str, 1);
	free(exit_status_str);
	free(result);
	return (new_result);
}

char	*expand_variables_in_str(char *str, t_exec *data)
{
	char	*result;
	char	*dollar;

	result = ft_strdup(str);
	dollar = strchr(result, '$');
	while (dollar != NULL)
	{
		if (*(dollar + 1) == '?')
		{
			result = replace_exit_status(result, dollar, data);
			dollar = ft_strchr(result, '$');
			continue ;
		}
		if (!ft_isalnum(*(dollar + 1)) && *(dollar + 1) != '_')
		{
			dollar = ft_strchr(dollar + 1, '$');
			continue ;
		}
		result = replace_variable(result, dollar, data);
		dollar = ft_strchr(result, '$');
	}
	return (result);
}
