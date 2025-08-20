/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   r_heredoc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikarouat <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 03:32:35 by ikarouat          #+#    #+#             */
/*   Updated: 2025/08/20 17:36:35 by ikarouat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Helper function to read the heredoc content
static void read_heredoc(t_heredoc *heredoc)
{
    char *line;
    char *content = ft_strdup("");
    char *temp;
    
    // Prompt user for input until delimiter is entered
    while (1)
    {
        line = readline("> ");
        if (!line || ft_strcmp(line, heredoc->delimeter) == 0)
        {
            free(line);
            break;
        }
        
        // Append line to content with newline
        temp = content;
        content = ft_strjoin(content, line);
        free(temp);
        
        temp = content;
        content = ft_strjoin(content, "\n");
        free(temp);
        
        free(line);
    }
    
    heredoc->raw_body = content;
}

int r_heredoc(t_ast *ast, t_redirect *r)
{
    int pipe_fd[2];
    pid_t pid;
    int status;
    
    // First, read the heredoc content
    read_heredoc(r->heredoc);
    
    // Create pipe
    if (pipe(pipe_fd) < 0)
        return (perror("pipe"), 1);
    
    pid = fork();
    if (pid < 0)
        return (perror("fork"), 1);
    
    if (pid == 0)
    {
        // Child process
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        
        // Close read end of pipe
        close(pipe_fd[0]);
        
        // Redirect input from pipe
        if (dup2(pipe_fd[1], STDOUT_FILENO) < 0)
            exit(1);
        
        // Write heredoc content to pipe
        if (r->heredoc->raw_body)
            write(STDOUT_FILENO, r->heredoc->raw_body, 
                  ft_strlen(r->heredoc->raw_body));
        
        close(pipe_fd[1]);
        exit(0);
    }
    
    // Parent process
    close(pipe_fd[1]);
    
    // Create another fork for the command
    pid = fork();
    if (pid < 0)
        return (perror("fork"), 1);
    
    if (pid == 0)
    {
        // Redirect stdin from pipe
        if (dup2(pipe_fd[0], STDIN_FILENO) < 0)
            exit(1);
        
        close(pipe_fd[0]);
        exit(execute_command(ast));
    }
    
    // Parent process
    close(pipe_fd[0]);
    waitpid(0, &status, 0);
    
    return WEXITSTATUS(status);
}
