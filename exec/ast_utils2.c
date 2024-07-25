/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicolof <lnicolof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 09:40:35 by lnicolof          #+#    #+#             */
/*   Updated: 2024/07/25 16:35:14 by lnicolof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_parenthesis(t_cmd *node)
{
	int		count;
	t_cmd	*current;

	if (!node)
		return (0);
	count = 0;
	current = node;
	while (current)
	{
		if (current->type == C_BRACKET)
			count++;
		else if (current->type == O_BRACKET)
			count--;
		current = current->next;
	}
	return (count);
}

int	recursive_free_ast(t_ast **ast)
{
	if (*ast == NULL)
        return (0);
    if ((*ast)->left)
        recursive_free_ast(&(*ast)->left);
    if ((*ast)->right)
        recursive_free_ast(&(*ast)->right);
    free(*ast);
    *ast = NULL;
    return (0);
}

void pipe_error(void)
{
    perror("pipe :");
    return;
}
