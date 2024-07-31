/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicolof <lnicolof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 23:23:29 by lnicolof          #+#    #+#             */
/*   Updated: 2024/07/31 23:35:43 by lnicolof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	error_export2(char *var, t_envp **env)
{
	ft_putstr_cmd_fd("Minishell : export: `", 2, NULL, 2);
	ft_putstr_cmd_fd(var, 2, NULL, 2);
	ft_putstr_cmd_fd("': not a valid identifier", 2, NULL, 0);
	return (ft_return_code(ft_strdup("1"), env));
}

void	ft_print_env(t_envp **env)
{
	t_envp	*curr;

	curr = *env;
	while (curr)
	{
		if (!ft_strcmp(curr->var_name, "?"))
		{
			curr = curr->next;
			continue ;
		}
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(curr->var_name, 1);
		if (curr->var_value)
		{
			write(1, "=\"", 3);
			ft_putstr_fd(curr->var_value, 1);
			write(1, "\"", 2);
		}
		else if (curr->print_flag && !curr->var_value)
			write(1, "=\"\"", 4);
		write(1, "\n", 2);
		curr = curr->next;
	}
}
