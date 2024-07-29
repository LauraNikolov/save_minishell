/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_create_token_lst.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicolof <lnicolof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 00:29:55 by renard            #+#    #+#             */
/*   Updated: 2024/07/29 16:44:23 by lnicolof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_search_var(char *var, t_envp **env)
{
	t_envp	*curr;

	curr = *env;
	if (!var || !*var)
		return (NULL);
	while (curr)
	{
		if (!ft_strcmp(curr->var_name, var))
			return (curr->var_value);
		curr = curr->next;
	}
	return (NULL);
}

static int	ft_get_symb(t_save_struct *t_struct, char *buff, char **cmd)
{
	int	len;
	int	i;

	i = 0;
	len = ft_check_double_symbols(buff, cmd);
	add_to_lst(&(t_struct->cmd), create_cmd_node(NULL, cmd, NULL));
	i = len;
	while (i--)
		ft_strcat(t_struct->save_spaces, "0");
	return (len);
}

void	ft_encode_expand(char **exp_code, char c, int quote_flag,
		t_save_struct *t_struct)
{
	int		len;
	char	*new_exp;

	if (!*exp_code)
	{
		*exp_code = ft_calloc(2, sizeof(char));
		if (!*exp_code)
			return ;
		if (c == '\'' && quote_flag == 1)
			(*exp_code)[0] = '0';
		else
			(*exp_code)[0] = '1';
		return ;
	}
	len = ft_strlen(*exp_code);
	if (ft_safe_malloc(&new_exp, len + 2, t_struct))
		return ;
	ft_strlcpy(new_exp, *exp_code, len + 1);
	if (c == '\'' && quote_flag == 1)
		ft_strcat(new_exp, "0");
	else
		ft_strcat(new_exp, "1");
	ft_safe_free(exp_code);
	*exp_code = ft_strdup(new_exp);
	ft_safe_free(&new_exp);
}

static int	ft_split_cmd(char *buffer, int *j, char **exp_code,
		t_save_struct *t_struct)
{
	int		len;
	char	c;
	int		quote_flag;

	len = 0;
	c = '\0';
	quote_flag = -1;
	while (((buffer[*j] && quote_flag == 1)) || ((buffer[*j]
				&& !ft_is_str(buffer[*j], "|()&") && quote_flag == -1)))
	{
		if (buffer[*j] == '\'' || buffer[*j] == '\"')
		{
			c = buffer[*j];
			quote_flag *= -1;
		}
		if (buffer[*j] == '$')
			ft_encode_expand(exp_code, c, quote_flag, t_struct);
		(*j)++;
		len++;
	}
	return (len);
}

void	ft_create_token_lst(char *buffer, t_save_struct *t_struct)
{
	char			*exp_code;
	int				j;
	int				len;
	t_data_parsing	data;

	j = 0;
	exp_code = NULL;
	ft_memset(&data, '0', sizeof(data));
	if (t_struct && !t_struct->save_spaces)
		if (ft_safe_malloc(&(t_struct->save_spaces), ft_strlen(buffer) + 1,
				t_struct) == -1)
			return (exit_error("Malloc failed\n", t_struct));
	data.buffer = buffer;
	data.cmd = NULL;
	data.t_struct = t_struct;
	data.bufflen = ft_strlen(buffer);
	while (buffer[j])
	{
		len = ft_split_cmd(buffer, &j, &exp_code, t_struct);
		add_to_lst(&(t_struct->cmd), create_cmd_node(ft_handle_quote(&buffer[j
					- len], len, &data, &data.cmd), &data.cmd, &exp_code));
		if (ft_is_str(buffer[j], "|()&"))
			j += ft_get_symb(t_struct, &buffer[j], &data.cmd);
	}
}
