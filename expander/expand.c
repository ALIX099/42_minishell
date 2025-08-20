/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikarouat <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 23:57:35 by ikarouat          #+#    #+#             */
/*   Updated: 2025/08/20 03:06:42 by ikarouat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int  match_pattern(char *pattern, const char *filename)
{
    char *star = ft_strchr(pattern, '*');
    
    if (!star) {
        // No wildcards, do exact match
        return ft_strcmp(pattern, filename) == 0;
    }
    
    // Check if beginning matches (before first star)
    size_t prefix_len = star - pattern;
    if (prefix_len > 0 && ft_strncmp(pattern, filename, prefix_len) != 0)
        return 0;
    
    // Move past the star
    pattern = star + 1;
    
    // If pattern ends with *, it matches anything after the prefix
    if (*pattern == '\0')
        return 1;
    
    // For simplicity, we'll only handle patterns with a single '*'
    // A more complete solution would handle multiple '*' characters
    
    // Check if the filename ends with the pattern after the '*'
    size_t filename_len = ft_strlen(filename);
    size_t suffix_len = ft_strlen(pattern);
    
    if (suffix_len > filename_len)
        return 0;
    
    return ft_strcmp(pattern, filename + (filename_len - suffix_len)) == 0;
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

char    *expand_variables_in_str(char *str)
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
        
        char *env_val = getenv(env_var);
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

void    expand_node(char **arg, t_segment *segments)
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
            expanded_segment = expand_variables_in_str(current->seg_txt);
        }
        else
        {
            env_expand_segment = expand_variables_in_str(current->seg_txt);
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

void	expand_redir_list(t_redirect **redirections)
{
	t_redirect	*tmp;

	tmp = *redirections;
	while (tmp)
	{
		if (tmp->type != REDIRECT_HEREDOC)
			expand_node(&(tmp->file->value), tmp->file->segments);
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
			
			expand_node(&ast->argv[i]->value, ast->argv[i]->segments);
			i++;
		}
		if (ast->redirects)
			expand_redir_list(&ast->redirects);
	}
	else
	{
		expand(ast->left);
		expand(ast->right);
		return ;
	}
}
