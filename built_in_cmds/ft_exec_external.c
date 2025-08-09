/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_external.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 11:53:00 by macbookpro        #+#    #+#             */
/*   Updated: 2025/08/10 00:50:48 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

static char	*find_command_path(t_ast *ast)
{
	char	*path_env;
	char	**paths;
	char	*full_path;
	int		i;

	if (ast->argv[0]->value[0] == '/' || ast->argv[0]->value[0] == '.')
		return (ft_strdup(ast->argv[0]->value));
	path_env = get_env_value(ast->exec->my_env, "PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);

	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin_free(full_path, ast->argv[0]->value);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

static int	check_command_errors(t_ast *ast, char *path)
{
	struct stat	st;

	if (stat(path, &st) == 0)
	{
		if (S_ISDIR(st.st_mode))
		{
			write(2, "rsh: ", 5);
			write(2, ast->argv[0]->value, ft_strlen(ast->argv[0]->value));
			write(2, ": Is a directory\n", 17);
			return (126);
		}
		if (access(path, X_OK) == -1)
		{
			write(2, "rsh: ", 5);
			write(2, ast->argv[0]->value, ft_strlen(ast->argv[0]->value));
			write(2, ": Permission denied\n", 20);
			return (126);
		}
	}
	else
	{
		perror(ast->argv[0]->value);
		return (127);
	}
	return (0);
}

int	exec_external(t_ast *ast)
{
	pid_t	pid;
	char	**args;
	char	*path;
	int		status;

	path = find_command_path(ast);
	if (!path)
	{
		write(2, "rsh: ", 5);
		write(2, ast->argv[0]->value, ft_strlen(ast->argv[0]->value));
		write(2, ": command not found\n", 21);
		return (127);
	}
	if ((status = check_command_errors(ast, path)) != 0)
		return (status);

	args = expand_args_to_array(ast->argv);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	if (pid == 0)
	{
		execve(path, args, ast->exec->env);
		perror("execve");
		_exit(126);
	}
	waitpid(pid, &status, 0);
	return (WIFEXITED(status) ? WEXITSTATUS(status) : 1);
}

