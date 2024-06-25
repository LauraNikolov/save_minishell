/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicolof <lnicolof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 14:48:11 by lnicolof          #+#    #+#             */
/*   Updated: 2024/06/25 17:25:16 by lnicolof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int redir_in(t_cmd *cmd)
{
    int fd;
    char *last_redir;
    t_redir *current = cmd->redir;

    fd = -1;
    if(!cmd->redir)
        return(-1);
    else
    {
        while(current && current->next)
        {
            if(current->type == R_IN)
            { 
                fd = open(current->next->redir, O_WRONLY, 0644);
            }
            else if(current->type == R_HEREDOC)
                fd = open(current->next->redir, O_WRONLY, 0644);
            else
            {
                current = current->next;
                continue;
            }
            if(fd == -1)
            {
                perror("minishell");
                return(-1);
            }
            if(!current->next->next)
                break;
            else
                close(fd);
            last_redir = current->redir;
            current = current->next;
        }
    }
    return(fd);
}

int redir_out(t_cmd *cmd)
{
    int fd;
    char *last_redir;
    t_redir *current = cmd->redir;

    fd = -1;
    if(!cmd->redir)
        return(-1);
    else
    {
        while(current && current->next)
        {
            if(current->type == R_OUT)
            { 
                fd = open(current->next->redir, O_WRONLY | O_TRUNC | O_CREAT, 0644);
            }
            else if(current->type == R_APPEND)
                fd = open(current->next->redir, O_WRONLY| O_APPEND | O_CREAT, 0644);
            else
            {
                current = current->next;
                continue;
            }
            if(fd == -1)
            {
                perror("minishell");
                return(-1);
            }
            if(!current->next->next)
                break;
            else
                close(fd);
            last_redir = current->redir;
            current = current->next;
        }
    }
    return(fd);
}