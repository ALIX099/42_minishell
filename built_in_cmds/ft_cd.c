/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 05:49:30 by abouknan          #+#    #+#             */
/*   Updated: 2025/08/11 19:21:45 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void	set_env(t_ast *ast, const char *key, const char *value);

int	count_args(t_expand_arg **args)
{
	int	count;

	count = 0;
	while (args && args[count])
		count++;
	return (count);
}

int	ft_cd(t_ast *ast, t_expand_arg **args)
{
	char	buff[4096];
	char	*home_path;

	home_path = NULL;
	if (!args[1]->value)
		return (0);
	if (count_args(args) > 2)
		return (write(2, "rsh: cd: too many arguments\n", 28), 1);
	if (!args[1] || !args[1]->value || args[1]->value[0] == '\0')
	{
		home_path = getenv("HOME");
		if (!home_path)
			return (write(2, "rsh: cd: HOME not set\n", 23), 1);
	}
	else if (args[1]->value[0] == '-')
		return (write(2, "rsh: cd: options are invalid\n", 29), 1);
	else
		home_path = args[1]->value;
	set_env(ast, "OLDPWD", getcwd(buff, sizeof(buff)));
	if (chdir(home_path) == -1)
		return (perror("rsh: cd"), 1);
	set_env(ast, "PWD", getcwd(buff, sizeof(buff)));
	return (0);
}

void	update_env_value(t_env *env_list, const char *key, const char *value)
{
	t_env	*tmp;

	tmp = env_list;
	if (get_env(env_list, key))
	{
		while (tmp)
		{
			if (!ft_strcmp(tmp->key, key))
			{
				if (tmp->value)
					free(tmp->value);
				tmp->value = ft_strdup(value);
				return ;
			}
			tmp = tmp->next;
		}
	}
}

void	set_env(t_ast *ast, const char *key, const char *value)
{
	if (!key || !value)
		return ;
	update_env_value(ast->exec->my_env, key, value);
	if (!get_env(ast->exec->my_env, key))
		add_back(&ast->exec->my_env, env_new(key, value));
}
