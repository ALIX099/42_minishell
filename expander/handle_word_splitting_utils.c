/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_word_splitting_utils.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikarouat <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 04:12:19 by ikarouat          #+#    #+#             */
/*   Updated: 2025/08/21 07:15:08 by ikarouat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_expand_arg	**create_new_argv(int old_count, int word_count)
{
	return (malloc(sizeof(t_expand_arg *) * (old_count + word_count - 1 + 1)));
}

static void	copy_args_before(t_expand_arg **new_argv, t_expand_arg **old_argv,
		int pos)
{
	int	j;

	j = 0;
	while (j < pos)
	{
		new_argv[j] = old_argv[j];
		j++;
	}
}

static void	insert_split_words(t_expand_arg **new_argv, char **split_words,
		int pos, int count)
{
	int	j;

	j = 0;
	while (j < count)
	{
		new_argv[pos + j] = malloc(sizeof(t_expand_arg));
		new_argv[pos + j]->value = split_words[j];
		new_argv[pos + j]->segments = NULL;
		j++;
	}
}

int	process_split(t_ast *cmd_node, int i, char **split_words, int word_count)
{
	t_expand_arg	**new_argv;
	int				old_arg_count;
	int				j;

	old_arg_count = 0;
	while (cmd_node->argv[old_arg_count])
		old_arg_count++;
	new_argv = create_new_argv(old_arg_count, word_count);
	copy_args_before(new_argv, cmd_node->argv, i);
	insert_split_words(new_argv, split_words, i, word_count);
	j = i + 1;
	while (j < old_arg_count)
	{
		new_argv[j + word_count - 1] = cmd_node->argv[j];
		j++;
	}
	new_argv[old_arg_count + word_count - 1] = NULL;
	free(cmd_node->argv[i]->value);
	free(cmd_node->argv[i]);
	free(split_words);
	free(cmd_node->argv);
	cmd_node->argv = new_argv;
	return (i + word_count);
}
