/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 18:58:37 by ikarouat          #+#    #+#             */
/*   Updated: 2025/08/20 05:19:37 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

//static char *token_type_to_str(t_token_type type)
//{
//    switch (type) {
//        case IS_WORD: return "WORD";
//        case IS_PIPE: return "PIPE";
//        case IS_REDIRECT_IN: return "REDIR_IN";
//        case IS_REDIRECT_OUT: return "REDIR_OUT";
//        case IS_REDIRECT_APPEND: return "REDIR_APPEND";
//        case IS_REDIRECT_HEREDOC: return "HEREDOC";
//        case IS_AND: return "AND";
//        case IS_OR: return "OR";
//        case IS_OPEN_BRACKET: return "OPEN_BRACKET";
//        case IS_CLOSE_BRACKET: return "CLOSE_BRACKET";
//        default: return "UNKNOWN";
//    }
//}
//
//// Convert segment state to string for debugging
//static char *segment_state_to_str(t_segment_state state)
//{
//    switch (state) {
//        case LITERAL: return "LITERAL";
//        case S_QUOTED: return "S_QUOTED";
//        case D_QUOTED: return "D_QUOTED";
//        default: return "UNKNOWN";
//    }
//}

// Print a visual representation of tokens and their segments
/*static void print_tokens(t_token *tokens)
{
    int token_count = 0;
    
    printf("\n=== TOKEN LIST ===\n");
    
    while (tokens) {
        token_count++;
        printf("\nToken %d: \"%s\"\n", token_count, tokens->value);
        printf("  Type: %s\n", token_type_to_str(tokens->type));
        printf("  Expandable: %s\n", tokens->expandable == EXPAND ? "YES" : "NO");
        
        // Print segments
        t_segment *seg = tokens->segments;
        int seg_count = 0;
        
        printf("  Segments:\n");
        if (!seg) {
            printf("    (No segments)\n");
        }
        
        while (seg) {
            seg_count++;
            printf("    %d. \"%s\" (%s)\n", 
                  seg_count, 
                  seg->seg_txt, 
                  segment_state_to_str(seg->state));
            seg = seg->next;
        }
        
        tokens = tokens->next;
    }
    
    printf("\nTotal tokens: %d\n", token_count);
    printf("=================\n\n");
}*/

t_token	*tokenize(char **line)
{
	t_token	*tokens;
	char	*trimmed_line;

	tokens = NULL;
	trimmed_line = ft_strtrim(*line, " \n\t\v\r\f");
	free(*line);
	line = &trimmed_line;
	init_tokens(&tokens, *line);
	//print_tokens(tokens);
	return (tokens);
}
