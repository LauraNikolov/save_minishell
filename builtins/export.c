/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicolof <lnicolof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 23:49:29 by lnicolof          #+#    #+#             */
/*   Updated: 2024/07/31 23:49:30 by lnicolof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_sort_env(t_envp **env)
{
	t_envp	*curr;

	if (!env || !*env)
		return ;
	curr = *env;
	while (curr->next)
	{
		if (ft_strcmp(curr->var_name, curr->next->var_name) > 0)
		{
			ft_swap_content(&curr->var_name, &curr->next->var_name);
			ft_swap_content(&curr->var_value, &curr->next->var_value);
			curr = *env;
		}
		else
			curr = curr->next;
	}
}

static int	set_i(char *var)
{
	int	i;

	i = ft_strchr(var, '=');
	if (i == -1)
		i = ft_strlen(var);
	return (i);
}

static void	ft_add_var(t_envp **env, char *var, int flag)
{
	t_envp	*curr;
	int		i;

	curr = *env;
	i = set_i(var);
	while (curr)
	{
		if (!ft_strncmp(curr->var_name, var, i))
		{
			if ((size_t)i != ft_strlen(curr->var_name))
				break ;
			flag = 1;
			if (var[i + 1])
				ft_override_content(&curr->var_value, &var[i + 1]);
			if (var[i] == '=')
				curr->print_flag = 1;
		}
		curr = curr->next;
	}
	if (!flag && var[i] == '=')
		add_to_envp_lst(env, create_envp_node(var, 1));
	else if (!flag)
		add_to_envp_lst(env, create_envp_node(var, 0));
}

int	ft_fork_export(t_envp **env)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		ft_sort_env(env);
		ft_print_env(env);
		exit(0);
	}
	else
		wait(NULL);
	return (0);
}

int	ft_export(t_cmd *node, t_envp **env)
{
	int		i;
	int		j;
	char	**var;

	var = node->cmd;
	if (!env || !*var)
		return (0);
	if (!var[1])
		return (ft_fork_export(env));
	i = 0;
	ft_expand(node, env, NULL);
	while (var[++i])
	{
		j = 0;
		if (var[i][0] == '=')
			error_export2(var[i], env);
		while (var[i][j] && var[i][j] != '=')
		{
			if (var[i][j] == '-' || !ft_isalpha(var[i][0]))
				error_export2(var[i], env);
			j++;
		}
		ft_add_var(env, var[i], 0);
	}
	return (ft_return_code(ft_strdup("0"), env));
}
