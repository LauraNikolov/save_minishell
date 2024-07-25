/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicolof <lnicolof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 15:33:34 by lnicolof          #+#    #+#             */
/*   Updated: 2024/07/22 10:41:18 by lnicolof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_nbr_of_cmd(t_cmd *cmd)
{
	t_cmd	*current;
	int		i;

	current = cmd;
	i = 0;
	while (current)
	{
		if (current->type == WORD)
			i++;
		current = current->next;
	}
	return (i);
}
