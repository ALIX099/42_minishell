/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 10:00:00 by macbookpro        #+#    #+#             */
/*   Updated: 2025/08/06 01:51:07 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void    hard_coding_env(t_exec *exec)
{
    char buff[4096];

    exec->my_env = NULL;
    getcwd(buff, sizeof(buff));
    add_back(&exec->my_env, env_new("USER", "abouknan"));
    add_back(&exec->my_env, env_new("PWD", buff));
    add_back(&exec->my_env, env_new("OLDPWD", NULL));
    add_back(&exec->my_env, env_new("SHLVL", "1"));
    add_back(&exec->my_env, env_new("HOME", buff));
    add_back(&exec->my_env, env_new("PATH",
        "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin"));
    add_back(&exec->my_env, env_new("_", "/usr/bin/env"));
    exec->empty_env = 1;
}

t_exec	init_env(char **envp)
{
    char    *tmp;
    char    *sep;
	t_exec	exec;
	int		i;

	if (!envp || !envp[0])
	{
		hard_coding_env(&exec);
		return (exec);
	}
    exec.my_env = NULL;
    exec.empty_env = false;
    i = 0;
    while (envp[i])
    {
        sep = ft_strchr(envp[i], '=');
        sep = '\0';
        add_back(exec.my_env, env_new(envp[i], sep + 1));
        sep = '=';
    }
    change_values();
}
