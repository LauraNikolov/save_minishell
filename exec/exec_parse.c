/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_parse.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicolof <lnicolof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 11:56:13 by lnicolof          #+#    #+#             */
/*   Updated: 2024/07/23 12:25:06 by lnicolof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <errno.h>
#include <sys/wait.h>

void	manage_err_dir(t_cmd *cmd)
{
	struct stat	path_stat;

	if (stat(cmd->cmd[0], &path_stat) == -1)
	{
		if (strchr(cmd->cmd[0], '/'))
		{
			dprintf(2, "%s: command not found\n", cmd->cmd[0]);
			exit(127);
		}
		else
		{
			dprintf(2, "minishell: %s: No such file or directory\n",
				cmd->cmd[0]);
			exit(127);
		}
	}
	if (S_ISDIR(path_stat.st_mode))
	{
		if (!strchr(cmd->cmd[0], '/'))
		{
			dprintf(2, "%s: command not found\n", cmd->cmd[0]);
			exit(127);
		}
		dprintf(2, "minishell: %s: Is a directory\n", cmd->cmd[0]);
		exit(126);
	}
	if (access(cmd->cmd[0], X_OK) == -1)
	{
		if (errno == EACCES)
		{
			dprintf(2, "minishell: %s: Permission denied\n", cmd->cmd[0]);
			exit(126);
		}
		else
		{
			dprintf(2, "minishell: %s: Error\n", cmd->cmd[0]);
			exit(1);
		}
	}
}

void	ft_parse_error(t_cmd *cmd)
{
	manage_err_dir(cmd);
	if (ft_strchr(cmd->cmd[0], '/') == -1)
	{
		dprintf(2, "%s: command not found\n", cmd->cmd[0]);
		exit(127);
	}
	if (ft_strchr(cmd->cmd[0], '.'))
	{
		dprintf(2, "minishell: %s: No such file or directory\n", cmd->cmd[0]);
		exit(126);
	}
}

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

void	ft_exec(save_struct *t_struct, char **envp)
{
	int	cmd_size;
	int	return_value;

	cmd_size = ft_nbr_of_cmd(t_struct->cmd);
	if (cmd_size == 1)
	{
		t_struct->cmd->std_in = 0;
		t_struct->cmd->std_out = 1;
		manage_heredoc(t_struct->cmd, t_struct);
		return_value = ft_execve_single_cmd(t_struct->cmd, &envp, t_struct);
		close_fds(t_struct->cmd);
		ft_return_code(ft_itoa(return_value), &t_struct->envp);
		destroy_tmp_file(t_struct->cmd);
	}
	else
	{
		manage_heredoc(t_struct->cmd, t_struct);
		ft_exec_multi_cmds(t_struct, envp);
		close_fds(t_struct->cmd);
		destroy_tmp_file(t_struct->cmd);
		recursive_free_ast(t_struct->ast);
	}
}
