/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicolof <lnicolof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 00:30:40 by renard            #+#    #+#             */
/*   Updated: 2024/07/29 19:27:01 by lnicolof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_count_bracket(t_cmd *node, char **error_cmd, t_envp **env)
{
	t_cmd	*curr;
	int		p_counter;

	curr = node;
	p_counter = 0;
	while (curr)
	{
		if (curr->prev && curr->type == O_BRACKET
			&& curr->prev->type == O_BRACKET)
			if (ft_bad_expression(curr))
				return (ft_return_code(ft_strdup("1"), env));
		if (curr->type == O_BRACKET && ft_is_enum(curr->next, error_cmd))
			p_counter++;
		if (curr->type == C_BRACKET && ft_is_enum2(curr->next, error_cmd))
			p_counter--;
		curr = curr->next;
	}
	return (p_counter);
}

int	ft_check_obracket(t_cmd *node, t_envp **env, t_save_struct *t_struct)
{
	char	*error_cmd;

	(void)t_struct;
	error_cmd = NULL;
	if (*(node->bool_bracket) == 0)
	{
		if (ft_count_bracket(node, &error_cmd, env) != 0)
		{
			ft_putstr_cmd_fd("minishell: syntax error near unexpected token `",
				2, &error_cmd, 0);
			return (ft_return_code(ft_strdup("2"), env));
		}
		*(node->bool_bracket) = 1;
	}
	return (0);
}

int	ft_check_op(t_cmd *node, t_envp **env, t_save_struct *t_struct)
{
	(void)t_struct;
	if (!node->prev || !node->next || node->next->type == AND
		|| node->next->type == OR || node->next->type == PIPE)
	{
		ft_putstr_cmd_fd("minishell: syntax error near unexpected token `", 2,
			&node->cmd[0], 0);
		return (ft_return_code(ft_strdup("2"), env));
	}
	return (0);
}

int	ft_exec_func(t_cmd **cmd, t_envp **env, t_save_struct *t_struct)
{
	t_cmd	*curr;
	int		(*ft_tab[6])(t_cmd *, t_envp **, t_save_struct *);

	ft_init_ft_tab(ft_tab);
	curr = *cmd;
	while (curr)
	{
		if (curr->type == NO_TYPE)
		{
			ft_putstr_cmd_fd("minishell: syntax error near unexpected token `",
				2, &curr->cmd[0], 0);
			return (ft_return_code(ft_strdup("2"), env), -1);
		}
		if (!curr->cmd && curr->redir)
		{
			if (ft_check_redir(curr, env) != 0)
				return (-1);
		}
		else if (ft_tab[curr->type](curr, env, t_struct) != 0)
			return (-1);
		curr = curr->next;
	}
	return (0);
}

void	ft_init_ft_tab(int (*ft_tab[6])(t_cmd *, t_envp **, t_save_struct *))
{
	ft_tab[WORD] = ft_check_word;
	ft_tab[PIPE] = ft_check_pipe;
	ft_tab[AND] = ft_check_op;
	ft_tab[OR] = ft_check_op;
	ft_tab[O_BRACKET] = ft_check_obracket;
	ft_tab[C_BRACKET] = ft_check_cbracket;
}
