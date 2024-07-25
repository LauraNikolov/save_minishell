#include "../minishell.h"

void ft_safe_free(char **s)
{
	if (*s)
	{
		free(*s);
		*s = NULL;
	}
}

int	ft_safe_malloc(char **s, int size)
{
	if (!s)
		return (0);
	*s = ft_calloc(size, sizeof(char));
	if (!*s)
	{
		ft_putstr_fd("Malloc error\n", 2);
		return (-1);
	}
	return (0);
}

int	ft_is_str(char c, char *s)
{
	int	i;

	i = 0;
	if (!c || !s)
		return (0);
	while (s[i])
	{
		if (c == s[i])
			return (1);
		i++;
	}
	return (0);
}

void	ft_free_tab(char **split)
{
	int	i;

	i = 0;
	while (split[i])
		free(split[i++]);
	free(split);
}

void	ft_swap_content(char **s1, char **s2)
{
	char	*tmp;

	tmp = *s1;
	*s1 = *s2;
	*s2 = tmp;
}


void	ft_override_content(char **s1, char *s2)
{
	if (!s2)
		return ;
	free(*s1);
	*s1 = NULL;
	*s1 = ft_strdup(s2);
}

int	ft_str_is_alpha(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if ((s[i] >= 'a' || s[i] <= 'z') || (s[i] >= 'A' || s[i] <= 'Z'))
			return (1);
		i++;
	}
	return (0);
}

int	ft_putstr_cmd_fd (char *s, int fd, char **str, int flag)
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

void	ft_all_free(save_struct *t_struct)
{
	if(t_struct->cmd)
		ft_free_lst(t_struct->cmd);
	if(t_struct->save_spaces)
		free(t_struct->save_spaces);
	if(t_struct->ast)
		free(t_struct->ast);
	if(t_struct)
		free(t_struct);
}


