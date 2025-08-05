/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbookpro <macbookpro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 10:00:00 by macbookpro        #+#    #+#             */
/*   Updated: 2025/08/05 20:28:33 by macbookpro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void    hard_coding_env(t_exec *exec)
{
    char buff[4096];

    exec->my_env = NULL;
    getcwd(buff, sizeof(buff));

    add_back(&exec->my_env, env_new("PWD", buff));
    add_back(&exec->my_env, env_new("OLDPWD", NULL));
    add_back(&exec->my_env, env_new("SHLVL", "1"));
    add_back(&exec->my_env, env_new("HOME", buff));
    add_back(&exec->my_env, env_new("_", "/usr/bin/env"));
    exec->empty_env = 1;
}

t_exec       init_env(char **envp)
{
    t_exec exec;
    int i;

    i = 0;
    if (!envp || !envp[0])
    {
        hard_coding_env(&exec);
        return (exec.exit_value = 0, exec);
    }
    else
    {
        
    }   
}
