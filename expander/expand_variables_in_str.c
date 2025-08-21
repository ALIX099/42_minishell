/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables_in_str.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikarouat <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 04:05:07 by ikarouat          #+#    #+#             */
/*   Updated: 2025/08/21 06:54:07 by ikarouat         ###   ########.fr       */
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

static char	*replace_variable(char *result, char *dollar, t_exec *data)
{
	size_t	prefix_len;
	char	*var_name;
	size_t	var_len;
	char	*env_val;
	char	*new_result;
	size_t	new_len;

	prefix_len = dollar - result;
	var_len = get_var_info(dollar + 1, &var_name);
	env_val = get_env_value(data->my_env, var_name);
	if (!env_val)
		env_val = "";
	new_len = prefix_len + ft_strlen(env_val) + ft_strlen(dollar + var_len + 1)
		+ 1;
	new_result = malloc(new_len);
	ft_strlcpy(new_result, result, prefix_len + 1);
	ft_strlcat(new_result, env_val, new_len);
	ft_strlcat(new_result, dollar + var_len + 1, new_len);
	free(var_name);
	free(result);
	return (new_result);
}

char	*expand_variables_in_str(char *str, t_exec *data)
{
	char	*result;
	char	*dollar;

	result = ft_strdup(str);
	while ((dollar = strchr(result, '$')) != NULL)
	{
		if (!ft_isalnum(*(dollar + 1)) && *(dollar + 1) != '_')
			break ;
		result = replace_variable(result, dollar, data);
	}
	return (result);
}
