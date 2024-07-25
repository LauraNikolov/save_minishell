/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicolof <lnicolof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 18:33:30 by lnicolof          #+#    #+#             */
/*   Updated: 2024/07/23 12:41:57 by lnicolof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <errno.h>


t_cmd *get_last_cmd(t_ast *node) {
    if (node == NULL) {
        return NULL;
    }
    if (node->left == NULL && node->right == NULL) {
        return node->cmd;
    }
    if (node->right != NULL) {
        return get_last_cmd(node->right);
    }
    return get_last_cmd(node->left);
}


int get_return_code(t_cmd *cmd)
{
	int return_value;
	int status;

	return_value = 0;
	status = 0;
	waitpid(cmd->pid, &status, 0);
	if (WIFEXITED(status))
		return_value = WEXITSTATUS(status);
	while(wait(&status) > 0);
	return (return_value);
}

int	ft_pipe_recursive(t_ast *root, char **envp, int return_value, save_struct *t_struct)
{
	if (root->right->cmd->type == PIPE || root->right->cmd->type == AND
		|| root->right->cmd->type == OR)
	{
		root->right->cmd->prev_fd = root->left->cmd->prev_fd;
		return_value = exec_ast_recursive(root->right, envp,
				return_value, t_struct);
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

int	ft_pipe(t_ast *root, char **envp, int return_value,
		save_struct *t_struct)
{
	return (ft_pipe_recursive(root, envp, return_value, t_struct));
}

int	ft_and_recursive(t_ast *root, char **envp, int return_value, save_struct *t_struct)
{
	if (root->right->cmd->type == PIPE || root->right->cmd->type == AND
		|| root->right->cmd->type == OR)
	{
		if (return_value == 0)
			return_value = exec_ast_recursive(root->right, envp,
					return_value, t_struct);
		else
			return (return_value);
	}
	else
	{
		if (return_value == 0)
		{
			return_value = ft_execve_single_cmd(root->right->cmd, &envp, t_struct);
		}
		else
			return (return_value);
	}
	return (return_value);
}

int	ft_and(t_ast *root, char **envp, int return_value,
		save_struct *t_struct)
{
	return (ft_and_recursive(root, envp, return_value, t_struct));
}

int	ft_or_recursive(t_ast *root, char **envp,
		int return_value, save_struct *t_struct)
{
	if (root->right->cmd->type == PIPE || root->right->cmd->type == AND
		|| root->right->cmd->type == OR)
	{
		if (return_value != 0)
		{
			return_value = exec_ast_recursive(root->right, envp,
					return_value, t_struct);
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

int	ft_or(t_ast *root, char **envp, int return_value,
		save_struct *t_struct)
{
	return (ft_or_recursive(root, envp, return_value, t_struct));
}

void	ft_handle_ast_recursive(t_ast *root, char **envp, int *return_value, save_struct *t_struct)
{
	if(root->cmd->type == AND || root->cmd->type == OR)
	{
		if(root->left->cmd->type == WORD)
		{
			*return_value = ft_execve_single_cmd(root->left->cmd, &envp, t_struct);
		}	
	}

	if (root->cmd->type == AND)
	{
		if (*return_value == 0)
			*return_value = exec_ast_recursive(root->right, envp,
					*return_value, t_struct);
		else
			return ;
	}
	else if (root->cmd->type == OR)
	{
		if (*return_value != 0)
			*return_value = exec_ast_recursive(root->right, envp,
					*return_value, t_struct);
		else
			return ;
	}
	else
	{
		root->right->cmd->prev_fd = root->left->cmd->prev_fd;
		*return_value = exec_ast_recursive(root->right, envp,
				*return_value, t_struct);
	}
}

void	ft_handle_exec(t_ast *root, char **envp,
		int *return_value, save_struct *t_struct)
{
	if (root->cmd->type == PIPE)
	{
		if(pipe(root->cmd->pipe) == -1)
			exit_error("pipe failed\n", t_struct);
		root->right->cmd->std_in = root->left->cmd->prev_fd;
		if (root != t_struct->save_root->right)
			root->right->cmd->std_out = root->cmd->pipe[1];
		if (root == t_struct->save_root || root->parent->cmd->type == OR
			|| root->parent->cmd->type == AND)
			root->right->cmd->std_out = STDOUT_FILENO;
		*return_value = ft_execve_pipe(root->right->cmd, envp, root, t_struct);
		if(root == t_struct->save_root || root->parent->cmd->type == OR || root->parent->cmd->type == AND)
		{
			if(root->cmd->prev_fd != -1)
					close(root->cmd->prev_fd);
			*return_value = get_return_code(get_last_cmd(root));
		}
	}
	else if (root->cmd->type == AND)
	{
		if (*return_value == 0)
		{
			*return_value = ft_execve_single_cmd(root->right->cmd, &envp,
					t_struct);
		}
	}
	else if (root->cmd->type == OR)
	{
			if (*return_value != 0)
			{
				*return_value = ft_execve_single_cmd(root->right->cmd, &envp,
						t_struct);
			}
		}
}



int	exec_ast_recursive(t_ast *root, char **envp, int return_value, save_struct *t_struct)
{
	//print_ast(root, 0, '|');
	if (root == NULL)
		return (return_value);
	if (root->left->cmd->type == PIPE || root->left->cmd->type == AND
		|| root->left->cmd->type == OR)
		return_value = exec_ast_recursive(root->left, envp, return_value, t_struct);
	if (root->left->cmd->type == WORD && root->right->cmd->type == WORD)
		return_value = exec_leaf(root, envp, return_value, t_struct);
	else
	{
		if (root->right->cmd->type == PIPE || root->right->cmd->type == AND
			|| root->right->cmd->type == OR)
		{
			ft_handle_ast_recursive(root, envp, &return_value, t_struct);
		}
		else
		{	
			ft_handle_exec(root, envp, &return_value, t_struct);
		}
	}
	return (return_value);
}

