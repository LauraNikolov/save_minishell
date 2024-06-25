/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melmarti <melmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 12:46:17 by melmarti          #+#    #+#             */
/*   Updated: 2024/06/12 15:36:40 by melmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	clean_and_save(t_list **stash)
{
	int		i;
	int		j;
	t_list	*new_node;
	t_list	*last;

	new_node = malloc(sizeof(t_list));
	if (!stash || !new_node)
		return ;
	new_node->next = NULL;
	last = ft_lst_last(*stash);
	i = 0;
	while (last->content[i] && last->content[i] != '\n')
		i++;
	if (last->content && last->content[i] == '\n')
		i++;
	new_node->content = malloc(sizeof(char) * ((ft_strlen(last->content) - i)
				+ 1));
	j = 0;
	while (last->content[i])
		new_node->content[j++] = last->content[i++];
	new_node->content[j] = '\0';
	ft_free_gnl(*stash);
	*stash = new_node;
}

void	extract_line(t_list *stash, char **line)
{
	int	i;
	int	j;

	if (stash == NULL)
		return ;
	ft_malloc_line(stash, line);
	if (!line)
		return ;
	j = 0;
	while (stash)
	{
		i = 0;
		while (stash->content[i])
		{
			if (stash->content[i] == '\n')
			{
				(*line)[j++] = stash->content[i];
				break ;
			}
			(*line)[j++] = stash->content[i++];
		}
		stash = stash->next;
	}
	(*line)[j] = '\0';
}

void	add_to_lst_gnl(t_list **stash, char *buffer, int size)
{
	t_list	*new_node;
	t_list	*last;
	int		i;

	new_node = malloc(sizeof(t_list));
	if (!new_node)
		return ;
	new_node->next = NULL;
	new_node->content = malloc(sizeof(char) * (size + 1));
	if (!new_node->content)
		return ;
	i = 0;
	while (buffer[i] && i < size)
	{
		new_node->content[i] = buffer[i];
		i++;
	}
	new_node->content[i] = '\0';
	if (*stash == NULL)
	{
		*stash = new_node;
		return ;
	}
	last = ft_lst_last(*stash);
	last->next = new_node;
}

void	read_and_add(t_list **stash, int fd)
{
	char	*buffer;
	int		nread;

	nread = 1;
	while (!is_end(*stash) && nread != 0)
	{
		buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
		if (!buffer)
			return ;
		nread = (int)read(fd, buffer, BUFFER_SIZE);
		if ((!*stash && nread == 0) || nread == -1)
		{
			free(buffer);
			return ;
		}
		buffer[nread] = '\0';
		add_to_lst_gnl(stash, buffer, nread);
		free(buffer);
	}
}

char	*get_next_line(int fd)
{
	static t_list	*stash = NULL;
	char			*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, &line, 0) < 0)
		return (NULL);
	line = NULL;
	read_and_add(&stash, fd);
	if (stash == NULL)
		return (NULL);
	extract_line(stash, &line);
	clean_and_save(&stash);
	if (line[0] == '\0')
	{
		ft_free_gnl(stash);
		stash = NULL;
		free(line);
		return (NULL);
	}
	return (line);
}

//  int	main(void)
// {
// 	int fd;
// 	char *line;

// 	fd = open("text.txt", O_RDONLY);
// 	if (fd == -1)
// 		return (1);
// 	line = get_next_line(fd);
// 	while (line)
// 	{
// 		printf("%s", line);
// 		free(line);
// 		line = get_next_line(fd);
// 	}
// 	close(fd);
// 	return (0);
// } 
