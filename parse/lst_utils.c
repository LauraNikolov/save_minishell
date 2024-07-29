/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melmarti <melmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 00:30:37 by renard            #+#    #+#             */
/*   Updated: 2024/07/29 13:59:43 by melmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_print_envp(t_envp **envp)
{
	t_envp	*curr;

	curr = *envp;
	while (curr)
	{
		if (!ft_strcmp(curr->var_name, "?"))
		{
			curr = curr->next;
			continue ;
		}
		printf("%s", curr->var_name);
		printf("=%s\n", curr->var_value);
		curr = curr->next;
	}
	return (0);
}

t_cmd	*lst_last(t_cmd *node)
{
	t_cmd	*curr;

	curr = node;
	while (curr->next)
		curr = curr->next;
	return (curr);
}

t_redir	*lst_last_redir(t_redir *node)
{
	t_redir	*curr;

	curr = node;
	while (curr->next)
		curr = curr->next;
	return (curr);
}

t_envp	*lst_envp_last(t_envp *node)
{
	t_envp	*curr;

	if (!node)
		return (NULL);
	curr = node;
	while (curr->next)
		curr = curr->next;
	return (curr);
}
