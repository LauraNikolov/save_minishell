/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_envp_to_char.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicolof <lnicolof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 13:53:09 by melmarti          #+#    #+#             */
/*   Updated: 2024/08/01 15:27:26 by lnicolof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_lst_env_size(t_envp *env)
{
	int	i;

	i = 0;
	while (env)
	{
		if (!env->var_name)
		{
			env = env->next;
			continue ;
		}
		env = env->next;
		i++;
	}
	return (i);
}

static int	ft_var_size(t_envp *node)
{
	int	i;
	int	len;

	len = 0;
	i = 0;
	if (!node)
		return (0);
	if (node->var_name)
	{
		while (node->var_name[i])
		{
			i++;
			len++;
		}
	}
	i = 0;
	if (node->var_value)
	{
		while (node->var_value[i])
		{
			i++;
			len++;
		}
	}
	return (len + 1);
}

static void	ft_split_envp(char **envp, t_envp **env)
{
	int	j;
	int	l;
	int	k;
	int len;
	t_envp *curr;

	curr = *env;

	j = -1;
	if (!env)
		return ;
	while (curr)
	{
		if (!curr->var_name)
		{
			curr = curr->next;
			continue ;
		}
		len = ft_var_size(curr) + 1;
		printf("size = %d, %s", len, curr->var_name);
		envp[++j] = malloc(sizeof(char) * len);
		if (!envp[j])
			return ;
		k = 0;
		l = 0;	
		while (curr->var_name[l])
		{
			envp[j][k] = curr->var_name[l];
			l++;
			k++;
		}
		envp[j][k++] = '=';
		l = 0;
		if (curr->var_value)
			while (curr->var_value[l])
				envp[j][k++] = curr->var_value[l++];
		envp[j][k] = curr->var_name[l];
		printf(" strlen = %ld\n\n", strlen(envp[j]));
		curr = curr->next;
	}
	envp[j] = NULL;
}

char	**ft_envp_to_char(t_envp **env, t_save_struct *tstruct)
{
	if(tstruct->envp_to_char)
		ft_free_tab(tstruct->envp_to_char);
	tstruct->envp_to_char = NULL;
	tstruct->envp_to_char = malloc(sizeof(char *) * (ft_lst_env_size(*env)));
	if (tstruct->envp_to_char == NULL)
		return (exit_error("malloc failed\n", tstruct), NULL);
	ft_split_envp(tstruct->envp_to_char, env);
	return (tstruct->envp_to_char);
}
