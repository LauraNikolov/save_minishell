/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicolof <lnicolof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 17:39:28 by lnicolof          #+#    #+#             */
/*   Updated: 2024/07/29 19:35:10 by lnicolof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	close_pipe_parent(t_cmd *cmd, t_ast *root)
{
	close(root->cmd->pipe[1]);
	if (root->cmd->prev_fd != -1)
		close(root->cmd->prev_fd);
	if (cmd->std_out != STDOUT_FILENO)
		root->cmd->prev_fd = root->cmd->pipe[0];
}

void	execve_pipe(t_cmd *cmd, char **envp)
{
	if (execve(cmd->path, cmd->cmd, envp) == -1)
		ft_parse_error(cmd);
	else
		exit(-1);
}

void	set_pipe_redir(t_cmd *cmd, t_ast *root)
{
	if (apply_redir(cmd) == -1)
	{
		if (cmd->std_in != STDIN_FILENO)
			close(cmd->std_in);
		if (cmd->std_out != STDOUT_FILENO)
			close(cmd->std_out);
		close(root->cmd->pipe[0]);
		close(root->cmd->pipe[1]);
		exit(1);
	}
	if (cmd->std_in != STDIN_FILENO)
	{
		dup2(cmd->std_in, STDIN_FILENO);
		close(cmd->std_in);
	}
	if (cmd->std_out != STDOUT_FILENO)
	{
		dup2(cmd->std_out, STDOUT_FILENO);
		close(cmd->std_out);
	}
}

int	ft_execve_pipe(t_cmd *cmd, char **envp, t_ast *root, t_save_struct *t_struct)
{
	int	return_value;

	return_value = 0;
	cmd->pid = fork();
	ft_signal(0);
	if (cmd->pid == -1)
		exit_error("fork failed\n", t_struct);
	if (cmd->pid == 0)
	{
		set_pipe_redir(cmd, root);
		close(root->cmd->pipe[0]);
		return_value = ft_dispatch_builtin(cmd, t_struct);
		if (return_value != -1)
			exit(return_value);
		else
			execve_pipe(cmd, envp);
	}
	else
		close_pipe_parent(cmd, root);
	return (return_value);
}
