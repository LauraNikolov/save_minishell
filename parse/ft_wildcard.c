/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wildcard.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melmarti <melmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 00:30:30 by renard            #+#    #+#             */
/*   Updated: 2024/07/29 13:24:39 by melmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	get_wild_len(char *s)
{
	struct dirent	*entry;
	DIR				*dir;
	int				len;

	len = 0;
	dir = opendir(".");
	entry = readdir(dir);
	while (entry)
	{
		if (ft_match(s, entry->d_name))
			len++;
		entry = readdir(dir);
	}
	closedir(dir);
	return (len);
}

static void	ft_readdir(char ***new_tab, char **cmd, int *j, int i)
{
	struct dirent	*entry;
	DIR				*dir;

	dir = opendir(".");
	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] != '.')
		{
			if (ft_match(cmd[i], entry->d_name))
				(*new_tab)[(*j)++] = ft_strdup(entry->d_name);
		}
		entry = readdir(dir);
	}
	closedir(dir);
}

static char	**ft_cpy_match(int i, char **new_tab, char **cmd)
{
	int	j;
	int	index;

	index = -1;
	j = 0;
	while (cmd[++index])
	{
		if (index == i)
			ft_readdir(&new_tab, cmd, &j, i);
		else
			new_tab[j++] = ft_strdup(cmd[index]);
	}
	new_tab[j] = NULL;
	return (new_tab);
}

char	**ft_new_args(char **cmd)
{
	int		i;
	char	**new_tab;
	int		len;
	int		wild_len;

	i = -1;
	while (cmd[++i])
	{
		if (ft_is_char(cmd[i], '*'))
		{
			wild_len = get_wild_len(cmd[i]);
			len = ft_count_tab(cmd) + wild_len;
			new_tab = ft_calloc(sizeof(char *), len + wild_len);
			if (!new_tab)
				return (NULL);
			new_tab = ft_cpy_match(i, new_tab, cmd);
			ft_free_tab(cmd);
			cmd = ft_strdup_array(new_tab);
			ft_free_tab(new_tab);
		}
	}
	return (cmd);
}

void	ft_wildcard(t_cmd **lst)
{
	t_cmd	*curr;

	curr = *lst;
	while (curr)
	{
		if (curr->cmd)
			curr->cmd = ft_new_args(curr->cmd);
		curr = curr->next;
	}
}
