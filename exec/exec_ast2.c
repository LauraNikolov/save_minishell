/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicolof <lnicolof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 17:08:30 by lnicolof          #+#    #+#             */
/*   Updated: 2024/07/29 16:45:04 by lnicolof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_pipe_recursive(t_ast *root, char **envp, int return_value,
		t_save_struct *t_struct)
{
	if (root->right->cmd->type == PIPE || root->right->cmd->type == AND
		|| root->right->cmd->type == OR)
	{
		root->right->cmd->prev_fd = root->left->cmd->prev_fd;
		return_value = exec_ast_recursive(root->right, envp, return_value,
				t_struct);
	}
	else
	{
		pipe(root->cmd->pipe);
		root->right->cmd->std_in = root->left->cmd->prev_fd;
		if (root != t_struct->save_root->right)
			root->right->cmd->std_out = root->cmd->pipe[1];
		if (root == t_struct->save_root)
			root->right->cmd->std_out = STDOUT_FILENO;
		return_value = ft_execve_pipe(root->right->cmd, envp, root, t_struct);
	}
	return (return_value);
}

int	ft_pipe(t_ast *root, char **envp, int return_value, t_save_struct *t_struct)
{
	return (ft_pipe_recursive(root, envp, return_value, t_struct));
}

int	ft_and_recursive(t_ast *root, char **envp, int return_value,
		t_save_struct *t_struct)
{
	if (root->right->cmd->type == PIPE || root->right->cmd->type == AND
		|| root->right->cmd->type == OR)
	{
		if (return_value == 0)
			return_value = exec_ast_recursive(root->right, envp, return_value,
					t_struct);
		else
			return (return_value);
	}
	else
	{
		if (return_value == 0)
		{
			return_value = ft_execve_single_cmd(root->right->cmd, &envp,
					t_struct);
		}
		else
			return (return_value);
	}
	return (return_value);
}

int	ft_and(t_ast *root, char **envp, int return_value, t_save_struct *t_struct)
{
	return (ft_and_recursive(root, envp, return_value, t_struct));
}

int	ft_or_recursive(t_ast *root, char **envp, int return_value,
		t_save_struct *t_struct)
{
	if (root->right->cmd->type == PIPE || root->right->cmd->type == AND
		|| root->right->cmd->type == OR)
	{
		if (return_value != 0)
		{
			return_value = exec_ast_recursive(root->right, envp, return_value,
					t_struct);
		}
		else
			return (return_value);
	}
	else
	{
		if (return_value != 0)
		{
			return_value = ft_execve_single_cmd(root->right->cmd, &envp,
					t_struct);
		}
		else
			return (return_value);
	}
	return (return_value);
}
