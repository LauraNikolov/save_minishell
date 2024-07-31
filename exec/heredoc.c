/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicolof <lnicolof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 15:39:53 by lnicolof          #+#    #+#             */
/*   Updated: 2024/08/01 00:06:16 by lnicolof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	while_heredoc(char **line, int file, char *limiter)
{
	while (1)
	{
		write(1, "here_doc>  ", 10);
		*line = get_next_line(STDOUT_FILENO);
		if (*line == NULL)
		{
			write(STDOUT_FILENO, "\n", 1);
			return (1);
		}
		if (ft_limiter(limiter, *line) == 1)
			return (1);
		write(file, *line, ft_strlen(*line) - 1);
		write(file, "\n", 1);
		free(*line);
	}
	return (0);
}

char	*create_here_doc(char *str, char *limiter)
{
	int		file;
	char	*line;
	int		pid;

	file = 0;
	line = NULL;
	ft_signal(2);
	file = open(str, O_CREAT | O_RDWR | O_TRUNC, 0000644);
	if (file == -1)
		perror("open:");
	pid = fork();
	if (pid == 0)
	{
		while (1)
		{
			if (while_heredoc(&line, file, limiter) == 1)
				break ;
		}
		free(line);
		close(file);
		exit(22);
	}
	else
		heredoc_parent(pid, file);
	return (str);
}

static void	handle_heredoc(t_redir *redir, int i, t_save_struct *t_struct)
{
	char	*heredocname;

	heredocname = ft_strjoin(ft_itoa(i), "heredoctmp");
	if (redir->type == R_HEREDOC)
	{
		redir->next->redir = create_here_doc(heredocname, redir->next->redir);
		if (!redir->next->redir)
			exit_error("heredoc failed\n", t_struct);
	}
}

static void	process_redirections(t_cmd *cmd, int *i, t_save_struct *t_struct)
{
	t_redir	*current_redir;

	current_redir = cmd->redir;
	while (current_redir && current_redir->type == R_HEREDOC)
	{
		handle_heredoc(current_redir, *i, t_struct);
		if (current_redir->next)
			current_redir = current_redir->next->next;
		else
			current_redir = NULL;
		(*i)++;
	}
}

void	manage_heredoc(t_cmd *cmd, t_save_struct *t_struct)
{
	int		i;
	t_cmd	*current;

	i = 0;
	current = cmd;
	while (current)
	{
		if (current->redir)
			process_redirections(current, &i, t_struct);
		current = current->next;
	}
}
