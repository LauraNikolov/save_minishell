/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicolof <lnicolof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 00:30:51 by renard            #+#    #+#             */
/*   Updated: 2024/08/01 12:53:04 by lnicolof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_all_free(t_save_struct *t_struct, int flag, char **buff)
{
	if (flag)
		ft_safe_free(buff);
	if(t_struct->save_spaces)
		free(t_struct->save_spaces);
	if(t_struct->envp_to_char)
		ft_free_tab(t_struct->envp_to_char);
	if(t_struct->cmd)
		ft_free_lst(t_struct->cmd);
	if(t_struct)
		free(t_struct);
}

void	ft_safe_free(char **s)
{
	if (*s)
	{
		free(*s);
		*s = NULL;
	}
}

int	ft_safe_malloc(char **s, int size, t_save_struct *t_struct)
{
	if (!s || size <= 0)
		return (0);
	*s = ft_calloc(size, sizeof(char));
	if (!*s)
	{
		exit_error("Malloc error\n", t_struct);
		return (-1);
	}
	return (0);
}

int	ft_putstr_cmd_fd(char *s, int fd, char **str, int flag)
{
	int	i;

	i = -1;
	if (flag == 2)
	{
		write(fd, s, ft_strlen(s));
		return (-1);
	}
	write(fd, s, ft_strlen(s));
	if (flag == 1)
		while (str[++i])
		{
			write(fd, " ", 2);
			ft_putstr_fd(str[i], fd);
		}
	else if (str)
	{
		ft_putstr_fd(*str, fd);
		write(fd, "\'", 2);
	}
	write(fd, "\n", 2);
	return (-1);
}
