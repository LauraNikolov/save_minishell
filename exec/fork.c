/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicolof <lnicolof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 12:59:48 by lnicolof          #+#    #+#             */
/*   Updated: 2024/07/29 16:45:04 by lnicolof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_exec_multi_cmds(t_save_struct *t_struct, char **envp)
{
	t_cmd	*start;
	t_cmd	*end;
	int		return_value;

	return_value = -1;
	start = t_struct->cmd;
	while (t_struct->cmd->next)
		t_struct->cmd = t_struct->cmd->next;
	end = t_struct->cmd;
	t_struct->cmd = start;
	t_struct->ast = build_ast_recursive(start, end, NULL, t_struct);
	start = t_struct->cmd;
	while (start)
	{
		start->std_in = STDIN_FILENO;
		start->std_out = STDOUT_FILENO;
		start->prev_fd = -1;
		start->return_value = 0;
		start = start->next;
	}
	t_struct->save_root = t_struct->ast;
	return_value = exec_ast_recursive(t_struct->ast, envp, return_value,
			t_struct);
	if (ft_return_code(ft_itoa(return_value), &t_struct->envp) == -1)
		exit_error("malloc failed\n", t_struct);
}
