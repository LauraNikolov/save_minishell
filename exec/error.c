/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicolof <lnicolof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 11:37:50 by lnicolof          #+#    #+#             */
/*   Updated: 2024/07/29 16:52:24 by lnicolof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exit_error(char *str, t_save_struct *t_struct)
{
	ft_putstr_fd(str, 2);
	ft_free_envp_lst(&t_struct->envp, NULL);
	ft_all_free(t_struct, 0, NULL);
	exit(134);
}
