#include "../minishell.h"

int	ft_match(char *cmd, char *dir)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	j = 0;
	k = 0;
	while (cmd[i] && cmd[i] != '*')
		i++;
	while (cmd[k + i] && cmd[k + i] == '*')
		k++;
	if ((!cmd || !cmd[0]) && dir)
		return (0);
	if (!ft_strncmp(cmd, dir, i))
	{
		if (cmd[i + k - 1] == '*' && !cmd[i + k])
			return (1);
		if (!cmd[i + k] && !dir[i])
			return (1);
		while (cmd[i + k] && dir[j] && cmd[i + k] != dir[j])
			j++;
		j += ft_is_char(&dir[j + 1], dir[j]);
		return (ft_match(&cmd[i + k], &dir[j]));
	}
	return (0);
}

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

static char	**ft_cpy_match(int i, char **new_tab, char **cmd)
{
	struct dirent	*entry;
	DIR				*dir;
	int				index;
	int				j;

	j = 0;
	index = -1;
	while (cmd[++index])
	{
		if (index == i)
		{
			dir = opendir(".");
			entry = readdir(dir);
			while (entry)
			{
				if (entry->d_name[0] != '.')
				{
					if (ft_match(cmd[i], entry->d_name))
						new_tab[j++] = ft_strdup(entry->d_name);
				}
				entry = readdir(dir);
			}
			closedir(dir);
		}
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
			if (wild_len)
			{
				new_tab = ft_calloc(sizeof(char *), len + wild_len);
				if (!new_tab)
					return (NULL);
				new_tab = ft_cpy_match(i, new_tab, cmd);
				ft_free_tab(cmd);
				cmd = ft_strdup_array(new_tab);
				ft_free_tab(new_tab);
			}
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
		if (curr->cmd && curr->expand_flag)
			curr->cmd = ft_new_args(curr->cmd);
		curr = curr->next;
	}
	
}
