/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 23:54:13 by ikarouat          #+#    #+#             */
/*   Updated: 2025/08/06 01:50:57 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "minishell_parser.h"
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <unistd.h>
#include <stdbool.h>

# define RED "\033[1;31m"
# define YELLOW "\033[1;33m"
# define GREEN "\033[1;32m"
# define CYAN "\033[1;36m"
# define RESET "\033[0m"

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

typedef struct s_exec
{
	t_env			*my_env;
	char			*pwd;
	bool			empty_env;
	int				exit_value;
}					t_exec;

void				expand(t_ast *ast);
int					execute(t_ast *ast);
t_exec				init_env(char **envp);
t_env				*env_new(const char *key, const char *value);
void				add_back(t_env **env, t_env *to_add);
// To delete
void				print_ast(const char *direction, t_ast *ast, int depth);

#endif // MINISHELL_H