/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_external.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 11:53:00 by macbookpro        #+#    #+#             */
/*   Updated: 2025/08/08 21:32:58 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

char	*find_cmnd_path_helper(char **path, char *cmnd)
{
	char	*full_path;
	int		i;

	i = 0;
	while (path[i])
	{
		full_path = ft_strjoin(path[i], "/");
		full_path = ft_strjoin_free(full_path, cmnd);
		if (!access(full_path, X_OK))
			return (full_path);
		i++;
	}
	return (NULL);
}

char	*find_path(t_ast *ast, char *cmnd)
{
	char	*path_env;
	char	**path;
	char	*result;

	if (cmnd[0] == '/' || cmnd[0] == '.')
	{
		if (access(cmnd, F_OK) == -1 || access(cmnd, X_OK) == -1)
			return (NULL);
		if (access(cmnd, X_OK) == -1)
			return (gc_strdup(ast, cmnd));
		return (gc_strdup(ast, cmnd));
	}
	path_env = get_env_value(ast->exec->my_env, "PATH");
	if (!path_env || !cmnd)
		return (NULL);
	path = ft_split(path_env, ':');
	if (!path)
		return (NULL);
	result = find_cmnd_path_helper(path, cmnd);
	if (!result)
		return (NULL);
	return (result);
}

int	check_cmd_path(t_ast *ast, char *path)
{
	struct stat	st;

	if (stat(path, &st) == 0)
	{
		if (S_ISDIR(st.st_mode))
		{
			write(2, "minishell: ", 11);
			write(2, ast->argv[0]->value, ft_strlen(ast->argv[0]));
			write(2, ": Is a directory\n", 17);
			return (126);
		}
		if (access(path, X_OK) == -1)
		{
			write(2, "minishell: ", 11);
			write(2, ast->argv[0]->value, ft_strlen(ast->argv[0]->value));
			write(2, ": Permission denied\n", 20);
			return (126);
		}
	}
	else
		return (handle_stat_error(cmd));
	return (0);
}

int	shell(t_ast *ast, t_expand_arg **args)
{
	pid_t child_pid;
	char **args;
	char *path;

	if (!ast || !args || args[0]->value[0] == '\0')
		return (0);
	path = find_path(ast, ast->argv[0]->value);
	if (!path)
	{
		is_not_found(args[0]);
		return (127);
	}
	if (check_cmd_path(ast, path) != 0)
		return (0);
	args = append_to_array(ast->exec->my_env);
	child_pid = fork();
	if (child_pid == -1)
		return (handle_fork_error());
	if (child_pid == 0)
		handle_child_process(cmnd, path, args, cmnd->args);
	handle_parent_process(0, cmnd);
	return (0);
}
