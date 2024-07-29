/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_save_envp.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: renard <renard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 00:30:23 by renard            #+#    #+#             */
/*   Updated: 2024/07/29 00:31:32 by renard           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_save_envp(char **envp_tab, t_envp **envp_lst)
{
	int		i;
	char	return_code[4];

	i = 0;
	while (envp_tab[i])
	{
		add_to_envp_lst(envp_lst, create_envp_node(envp_tab[i], 0));
		i++;
	}
	return_code[0] = '?';
	return_code[1] = '=';
	return_code[2] = '0';
	return_code[3] = '\0';
	add_to_envp_lst(envp_lst, create_envp_node(return_code, 0));
	return ;
}
