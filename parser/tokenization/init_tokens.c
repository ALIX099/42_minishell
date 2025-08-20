/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_tokens.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 17:20:30 by ikarouat          #+#    #+#             */
/*   Updated: 2025/08/20 05:19:40 by abouknan         ###   ########.fr       */
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

//static void	set_token(t_token *new_token, char *s, int start, int i)
//{
//	new_token->value = ft_strndup(s + start, i - start);
//	new_token->type = get_token_type(new_token->value);
//}

//static char	*append_line(char *dst, char *src)
//{
//	char	*new_buff;
//	int		i;
//	int		len;
//	char	*tmp;
//
//	if (!dst)
//		return (ft_strdup(src));
//	if (!src)
//		return (ft_strdup(dst));
//	len = ft_strlen(dst) + ft_strlen(src);
//	new_buff = malloc(len + 1);
//	if (!new_buff)
//		return (NULL);
//	i = 0;
//	tmp = (char *) dst;
//	while (*(tmp) != '\0')
//		new_buff[i++] = *(tmp++);
//	tmp = (char *) src;
//	while (*(tmp) != '\0')
//		new_buff[i++] = *(tmp++);
//	new_buff[i] = '\0';
//	return (new_buff);
//}

static t_segment *create_segment(char *text, t_segment_state type)
{
    t_segment *segment = malloc(sizeof(t_segment));
    if (!segment)
        return NULL;
    
    segment->seg_txt = text;
    segment->state = type;
    if (type == S_QUOTED)
        segment->expandable = NO_EXPAND;
    else
        segment->expandable = EXPAND;
    segment->next = NULL;
    return segment;
}

static void add_segment(t_segment **head, t_segment *new_segment)
{
    if (!*head)
        *head = new_segment;
    else
    {
        t_segment *current = *head;
        while (current->next)
            current = current->next;
        current->next = new_segment;
    }
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
    if (!found) {
        write(2, "rsh: Unclosed quote\n", 20);
        *out = NULL;
        return (-1);
    }
    *out = ft_strndup(*s + start, i - start);
    return i + 1;
}

static int process_token_segments(t_token *token, char *s, int i)
{
    int start = i;
    token->segments = NULL;
    token->expandable = EXPAND; // Default to expandable
    
    while (s[i] && !ft_isspace(s[i]) && 
           // Only check for special chars if NOT inside quotes
           (!is_special_char("()<>|&", s[i]) || 
            s[start] == '\'' || s[start] == '"'))
    {
        if (s[i] == '\'' || s[i] == '"')
        {
            // Handle quoted part
            char quote = s[i];
            char *quoted_content;
            int next_pos = read_until_quote(&s, i, quote, &quoted_content);
            
            if (next_pos < 0)
                return -1;
                
            // Add quoted segment
            t_segment_state seg_type = (quote == '\'') ? S_QUOTED : D_QUOTED;
            add_segment(&token->segments, create_segment(quoted_content, seg_type));
            
            // If single-quoted, mark as not expandable
            if (quote == '\'')
                token->expandable = NO_EXPAND;
                
            i = next_pos;
        }
        else
        {
            // Handle unquoted part
            int plain_start = i;
            
            // Read until quote or space or special char (if not already in a quoted token)
            while (s[i] && !ft_isspace(s[i]) && 
                  ((!is_special_char("()<>|&", s[i]) || s[start] == '\'' || s[start] == '"')) && 
                  s[i] != '\'' && s[i] != '"')
                i++;
                
            char *plain_text = ft_strndup(s + plain_start, i - plain_start);
            add_segment(&token->segments, create_segment(plain_text, LITERAL));
        }
        
        // Break if we hit a special char and this isn't a quoted token
        if (s[i] && is_special_char("()<>|&", s[i]) && 
           s[start] != '\'' && s[start] != '"')
            break;
    }
    
    // Rebuild full token value from segments
    token->value = ft_strdup("");
    t_segment *seg = token->segments;
    while (seg)
    {
        char *temp = token->value;
        token->value = ft_strjoin(token->value, seg->seg_txt);
        free(temp);
        seg = seg->next;
    }
    
    token->type = IS_WORD; // Force to WORD type for quoted special chars
    return i - start;
}

