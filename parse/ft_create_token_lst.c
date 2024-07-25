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

int	ft_inside_quote(char *s, char **cmd, int *cmd_index, save_struct *t_struct)
{
	int		i;
	char	c;

	i = 0;
	c = s[-1];
	while (s[i] && s[i] != c)
	{
		if (s[i] == ' ')
		{
			(*cmd)[*cmd_index] = '%';
			if(t_struct)
				ft_strcat(t_struct->save_spaces, "1");
			(*cmd_index)++;
		}
		else
		{
			(*cmd)[*cmd_index] = s[i];
			if (t_struct)
				ft_strcat(t_struct->save_spaces, "0");
			(*cmd_index)++;
		}
		i++;
	}
	return (i + 1);
}

t_redir	*ft_redir(char *s, int *i, int len)
{
	t_redir	*redir;
	char	*cmd;
	int		infile;

	infile = 0;
	redir = NULL;
	while (*i < len)
	{
		if (ft_is_str(s[*i], "><"))
		{
			*i += ft_check_double_symbols(&s[*i], &cmd);
			add_to_redir_lst(&redir, create_redir_node(cmd));
			ft_safe_free(&cmd);
		}
		while (*i < len && s[*i] == ' ')
			(*i)++;
		if (!ft_is_str(s[*i], "><"))
		{
			while (s[*i + infile] && !ft_is_str(s[*i + infile], "><") && s[*i
				+ infile] != ' ')
				infile++;
			ft_handle_quote(&s[*i], &cmd, infile, NULL, 0);
			add_to_redir_lst(&redir, create_redir_node(cmd));
			ft_safe_free(&cmd);
			(*i) += infile - 1;
			break ;
		}
	}
	return (redir);
}

t_redir	*ft_handle_quote(char *s, char **cmd, int len, save_struct *t_struct, int bufflen)
{
	int		i;
	int		cmd_index;
	t_redir	*redir;

	redir = NULL;

	if (t_struct && !t_struct->save_spaces)
		ft_safe_malloc(&(t_struct->save_spaces), bufflen + 1);
	ft_safe_malloc(cmd, ft_quote_len(s, len) + 1);
	cmd_index = 0;
	i = -1;
	while (s[++i] && i < len)
	{
		if (s[i] == '\'' || s[i] == '\"')
			i += ft_inside_quote(&s[i + 1], cmd, &cmd_index, t_struct);
		else if (ft_is_str(s[i], "><"))
			add_to_redir_lst(&redir, ft_redir(s, &i, len));
		else
		{
			(*cmd)[cmd_index] = s[i];
			if (t_struct && (*cmd)[cmd_index] != ' ')
				ft_strcat(t_struct->save_spaces, "3");
			else if (t_struct)
				ft_strcat(t_struct->save_spaces, "2");
			cmd_index++;
		}
	}
	return (redir);
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

static int	ft_get_symb(save_struct *t_struct, char *buff, char **cmd)
{
	int	len;
	int	i;

	i = 0;
	len = ft_check_double_symbols(buff, cmd);
	add_to_lst(&(t_struct->cmd), create_cmd_node(NULL, cmd, buff[-1], t_struct));
	i = len;
	while (i--)
		ft_strcat(t_struct->save_spaces, "0");
	return (len);
}

void	ft_create_token_lst(char *buffer, save_struct *t_struct)
{
	char	*cmd;
	int		j;
	int		len;
	int		quote_flag;

	j = 0;
	quote_flag = -1;
	cmd = NULL;
	while (buffer[j])
	{
		len = 0;
		while (((buffer[j] && quote_flag == 1)) || ((buffer[j]
					&& !ft_is_str(buffer[j], "|()&") && quote_flag == -1)))
		{
			if (buffer[j] == '\'' || buffer[j] == '\"')
				quote_flag *= -1;
			j++;
			len++;
		}
		add_to_lst(&(t_struct->cmd), create_cmd_node(ft_handle_quote(&buffer[j
					- len], &cmd, len, t_struct, ft_strlen(buffer)), &cmd, buffer[j - 1], t_struct));
		if (ft_is_str(buffer[j], "|()&"))
			j += ft_get_symb(t_struct, &buffer[j], &cmd);
	}
}

