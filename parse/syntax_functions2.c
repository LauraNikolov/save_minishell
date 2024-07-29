/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_functions2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicolof <lnicolof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 00:30:42 by renard            #+#    #+#             */
/*   Updated: 2024/07/29 19:30:15 by lnicolof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_check_redir2(t_cmd *node, t_envp **env)
{
	t_redir	*tmp;

	tmp = node->redir;
	while (tmp->next)
	{
		if ((tmp->type >= 6 && tmp->type <= 9) && (!tmp->next
				|| (tmp->next->type >= 6 && tmp->next->type <= 9)
				|| tmp->next->type != WORD || !tmp->next->redir[0]))
		{
			if (!(tmp->next->type >= 6 && tmp->next->type <= 9))
				ft_putstr_fd("syntax error near unexpected token `newline'", 2);
			else
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `",
					2);
				ft_putstr_fd(tmp->next->redir, 2);
			}
			return (ft_return_code(ft_strdup("2"), env), -1);
		}
		tmp = tmp->next;
	}
	return (0);
}

int	ft_check_redir(t_cmd *node, t_envp **env)
{
	t_redir	*tmp;

	if (!node->redir || !node->redir->next)
		return (0);
	tmp = node->redir;
	if (!ft_strcmp(tmp->next->redir, "*"))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(tmp->next->redir, 2);
		ft_putstr_fd(": ambiguous redirect\n", 2);
		return (ft_return_code(ft_strdup("1"), env), -1);
	}
	if (!tmp->next || !tmp->next->redir[0])
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `newline'",
			2);
		return (ft_return_code(ft_strdup("2"), env), -1);
	}
	ft_check_redir2(node, env);
	return (0);
}

int	ft_check_word2(t_cmd *node, t_envp **env)
{
	if (node->next->type == O_BRACKET && !node->cmd[1])
	{
		ft_putstr_cmd_fd("minishell: syntax error near unexpected token `", 2,
			&node->next->next->cmd[0], 0);
		return (ft_return_code(ft_strdup("2"), env));
	}
	else if (node->next->type == O_BRACKET && node->cmd[1])
	{
		ft_putstr_cmd_fd("minishell: syntax error near unexpected token `", 2,
			&node->next->next->cmd[0], 0);
		return (ft_return_code(ft_strdup("2"), env));
	}
	return (0);
}

int	ft_check_word(t_cmd *node, t_envp **env, t_save_struct *t_struct)
{
	if (t_struct->cmd && t_struct->cmd->cmd && t_struct->cmd->cmd[0]
		&& t_struct->cmd->cmd[0][0] == '\t')
		return (-1);
	if (!node->cmd)
		return (0);
	if (!node->next)
	{
		ft_get_path(node, t_struct);
		if (ft_check_redir(node, env) != 0)
			return (-1);
		return (0);
	}
	if (node->next->next)
		if (ft_check_word2(node, env) != 0)
			return (-1);
	ft_get_path(node, t_struct);
	if (ft_check_redir(node, env) != 0)
		return (-1);
	return (0);
}

int	ft_check_cbracket(t_cmd *node, t_envp **env, t_save_struct *t_struct)
{
	t_cmd	*curr;

	(void)t_struct;
	if (!node->next)
		return (0);
	if (node->next == WORD)
		return (-1);
	curr = node;
	while (curr)
	{
		if (curr->prev == NULL)
		{
			ft_putstr_cmd_fd("minishell: syntax error near unexpected token `",
				2, &node->next->cmd[0], 0);
			return (ft_return_code(ft_strdup("2"), env));
		}
		if (curr->prev->type == O_BRACKET)
			break ;
		curr = curr->prev;
	}
	return (0);
}
