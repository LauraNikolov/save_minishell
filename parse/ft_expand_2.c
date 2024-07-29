/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melmarti <melmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 00:30:00 by renard            #+#    #+#             */
/*   Updated: 2024/07/29 14:38:34 by melmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_bad_subst(char *s)
{
	int	i;

	i = 0;
	while (s[i] && ft_isalnum(s[i]))
		i++;
	if (s[i] != '}' || i == 0)
		return (1);
	return (0);
}

int	ft_expand_math(char *s, t_envp **env, int len)
{
	char	*var_value;
	char	*var;
	int		var_len;
	int		var_value_len;

	var = ft_strndup(s, len);
	var_value = ft_search_var(var, env);
	var_value_len = ft_strlen(var_value);
	var_len = ft_strlen(var) + 1;
	ft_safe_free(&var);
	return (var_len - var_value_len);
}

int	ft_expand_len(char *s, t_envp **env, char **exp_code)
{
	int	i;
	int	j;
	int	len;
	int	total_var_len;

	total_var_len = 0;
	j = 0;
	i = -1;
	while (s[++i])
	{
		if (s[i] == '$' && (ft_isalnum(s[i + 1])) && s[i + 1] != '?')
		{
			if (*exp_code && (*exp_code)[j] == '1')
			{
				len = 1;
				while (s[i + len] && !ft_is_str(s[i + len], "\'\" $}"))
					len++;
				total_var_len += ft_expand_math(&s[i + 1], env, len - 1);
				i += len - 1;
			}
			j++;
		}
	}
	return (i - total_var_len);
}

int	ft_cpy_expand_2(char *s, char **exp, int *j, t_envp **env)
{
	char	*var_value;
	char	*var;
	int		i;
	int		index;
	int		len;

	index = 0;
	i = 0;
	len = 1;
	while (s[i + len] && !ft_is_str(s[i + len], "\'\" $}"))
		len++;
	var = ft_strndup(&s[i + 1], len - 1);
	var_value = ft_search_var(var, env);
	index = 0;
	if (var_value)
		while (var_value[index])
			(*exp)[(*j)++] = var_value[index++];
	i += len;
	ft_safe_free(&var);
	return (i);
}

void	ft_cpy_expand(char *s, t_data_parsing *data, t_envp **env,
		char **exp_code)
{
	int i;
	int j;

	j = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] == '$' && ft_isalnum(s[i + 1]) && s[i + 1] != '?')
		{
			if (*exp_code && (*exp_code)[data->k] == '1')
				i += ft_cpy_expand_2(&s[i], &data->exp, &j, env);
			else if (*exp_code && (*exp_code)[data->k] == '0')
			{
				(data->exp)[j++] = s[i++];
			}
			data->k++;
		}
		else
			(data->exp)[j++] = s[i++];
	}
	(data->exp)[j] = '\0';
}