/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_parse.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicolof <lnicolof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 11:56:13 by lnicolof          #+#    #+#             */
/*   Updated: 2024/07/26 12:47:52 by lnicolof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>

static void	error_exec_str(char *str, char *cmd, int exit_code)
{
	ft_putstr_fd("minishell :", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(str, 2);
	exit(exit_code);
}

void	ft_parse_error(t_cmd *cmd)
{
	struct stat	buf;

	if (stat(cmd->cmd[0], &buf) != -1)
	{
		if (((S_ISDIR(buf.st_mode)) && (!ft_strncmp("./", cmd->cmd[0], 2)))
			|| (cmd->cmd[0][ft_strlen(cmd->cmd[0]) - 1] == '/'))
			error_exec_str("Is a directory", cmd->cmd[0], 126);
		if (!(buf.st_mode & S_IXUSR) || !(buf.st_mode & S_IRUSR)
			|| !S_ISLNK(buf.st_mode) || !S_ISDIR(buf.st_mode))
			error_exec_str("Permission denied", cmd->cmd[0], 126);
	}
	if (ft_strncmp("./", cmd->cmd[0], 2) == 0 || ft_strncmp("/", cmd->cmd[0],
			1) == 0)
		error_exec_str("No such file or directory", cmd->cmd[0], 127);
	else
		error_exec_str("command not found", cmd->cmd[0], 127);
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
		recursive_free_ast(&t_struct->ast);
	}
}
