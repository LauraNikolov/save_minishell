/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicolof <lnicolof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 20:43:21 by lauranicolo       #+#    #+#             */
/*   Updated: 2024/07/22 10:55:23 by lnicolof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	open_redir_in(t_redir *current, int *fd_in, int *fd_out)
{
	(void)fd_out;
	*fd_in = open(current->next->redir, O_RDONLY);
	if (*fd_in == -1)
	{
		access(current->next->redir, R_OK);
		perror("minishell");
		return (-1);
	}
	return (0);
}

int	open_redir_rout(t_redir *current, int *fd_in, int *fd_out)
{
	(void)fd_in;
	*fd_out = open(current->next->redir, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (*fd_out == -1)
	{
		access(current->next->redir, R_OK);
		perror("minishell");
		return (-1);
	}
	return (0);
}

int	open_redir_rappend(t_redir *current, int *fd_in, int *fd_out)
{
	(void)fd_in;
	*fd_out = open(current->next->redir, O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (*fd_out == -1)
	{
		access(current->next->redir, R_OK);
		perror("minishell");
		return (-1);
	}
	return (0);
}

void	set_new_fd(t_cmd *cmd, int *fd_in, int *fd_out)
{
	if (*fd_in != -1)
	{
		if (cmd->std_in != 0)
			close(cmd->std_in);
		cmd->std_in = *fd_in;
	}
	if (*fd_out != -1)
	{
		if (cmd->std_in != 0)
			close(cmd->std_in);
		cmd->std_out = *fd_out;
	}
}

int	apply_redir(t_cmd *cmd)
{
	t_redir	*current;
	int		fd_in;
	int		fd_out;

	fd_in = -1;
	fd_out = -1;
	current = cmd->redir;
	while (current && current->next)
	{
		if (dispatch_redir(current, &fd_in, &fd_out) == -1)
			return (-1);
		if (current->next)
			current = current->next->next;
		else
			current = NULL;
	}
	set_new_fd(cmd, &fd_in, &fd_out);
	return (0);
}
