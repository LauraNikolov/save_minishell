/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is_enum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melmarti <melmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 00:30:14 by renard            #+#    #+#             */
/*   Updated: 2024/07/29 14:11:34 by melmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_is_enum(t_cmd *node, char **error_node)
{
	if (!node)
		return (0);
	if (node->type == PIPE || node->type == R_HEREDOC || node->type == R_APPEND
		|| node->type == C_BRACKET)
		return (0);
	else if (node->prev)
		*error_node = node->cmd[0];
	return (1);
}

int	ft_is_enum2(t_cmd *node, char **error_node)
{
	if (!node)
		return (1);
	if (node->type == R_OUT)
		return (1);
	else if (node->type == AND)
		return (1);
	else if (node->type == OR)
		return (1);
	else if (node->type == C_BRACKET)
		return (1);
	else if (node->type == PIPE)
		return (1);
	else if (!*error_node)
		*error_node = node->cmd[0];
	return (0);
}
