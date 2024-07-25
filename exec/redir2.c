/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicolof <lnicolof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 10:52:44 by lnicolof          #+#    #+#             */
/*   Updated: 2024/07/22 10:53:17 by lnicolof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	dispatch_redir(t_redir *current, int *fd_in, int *fd_out)
{
	if (current->type == R_HEREDOC || current->type == R_IN)
	{
		if (*fd_in != -1)
			close(*fd_in);
		if (open_redir_in(current, fd_in, fd_out) == -1)
			return (-1);
	}
	else if (current->type == R_OUT)
	{
		if (*fd_out != -1)
			close(*fd_out);
		if (open_redir_rout(current, fd_in, fd_out) == -1)
			return (-1);
	}
	else if (current->type == R_APPEND)
	{
		if (*fd_out != -1)
			close(*fd_out);
		if (open_redir_rappend(current, fd_in, fd_out) == -1)
			return (-1);
	}
	return (0);
}
