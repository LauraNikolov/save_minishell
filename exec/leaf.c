/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   leaf.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicolof <lnicolof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 18:11:34 by lnicolof          #+#    #+#             */
/*   Updated: 2024/07/29 16:45:04 by lnicolof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	last_pipe(t_ast *root, t_cmd *cmd2, int *return_value)
{
	if (root->cmd->prev_fd != -1)
		close(root->cmd->prev_fd);
	*return_value = get_return_code(cmd2);
}

void	leaf_stdout(t_cmd *cmd2, t_ast *root, t_ast *save_root)
{
	cmd2->std_in = root->cmd->prev_fd;
	if (root != save_root->right)
		root->right->cmd->std_out = root->cmd->pipe[1];
	if (root == save_root)
		cmd2->std_out = STDOUT_FILENO;
	if (root->parent)
	{
		if (root->parent->cmd->type == OR || root->parent->cmd->type == AND)
			cmd2->std_out = STDOUT_FILENO;
	}
}

void	exec_pipe_leaf(t_ast *root, char **envp, int *return_value,
		t_save_struct *t_struct)
{
	t_cmd	*cmd1;
	t_cmd	*cmd2;

	cmd1 = root->left->cmd;
	cmd2 = root->right->cmd;
	if (pipe(root->cmd->pipe) == -1)
		exit_error("pipe failed\n", t_struct);
	if (root->cmd->prev_fd == -1)
		cmd1->std_in = STDIN_FILENO;
	else
		cmd1->std_in = root->cmd->prev_fd;
	cmd1->std_out = root->cmd->pipe[1];
	*return_value = ft_execve_pipe(cmd1, envp, root, t_struct);
	if (pipe(root->cmd->pipe) == -1)
		exit_error("pipe failed\n", t_struct);
	leaf_stdout(cmd2, root, t_struct->save_root);
	*return_value = ft_execve_pipe(cmd2, envp, root, t_struct);
	if (root == t_struct->save_root || root->parent->cmd->type == OR
		|| root->parent->cmd->type == AND)
		last_pipe(root, cmd2, return_value);
}

void	and_or_leaf(t_ast *root, char **envp, t_save_struct *t_struct,
		int *return_value)
{
	if (root->cmd->type == AND)
	{
		*return_value = ft_execve_single_cmd(root->left->cmd, &envp, t_struct);
		if (*return_value == 0)
		{
			*return_value = ft_execve_single_cmd(root->right->cmd, &envp,
					t_struct);
		}
	}
	else if (root->cmd->type == OR)
	{
		*return_value = ft_execve_single_cmd(root->left->cmd, &envp, t_struct);
		if (*return_value != 0)
		{
			*return_value = ft_execve_single_cmd(root->right->cmd, &envp,
					t_struct);
		}
	}
}

int	exec_leaf(t_ast *root, char **envp, int return_value, t_save_struct *t_struct)
{
	if (root->cmd->type == WORD)
		return (0);
	if (root->left->cmd->type == WORD && root->right->cmd->type == WORD)
	{
		if (root->cmd->type == PIPE)
			exec_pipe_leaf(root, envp, &return_value, t_struct);
		else if (root->cmd->type == AND || root->cmd->type == OR)
			and_or_leaf(root, envp, t_struct, &return_value);
	}
	return (return_value);
}
