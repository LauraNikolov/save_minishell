/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicolof <lnicolof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 13:32:24 by lnicolof          #+#    #+#             */
/*   Updated: 2024/07/29 16:45:04 by lnicolof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd	*get_last_cmd(t_ast *node)
{
	if (node == NULL)
	{
		return (NULL);
	}
	if (node->left == NULL && node->right == NULL)
	{
		return (node->cmd);
	}
	if (node->right != NULL)
	{
		return (get_last_cmd(node->right));
	}
	return (get_last_cmd(node->left));
}

int	get_return_code(t_cmd *cmd)
{
	int	return_value;
	int	status;

	return_value = 0;
	status = 0;
	waitpid(cmd->pid, &status, 0);
	if (WIFEXITED(status))
		return_value = WEXITSTATUS(status);
	while (wait(&status) > 0)
		;
	return (return_value);
}

int	ft_or(t_ast *root, char **envp, int return_value, t_save_struct *t_struct)
{
	return (ft_or_recursive(root, envp, return_value, t_struct));
}