static void str_to_tokens(t_token **tokens, char *s)
{
    t_token *new_token;
    int     i = 0, start;

    if (!s || !*s)
        return;
        
    new_token = malloc(sizeof(t_token));
    if (!new_token)
        return;
        
    ft_tokenlist_add_back(tokens, &new_token);
    
    while (s[i] && ft_isspace(s[i]))
        i++;
    start = i;
    
    // Check if token starts with a quote
    if (s[i] == '\'' || s[i] == '"')
    {
        // Process a token that starts with quotes (may contain special chars)
        int result = process_token_segments(new_token, s, start);
        if (result < 0)
        {
            *tokens = NULL; // Error occurred
            return;
        }
        i = start + result;
    }
    // Check if it's an unquoted special character
    else if (is_special_char("()<>|&", s[i]))
    {
        // Handle special token
        new_token->value = extract_token(s + i, &i);
        new_token->type = get_token_type(new_token->value);
        
        // Create a single segment for special token
        new_token->segments = NULL;
        add_segment(&new_token->segments, 
                   create_segment(ft_strdup(new_token->value), LITERAL));
    }
    else
    {
        // Process regular word token with potential mixed quotes
        int result = process_token_segments(new_token, s, start);
        if (result < 0)
        {
            *tokens = NULL; // Error occurred
            return;
        }
        i = start + result;
    }
    
    str_to_tokens(tokens, s + i);
}

//
//static void	str_to_tokens(t_token **tokens, char *s)
//{
//    t_token	*new_token;
//    int		i = 0, start;
//
//    if (!s || !*s)
//        return;
//    new_token = malloc(sizeof(t_token));
//    ft_tokenlist_add_back(tokens, &new_token);
//	new_token->expandable = EXPAND;
//    while (s[i] && ft_isspace(s[i]))
//        i++;
//    start = i;
//    if (s[i] == '\'' || s[i] == '"') {
//		if (s[i] == '\'')
//			new_token->expandable = NO_EXPAND;
//        int next = read_until_quote(&s, i, s[i], &new_token->value);
//        if (next < 0)
//        {
//            //ToDo:free_list(tokens);
//			*tokens = NULL;
//            return ;
//        }
//        new_token->type = IS_WORD;
//        str_to_tokens(tokens, s + next);
//        return;
//    }
//    while (s[i] && !is_special_char("()<>|&", s[i]) && !ft_isspace(s[i]) && s[i] != '\'' && s[i] != '"')
//        i++;
//    if (i > start)
//        set_token(new_token, s, start, i);
//    else if (s[i] && is_special_char("()<>|&", s[i]))
//    {
//        new_token->value = extract_token(s + i, &i);
//        new_token->type = get_token_type(new_token->value);
//    }
//    str_to_tokens(tokens, s + i);
//}

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

/*
static char *read_word(char **s)
{
    char    *result = ft_strdup("");
    int     i = 0;

    while ((*s)[i] && !ft_isspace((*s)[i]) && !is_special_char("()<>|&", (*s)[i]))
    {
        if ((*s)[i] == '"' || (*s)[i] == '\'')
        {
            char quote = (*s)[i];
            char *segment;
            int next = read_until_quote(s, i, quote, &segment);

            if (next < 0)
            {
                free(result);
                return (NULL); // unterminated quote
            }

            // append quoted contents
            result = append_line(result, segment);
            free(segment);

            i = next; // skip past closing quote
        }
        else
        {
            char buf[2] = { (*s)[i], '\0' };
            result = append_line(result, buf);
            i++;
        }
    }
    *s += i;
    return result;
}

static void str_to_tokens(t_token **tokens, char *s)
{
    while (*s)
    {
        while (*s && ft_isspace(*s))
            s++;

        if (!*s)
            break;

        t_token *new_token = malloc(sizeof(t_token));
        new_token->next = NULL;
        new_token->expandable = EXPAND;
        new_token->segments = NULL;
        if (is_special_char("()<>|&", *s))
        {
            new_token->value = extract_token(s, (int[]){0}); // dummy i_ptr
            new_token->type = get_token_type(new_token->value);
            s += ft_strlen(new_token->value);
        }
        else
        {
            new_token->value = read_word(&s);
            new_token->type = get_token_type(new_token->value);

            // if the word came from single quotes, mark NO_EXPAND
            if (new_token->value && new_token->value[0] == '\'' 
                && new_token->value[ft_strlen(new_token->value) - 1] == '\'')
                new_token->expandable = NO_EXPAND;
        }
        ft_tokenlist_add_back(tokens, &new_token);
    }
}
*/