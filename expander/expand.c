/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 23:57:35 by ikarouat          #+#    #+#             */
/*   Updated: 2025/08/20 20:56:05 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_word_splitting(t_ast *cmd_node)
{
    if (!cmd_node || !cmd_node->argv)
        return;
        
    int i = 0;
    while (cmd_node->argv[i])
    {
        char *value = cmd_node->argv[i]->value;
        t_segment *segments = cmd_node->argv[i]->segments;
        
        // Only split if:
        // 1. The value contains spaces
        // 2. The spaces came from wildcard expansion (not quoted text)
        // 3. There's at least one LITERAL segment with a wildcard
        
        if (!ft_strchr(value, ' '))
        {
            i++;
            continue;
        }
        
        // Check if this arg had a wildcard in a LITERAL segment
        int had_wildcard = 0;
        t_segment *seg = segments;
        while (seg)
        {
            if (seg->state == LITERAL && ft_strchr(seg->seg_txt, '*'))
            {
                had_wildcard = 1;
                break;
            }
            seg = seg->next;
        }
        
        // Only split if it had an unquoted wildcard
        if (!had_wildcard)
        {
            i++;
            continue;
        }
        
        // Now split the expanded value
        char **split_words = ft_split(value, ' ');
        int word_count = 0;
        while (split_words[word_count])
            word_count++;
            
        if (word_count <= 1)
        {
            // No actual splitting needed
            for (int j = 0; split_words[j]; j++)
                free(split_words[j]);
            free(split_words);
            i++;
            continue;
        }
        
        // Count current arguments
        int old_arg_count = 0;
        while (cmd_node->argv[old_arg_count])
            old_arg_count++;
            
        // Create new argv array with space for the split words
        t_expand_arg **new_argv = malloc(sizeof(t_expand_arg *) * 
                                        (old_arg_count + word_count - 1 + 1)); // +1 for NULL
        
        // Copy arguments before current
        int j;
        for (j = 0; j < i; j++)
            new_argv[j] = cmd_node->argv[j];
            
        // Insert split words as new arguments
        for (j = 0; j < word_count; j++)
        {
            new_argv[i+j] = malloc(sizeof(t_expand_arg));
            new_argv[i+j]->value = split_words[j]; // Take ownership
            new_argv[i+j]->segments = NULL; // New args don't need segments
        }
        
        // Copy remaining arguments
        for (j = i+1; j < old_arg_count; j++)
            new_argv[j+word_count-1] = cmd_node->argv[j];
            
        // Null-terminate the new array
        new_argv[old_arg_count+word_count-1] = NULL;
        
        // Free original argument (but not its segments, we're done with them)
        free(cmd_node->argv[i]->value);
        free(cmd_node->argv[i]);
        
        // Free split array (but not the strings, they're now owned by new_argv)
        free(split_words);
        
        // Free old argv array
        free(cmd_node->argv);
        
        // Set new argv array
        cmd_node->argv = new_argv;
        
        // Skip past all the newly inserted arguments
        i += word_count;
    }
}

static int match_pattern(const char *pattern, const char *filename)
{
    // Base case: end of pattern
    if (*pattern == '\0')
        return *filename == '\0';  // Match if also end of filename
        
    // Case 1: Wildcard '*'
    if (*pattern == '*')
    {
        // Skip consecutive asterisks
        while (*(pattern + 1) == '*')
            pattern++;
            
        // Try matching with '*' consuming zero or more characters
        for (size_t i = 0; i <= ft_strlen(filename); i++)
        {
            if (match_pattern(pattern + 1, filename + i))
                return 1;
        }
        
        return 0;
    }
    
    // Case 2: Regular character match
    if (*filename != '\0' && (*pattern == *filename || *pattern == '?'))
        return match_pattern(pattern + 1, filename + 1);
        
    // No match
    return 0;
}

char    *expand_wildcard(char *str)
{
    DIR             *dir;
    struct dirent   *entry;
    char            *result;
    int             matches;
    
    if (!ft_strchr(str, '*'))
        return (str);
    dir = opendir(".");
    if (!dir)
        return ft_strdup(str);
    result = ft_strdup("");
    matches = 0;
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] == '.')
            continue;
        
        // Check if this file matches our pattern
        if (match_pattern(str, entry->d_name)) {
            // Add space between multiple matches
            if (matches > 0) {
                char *temp = result;
                result = ft_strjoin(result, " ");
                free(temp);
            }
            
            // Add the matching filename to result
            char *temp = result;
            result = ft_strjoin(result, entry->d_name);
            free(temp);
            matches++;
        }
    }
    closedir(dir);
    if (matches == 0)
        return (free(result), ft_strdup(str));
    return (result);
}

char    *expand_variables_in_str(char *str, t_exec *data)
{
    char *result = ft_strdup(str);
    char *dollar;
    
    // Find and expand all variables in the string
    while ((dollar = strchr(result, '$')) != NULL)
    {
        if (!ft_isalnum(*(dollar + 1)) && *(dollar + 1) != '_')
            break;
            
        size_t var_len = 0;
        size_t prefix_len = dollar - result;
        
        while (ft_isalnum((dollar + 1)[var_len]) || (dollar + 1)[var_len] == '_')
            var_len++;
            
        char *env_var = malloc(var_len + 1);
        ft_strlcpy(env_var, (dollar + 1), var_len + 1);
        
        char *env_val = get_env_value(data->my_env, env_var);
        if (!env_val)
            env_val = "";
            
        size_t new_len = prefix_len + ft_strlen(env_val) + 
                         ft_strlen(dollar + var_len + 1) + 1;
                         
        char *new_result = malloc(new_len);
        ft_strlcpy(new_result, result, prefix_len + 1);
        ft_strlcat(new_result, env_val, new_len);
        ft_strlcat(new_result, dollar + var_len + 1, new_len);
        
        free(env_var);
        free(result);
        result = new_result;
    }
    
    return result;
}

void    expand_heredoc(t_heredoc *heredoc, t_exec *data)
{
    char    *expanded;

    if (!heredoc->raw_body || heredoc->quoted)
        return ;
    expanded = expand_variables_in_str(heredoc->raw_body, data);
    free(heredoc->raw_body);
    heredoc->raw_body = expanded;
}

void    expand_node(char **arg, t_segment *segments, t_exec *data)
{
	char        *result;
    char        *expanded_segment;
    char        *env_expand_segment;
    t_segment   *current;
    
    if (!*arg || !segments)
        return;
        
    result = ft_strdup("");
    current = segments;

    while (current)
    {
        // Handle segment based on its type
        if (current->state == S_QUOTED)
        {
            // Single quotes - no expansion
            expanded_segment = ft_strdup(current->seg_txt);
        }
        else if (current->state == D_QUOTED)
        {
            expanded_segment = expand_variables_in_str(current->seg_txt, data);
        }
        else
        {
            env_expand_segment = expand_variables_in_str(current->seg_txt, data);
            expanded_segment = expand_wildcard(env_expand_segment);
        }
        // Append expanded segment to result
        char *temp = result;
        result = ft_strjoin(result, expanded_segment);
        free(temp);
        free(expanded_segment);
        
        current = current->next;
    }
    
    // Replace original arg with concatenated result
    free(*arg);
    *arg = result;
}

void	expand_redir_list(t_redirect **redirections, t_exec *data)
{
	t_redirect	*tmp;

	tmp = *redirections;
	while (tmp)
	{
		if (tmp->type != REDIRECT_HEREDOC)
			expand_node(&(tmp->file->value), tmp->file->segments, data);
        else if (tmp->type == REDIRECT_HEREDOC && !(tmp->heredoc->quoted))
            expand_heredoc(tmp->heredoc, data);
		tmp = tmp->next;
	}
	return ;
}

void	expand(t_ast *ast)
{
	int	i;

	if (!ast)
		return ;
	if (ast->type == NODE_CMD)
	{
		i = 0;
		while (ast->argv[i])
		{
			
			expand_node(&ast->argv[i]->value, ast->argv[i]->segments, ast->exec);
            handle_word_splitting(ast);
			i++;
		}
		if (ast->redirects)
			expand_redir_list(&ast->redirects, ast->exec);
	}
	else
	{
		expand(ast->left);
		expand(ast->right);
		return ;
	}
}