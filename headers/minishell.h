/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 23:54:13 by ikarouat          #+#    #+#             */
/*   Updated: 2025/08/20 22:00:35 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "minishell_parser.h"
# include <signal.h>
# include <unistd.h>
# include <termios.h>

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
	t_env 			*my_env;
	char 			**env;
	int				exit_value;
	int				is_child;
	char			*key;
	char			*value;
	int				wait_input;
}					t_exec;

void				expand(t_ast *ast);
int					execute(t_ast *ast);
t_exec				init_env(char **envp);
t_env				*env_new(const char *key, const char *value);
void				add_back(t_env **env, t_env *to_add);
char				*ft_strjoin_free(char *s1, const char *s2);
char				**append_to_array(t_env *env, int env_or_export);
int					lst_size(t_env *env);
int					count_args(t_expand_arg **args);
int					get_env(t_env *env, const char *key);
char				**append_args(t_expand_arg **argv);
void				sort_env_array(char **arr);
char				*get_env_value(t_env *env, const char *key);
int					ft_env(t_ast *ast);
int					ft_unset(t_env **head, t_expand_arg **args);
int					ft_echo(t_ast *ast, t_expand_arg **args);
int					ft_pwd(t_expand_arg **args);
int					ft_cd(t_ast *ast, t_expand_arg **args);
int					ft_exit(t_ast *ast, t_expand_arg **args);
int					ft_export(t_ast *ast, t_expand_arg **args);
int					ft_external_cmds(t_ast *ast);
void				command_not_found(char *s);
void				update_env_value(t_env *env_list, const char *key,
						const char *value);
int 				ft_redirections(t_ast *ast, t_redirect *r);
void 				r_heredoc(t_redirect *r);
int					execute_command(t_ast *ast);
int					ft_pipeline(t_ast *ast);
int 				handle_child_status(pid_t pid);
void				sig_handler(int signal);
int					prepare_heredoc(t_redirect *r);
// To delete
void				print_ast(const char *direction, t_ast *ast, int depth);

#endif // MINISHELL_H