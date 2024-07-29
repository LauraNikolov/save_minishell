/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicolof <lnicolof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 00:30:03 by renard            #+#    #+#             */
/*   Updated: 2024/07/29 16:44:23 by lnicolof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_count_return_code(char *s, t_envp **env)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (s[i])
	{
		if (!ft_strncmp(&s[i], "$?", 2))
		{
			count += ft_strlen(ft_search_var("?", env));
			count -= 2;
		}
		i++;
	}
	return (count + ft_strlen(s));
}

static int	ft_match_return_val(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (!ft_strncmp(&s[i], "$?", 2))
			return (1);
		i++;
	}
	return (0);
}

static void	ft_concat_return_var(char *s, t_envp **env, char **res)
{
	char	*var_value;
	int		i;
	int		j;
	int		k;

	j = 0;
	k = 0;
	i = 0;
	while (s[i])
	{
		if (!ft_strncmp(&s[i], "$?", 2))
		{
			var_value = ft_search_var("?", env);
			k = 0;
			while (var_value[k])
				(*res)[j++] = var_value[k++];
			i += 2;
		}
		(*res)[j++] = s[i++];
	}
	(*res)[j] = '\0';
}

static char	*ft_expand_return_var(char *s, t_envp **env, char **exp_code,
		int *l)
{
	char	*res;

	res = NULL;
	if (ft_match_return_val(s))
	{
		if (*exp_code && (*exp_code)[*l] == '1')
		{
			res = malloc(sizeof(char) * (ft_count_return_code(s, env + 1)));
			if (!res)
				return (NULL);
			ft_concat_return_var(s, env, &res);
			(*l)++;
			return (ft_strdup(res));
		}
	}
	return (ft_strdup(s));
}

void	ft_expand(t_cmd *node, t_envp **env, t_save_struct *t_struct)
{
	int				i;
	int				len;
	t_data_parsing	data;

	i = -1;
	ft_memset(&data, '0', sizeof(t_data_parsing));
	data.exp = NULL;
	data.k = 0;
	while (node->cmd[++i])
	{
		if (ft_is_char(node->cmd[i], '$'))
		{
			data.exp = ft_strdup(node->cmd[i]);
			ft_safe_free(&node->cmd[i]);
			node->cmd[i] = ft_expand_return_var(data.exp, env, &node->exp_code,
					&data.k);
			ft_safe_free(&data.exp);
			len = ft_expand_len(node->cmd[i], env, &node->exp_code) + 1;
			ft_safe_malloc(&data.exp, len, t_struct);
			ft_cpy_expand(node->cmd[i], &data, env, &node->exp_code);
			ft_safe_free(&node->cmd[i]);
			node->cmd[i] = ft_strdup(data.exp);
			ft_safe_free(&data.exp);
		}
	}
}
