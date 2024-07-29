/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_redir.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: renard <renard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 00:30:17 by renard            #+#    #+#             */
/*   Updated: 2024/07/29 00:30:18 by renard           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	ft_add_infile(t_redir **redir, char *s, char **cmd_redir,
		t_data_parsing *data)
{
	int	len;
	int	quote_flag;
	int	i;

	quote_flag = -1;
	len = 0;
	while ((s[data->i + len] && quote_flag == 1) || (s[data->i + len]
			&& !ft_is_str(s[data->i + len], ">< ") && quote_flag == -1))
	{
		if (s[data->i + len] == '\'' || s[data->i + len] == '"')
			quote_flag *= -1;
		len++;
	}
	if (quote_flag == -1)
		len++;
	i = data->i;
	data->bufflen = -1;
	ft_handle_quote(&s[data->i], len - 1, data, cmd_redir);
	data->i = i;
	add_to_redir_lst(redir, create_redir_node(*cmd_redir));
	ft_safe_free(cmd_redir);
	data->i += len;
	data->bufflen = 1;
}

t_redir	*ft_redir(char *s, int len, t_data_parsing *data)
{
	t_redir	*redir;
	char	*cmd_redir;

	cmd_redir = NULL;
	redir = NULL;
	while (data->i < len)
	{
		if (ft_is_str(s[data->i], "><"))
		{
			data->i += ft_check_double_symbols(&s[data->i], &cmd_redir);
			add_to_redir_lst(&redir, create_redir_node(cmd_redir));
			ft_safe_free(&cmd_redir);
		}
		while (data->i < len && s[data->i] == ' ')
		{
			(data->i)++;
		}
		if (!ft_is_str(s[data->i], "><"))
		{
			ft_add_infile(&redir, s, &cmd_redir, data);
			break ;
		}
	}
	return (redir);
}

void	add_to_redir_lst(t_redir **head, t_redir *new_node)
{
	t_redir	*last;

	if (!*head)
	{
		*head = new_node;
		return ;
	}
	last = lst_last_redir(*head);
	last->next = new_node;
}

t_redir	*create_redir_node(char *s)
{
	t_redir	*new_node;

	if (!s)
		return (NULL);
	new_node = malloc(sizeof(t_redir));
	new_node->redir = ft_strdup(s);
	if (!ft_strcmp(s, "<<"))
		new_node->type = R_HEREDOC;
	else if (!ft_strcmp(s, "<"))
		new_node->type = R_IN;
	else if (!ft_strcmp(s, ">"))
		new_node->type = R_OUT;
	else if (!ft_strcmp(s, ">>"))
		new_node->type = R_APPEND;
	else
		new_node->type = WORD;
	new_node->next = NULL;
	return (new_node);
}
