/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_lst.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicolof <lnicolof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 13:52:23 by melmarti          #+#    #+#             */
/*   Updated: 2024/07/29 16:44:23 by lnicolof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_remove_null_node(t_cmd **lst)
{
	t_cmd	*curr;
	t_cmd	*next;

	curr = *lst;
	while (curr)
	{
		next = curr->next;
		if (!curr->cmd && !curr->redir)
		{
			if (curr == *lst)
			{
				*lst = next;
				if (next)
					next->prev = NULL;
			}
			else
			{
				curr->prev->next = next;
				if (next)
					next->prev = curr->prev;
			}
			free(curr);
		}
		curr = next;
	}
}

void	ft_clean_redir_lst(t_redir *redir, int *l, t_save_struct *t_struct)
{
	t_redir	*curr_redir;
	int		i;

	curr_redir = redir;
	while (curr_redir)
	{
		if (curr_redir->type == WORD)
		{
			i = -1;
			while (curr_redir->redir[++i])
			{
				while (t_struct->save_spaces[*l]
					&& ft_is_str(t_struct->save_spaces[*l], "0123"))
					(*l)++;
				if (curr_redir->redir[i] == '/'
					&& (t_struct->save_spaces[*l] == '4'))
					curr_redir->redir[i] = ' ';
				(*l)++;
			}
		}
		curr_redir = curr_redir->next;
	}
}

static void	ft_replace_spaces(char *cmd, t_save_struct *t_struct, int *k)
{
	int	j;

	j = 0;
	while (cmd[j])
	{
		while (t_struct->save_spaces[*k] && ft_is_str(t_struct->save_spaces[*k],
				"2456"))
			(*k)++;
		if ((cmd[j] == '/' && t_struct->save_spaces[*k] == '1'))
			cmd[j] = ' ';
		j++;
		(*k)++;
	}
}

void	ft_clean_cmd_lst(t_cmd **lst, t_save_struct *t_struct)
{
	t_cmd	*curr;
	int		i;
	int		k;
	int		l;

	l = 0;
	curr = *lst;
	k = 0;
	while (curr)
	{
		i = -1;
		if (!curr->cmd)
		{
			curr = curr->next;
			continue ;
		}
		while (curr->cmd[++i])
			ft_replace_spaces(curr->cmd[i], t_struct, &k);
		ft_clean_redir_lst(curr->redir, &l, t_struct);
		curr = curr->next;
	}
}
