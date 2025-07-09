/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_tokens.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikarouat <ikarouat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 17:20:30 by ikarouat          #+#    #+#             */
/*   Updated: 2025/07/09 21:43:03 by ikarouat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

static t_token_type	get_token_type(char *s)
{
	if (ft_strcmp(s, ">>") == 0)
		return (IS_REDIRECT_APPEND);
	else if (ft_strcmp(s, "<<") == 0)
		return (IS_REDIRECT_HEREDOC);
	else if (ft_strcmp(s, "|") == 0)
		return (IS_PIPE);
	else if (ft_strcmp(s, "<") == 0)
		return (IS_REDIRECT_IN);
	else if (ft_strcmp(s, ">") == 0)
		return (IS_REDIRECT_OUT);
	else if (ft_strcmp(s, "||") == 0)
		return (IS_OR);
	else if (ft_strcmp(s, "&&") == 0)
		return (IS_AND);
	else if (ft_strcmp(s, "(") == 0)
		return (IS_OPEN_BRACKET);
	else if (ft_strcmp(s, ")") == 0)
		return (IS_CLOSE_BRACKET);
	else
		return (IS_WORD);
}

static int	is_special_char(const char *s, int c)
{
	if (ft_strchr(s, c))
		return (1);
	return (0);
}

static void	set_token(t_token *new_token, char *s, int start, int i)
{
	new_token->value = ft_strndup(s + start, i - start);
	new_token->type = get_token_type(new_token->value);
}

static char	*append_line(char *dst, char *src)
{
	char	*new_buff;
	int		i;
	int		len;
	char	*tmp;

	if (!dst)
		return (ft_strdup(src));
	if (!src)
		return (ft_strdup(dst));
	len = ft_strlen(dst) + ft_strlen(src);
	new_buff = malloc(len + 1);
	if (!new_buff)
		return (NULL);
	i = 0;
	tmp = (char *) dst;
	while (*(tmp) != '\0')
		new_buff[i++] = *(tmp++);
	tmp = (char *) src;
	while (*(tmp) != '\0')
		new_buff[i++] = *(tmp++);
	new_buff[i] = '\0';
	return (new_buff);
}

static int	read_until_quote(char **s, int i, char quote, char **out)
{
    int start = i + 1;
    int found = 0;

    while ((*s)[++i]) {
        if ((*s)[i] == quote) {
            found = 1;
            break;
        }
    }
    while (!found) {
        char *more = readline("QUOTE> ");
        if (!more)
            return -1;
        *s = append_line(*s, more);
        while ((*s)[i]) {
            if ((*s)[i] == quote) {
                found = 1;
                break;
            }
            i++;
        }
    }
    *out = ft_strndup(*s + start, i - start);
    return i + 1;
}

static void	str_to_tokens(t_token **tokens, char *s)
{
    t_token	*new_token;
    int		i = 0, start;

    if (!s || !*s)
        return;
    new_token = malloc(sizeof(t_token));
    ft_tokenlist_add_back(tokens, &new_token);
    while (s[i] && ft_isspace(s[i]))
        i++;
    start = i;
    if (s[i] == '\'' || s[i] == '"') {
        int next = read_until_quote(&s, i, s[i], &new_token->value);
        if (next < 0)
			return;
        new_token->type = IS_WORD;
        str_to_tokens(tokens, s + next);
        return;
    }
    while (s[i] && !is_special_char("()<>|&", s[i]) && !ft_isspace(s[i]) && s[i] != '\'' && s[i] != '"')
        i++;
    if (i > start)
        set_token(new_token, s, start, i);
    else if (s[i] && is_special_char("()<>|&", s[i]))
    {
        new_token->value = extract_token(s + i, &i);
        new_token->type = get_token_type(new_token->value);
    }
    str_to_tokens(tokens, s + i);
}

void	init_tokens(t_token **tokens, char *str)
{
	unsigned int	i;

	i = 0;
	while (ft_isspace(str[i]))
		i++;
	if (!str[i])
		return;		
	str_to_tokens(tokens, str + i);
}
