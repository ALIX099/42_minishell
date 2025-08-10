/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_external.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 11:53:00 by macbookpro        #+#    #+#             */
/*   Updated: 2025/08/11 00:57:38 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

static char	*find_path(t_ast *ast)
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
			return (write(2, "rsh: ", 5), write(2, ast->argv[0]->value,
					ft_strlen(ast->argv[0]->value)), write(2,
					": Is a directory\n", 17), 126);
		if (access(path, X_OK) == -1)
			return (write(2, "rsh: ", 5), write(2, ": Permission denied\n", 20),
				126);
	}
	else
		return (perror(ast->argv[0]->value), 127);
	return (0);
}

int	ft_external_cmds(t_ast *ast)
{
	pid_t	pid;
	char	**args;
	char	*path;
	int		status;

	if (ast->argv[0]->value && !ast->argv[0]->value[0])
		return (0);
	path = find_path(ast);
	if (!path)
		return (command_not_found(ast->argv[0]->value), 127);
	status = check_command_errors(ast, path);
	if (status != 0)
		return (status);
	args = append_args(ast->argv);
	ast->exec->env = append_to_array(ast->exec->my_env, 0);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	if (pid == 0)
		return (execve(path, args, ast->exec->env), exit(126), 126);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}
