/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicolof <lnicolof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 16:05:37 by lnicolof          #+#    #+#             */
/*   Updated: 2024/07/31 19:53:32 by lnicolof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	wait_for_child(t_cmd *cmd, int *return_value)
{
	int	status;

	waitpid(cmd->pid, &status, 0);
	if (WIFEXITED(status))
		*return_value = WEXITSTATUS(status);
	return (*return_value);
}

void	manage_single_child(t_cmd *cmd, char ***envp)
{
	if (apply_redir(cmd) == -1)
		exit(1);
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
	if (cmd->cmd)
	{
		if (execve(cmd->path, cmd->cmd, *envp) == -1)
			ft_parse_error(cmd);
	}
	exit(-1);
}

static int	update_envp(char ***envp, t_save_struct *t_struct)
{
	char	**new_envp;

	new_envp = ft_envp_to_char(t_struct->envp);
	if (new_envp == NULL)
	{
		return (-1);
	}
	*envp = new_envp;
	return (0);
}

int	ft_execve_single_cmd(t_cmd *cmd, char ***envp, t_save_struct *t_struct)
{
	int	return_value;

	return_value = 0;
	if (is_it_builtin(cmd, &t_struct->envp, t_struct) == 1)
	{
		if (apply_redir(cmd) == -1)
			return (ft_return_code(ft_strdup("1"), &t_struct->envp));
		return_value = ft_dispatch_builtin(cmd, t_struct, 0);
		if (return_value != -1)
		{
			if (update_envp(envp, t_struct) == -1)
				return (exit_error("environnement expand failed\n", t_struct),
					0);
			return (return_value);
		}
	}
	cmd->pid = fork();
	ft_signal(0);
	if (cmd->pid == -1)
		exit_error("fork failed", t_struct);
	if (cmd->pid == 0)
		manage_single_child(cmd, envp);
	else
		wait_for_child(cmd, &return_value);
	return (return_value);
}
