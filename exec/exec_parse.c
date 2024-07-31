/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_parse.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicolof <lnicolof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 11:56:13 by lnicolof          #+#    #+#             */
/*   Updated: 2024/08/01 00:13:09 by lnicolof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>

void	close_fds(t_cmd *cmd_list)
{
	t_cmd	*current;

	current = cmd_list;
	while (current)
	{
		if (current->std_out != 1)
		{
			close(current->std_out);
			current->std_out = 1;
		}
		if (current->std_in != 0)
		{
			close(current->std_in);
			current->std_in = 0;
		}
		if (current->type == R_HEREDOC)
			free(current->next->redir);
		current = current->next;
	}
}

void	destroy_tmp_file(t_cmd *cmd)
{
	t_cmd	*current;
	t_redir	*current_redir;

	current = cmd;
	while (current)
	{
		if (current->redir)
		{
			current_redir = current->redir;
			while (current_redir)
			{
				if (current_redir->type == R_HEREDOC)
					unlink(current_redir->next->redir);
				current_redir = current_redir->next->next;
			}
		}
		current = current->next;
	}
}

static void	ft_exec2(t_save_struct *t_struct, char **envp)
{
	manage_heredoc(t_struct->cmd, t_struct);
	if (g_exit_status != 2)
		ft_exec_multi_cmds(t_struct, envp);
	else
	{
		ft_return_code(ft_strdup("130"), &t_struct->envp);
		destroy_tmp_file(t_struct->cmd);
		g_exit_status = 0;
		return ;
	}
	close_fds(t_struct->cmd);
	destroy_tmp_file(t_struct->cmd);
	recursive_free_ast(&t_struct->ast);
}

void	ft_exec(t_save_struct *t_struct, char **envp)
{
	int	cmd_size;
	int	return_value;

	return_value = 0;
	cmd_size = ft_nbr_of_cmd(t_struct->cmd);
	if (cmd_size == 1)
	{
		t_struct->cmd->std_in = 0;
		t_struct->cmd->std_out = 1;
		manage_heredoc(t_struct->cmd, t_struct);
		if (g_exit_status != 2)
			return_value = ft_execve_single_cmd(t_struct->cmd, &envp, t_struct);
		else
		{
			ft_return_code(ft_strdup("130"), &t_struct->envp);
			destroy_tmp_file(t_struct->cmd);
			g_exit_status = 0;
			return ;
		}
		close_fds(t_struct->cmd);
		ft_return_code(ft_itoa(return_value), &t_struct->envp);
		destroy_tmp_file(t_struct->cmd);
	}
	else
		ft_exec2(t_struct, envp);
}
