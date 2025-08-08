/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbookpro <macbookpro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 10:00:00 by macbookpro        #+#    #+#             */
/*   Updated: 2025/08/08 12:04:05 by macbookpro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

int	lst_size(t_env *env)
{
	int		i;
	t_env	*tmp;

	tmp = env;
	i = 0;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

static void	change_values(t_env *env)
{
	t_env	*tmp;
	int		nb;

	tmp = env;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, "SHLVL"))
		{
			nb = ft_atoi(tmp->value);
			free(tmp->value);
			tmp->value = ft_itoa(nb + 1);
		}
        else if (!ft_strcmp(tmp->key, "_"))
        {
            free(tmp->value);
            tmp->value = ft_strdup("/usr/bin/env");
        }
		tmp = tmp->next;
	}
}

static void	hard_coding_env(t_exec *exec)
{
	char	buff[4096];

	exec->my_env = NULL;
	getcwd(buff, sizeof(buff));
	add_back(&exec->my_env, env_new("PWD", buff));
	add_back(&exec->my_env, env_new("OLDPWD", NULL));
	add_back(&exec->my_env, env_new("SHLVL", "1"));
	// add_back(&exec->my_env, env_new("HOME", buff));
	add_back(&exec->my_env, env_new("PATH",
			"/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin"));
	add_back(&exec->my_env, env_new("_", "/usr/bin/env"));
	exec->empty_env = true;
}

t_exec	init_env(char **envp)
{
	char	*sep;
	t_exec	exec;
	int		i;

	if (!envp || !envp[0])
	{
		hard_coding_env(&exec);
		return (exec);
	}
	exec.my_env = NULL;
	exec.empty_env = false;
	i = -1;
	while (envp[++i])
	{
		sep = ft_strchr(envp[i], '=');
		if (sep)
		{
			*sep = '\0';
			add_back(&exec.my_env, env_new(envp[i], sep + 1));
			*sep = '=';
		}
	}
	change_values(exec.my_env);
	return (exec);
}

char	**append_to_array(t_env *env)
{
	char	*first;
	char	**array;
	t_env	*tmp;
	int		i;

	i = 0;
	tmp = env;
	array = (char **)malloc((lst_size(env) + 1) * sizeof(char *));
	if (!array)
		return (NULL);
	while (tmp)
	{
		if (tmp->value)
		{
			first = ft_strjoin(tmp->key, "=");
			array[i] = ft_strjoin_free(first, tmp->value);
		}
		else
			array[i] = ft_strjoin(tmp->key, "=");
		i++;
		tmp = tmp->next;
	}
	array[i] = NULL;
	return (array);
}
