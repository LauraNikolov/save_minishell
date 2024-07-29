/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: renard <renard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 00:30:48 by renard            #+#    #+#             */
/*   Updated: 2024/07/29 00:31:03 by renard           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
