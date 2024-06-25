#include "../minishell.h"

int	ft_check_pipe(t_cmd *node, t_envp **env)
{
	if (!node->prev || !node->next)
	{
		printf("Minishell: syntax error near unexpected token '%s'\n", node->cmd[0]);
		return (-1);
	}
	if (!(node->prev->type == WORD || node->prev->type == C_BRACKET)
		&& (node->next->type == R_OUT || node->next->type == R_IN
			|| node->next->type == R_APPEND || node->next->type == R_HEREDOC
			|| node->next->type == WORD))
	{
		ft_putstr_cmd_fd("Minishell: syntax error near unexpected token `", 2,
					&node->next->cmd[0], 0);
		return (ft_return_code("2", env));
	}
	return (0);
}

int	ft_check_word(t_cmd *node, t_envp **env)
{
	if (!node->next)
	{
		ft_get_path(node);
		if (!node->path)
			node->path = ft_strdup(node->cmd[0]);
		return (0);
	}
	if (node->next->next)
	{
		if (node->next->type == O_BRACKET && !node->cmd[1])
		{
			ft_putstr_cmd_fd("Minishell: syntax error near unexpected token `", 2,
					&node->next->next->cmd[0], 0);
				return (ft_return_code("2", env));
		}
		else if (node->next->type == O_BRACKET && node->cmd[1])
		{
			ft_putstr_cmd_fd("Minishell: syntax error near unexpected token `", 2,
					&node->next->next->cmd[0], 0);
				return (ft_return_code("2", env));
		}
	}
	ft_get_path(node);
	if (!node->path)
		node->path = ft_strdup(node->cmd[0]);
	return (0);
}

int	ft_check_Cbracket(t_cmd *node, t_envp **env)
{
	t_cmd	*curr;

	if (!node->next)
		return (0);
	if (node->next == WORD)
		return (-1);
	curr = node;
	while (curr)
	{
		if (curr->prev == NULL)
		{
			ft_putstr_cmd_fd("Minishell: syntax error near unexpected token `", 2,
					&node->next->cmd[0], 0);
			return (ft_return_code("2", env));
		}
		if (curr->prev->type == O_BRACKET)
			break ;
		curr = curr->prev;
	}
	return (0);
}

int	ft_is_enum(t_cmd *node, char **error_node)
{
	if (!node)
		return (0);
	if (node->type == PIPE || node->type == R_HEREDOC || node->type == R_APPEND)
		return (0);
	else if (node->prev)
		*error_node = node->cmd[0];
	return (1);
}

int	ft_is_enum2(t_cmd *node, char **error_node)
{
	if (!node)
		return (1);
	if (node->type == R_OUT)
		return (1);
	else if (node->type == AND)
		return (1);
	else if (node->type == OR)
		return (1);
	else if (node->type == C_BRACKET)
		return (1);
	else if (node->type == PIPE)
		return (1);
	else if (!*error_node)
		*error_node = node->cmd[0];
	return (0);
}

int	ft_bad_expression(t_cmd *node)
{
	if (!node->next)
		return (0);
	if (node->next->type == WORD)
	{
		if (node->next->cmd[1])
		{
			ft_putstr_cmd_fd("Minishell: syntax error in expression :", 2,
				node->next->cmd, 1);
			return (1);
		}
	}
	return (0);
}

int	ft_check_Obracket(t_cmd *node, t_envp **env)
{
	t_cmd	*curr;
	char	*error_cmd;
	int		p_counter;

	error_cmd = NULL;
	curr = NULL;
	p_counter = 0;
	if (*(node->bool_bracket) == 0)
	{
		curr = node;
		while (curr)
		{
			if (curr->prev && curr->type == O_BRACKET
				&& curr->prev->type == O_BRACKET)
			{
				if (ft_bad_expression(curr))
					return(ft_return_code("1", env));
			}
			if (curr->type == O_BRACKET && ft_is_enum(curr->next, &error_cmd))
				p_counter++;
			if (curr->type == C_BRACKET && ft_is_enum2(curr->next, &error_cmd))
				p_counter--;
			curr = curr->next;
		}
		if (p_counter != 0)
		{
			ft_putstr_cmd_fd("Minishell: syntax error near unexpected token `",
					2, &error_cmd, 0);
			return(ft_return_code("2", env));
		}
		*(node->bool_bracket) = 1;
	}
	return (0);
}

int	ft_check_op(t_cmd *node, t_envp **env)
{
	if (!node->next || node->next->type == AND || node->next->type == OR
		|| node->next->type == PIPE)
	{
		ft_putstr_cmd_fd("Minishell: syntax error near unexpected token `",
				2, &node->cmd[0], 0);
		return (ft_return_code("2", env));
	}
	return (0);
}

int	ft_check_redir(t_cmd *node, t_envp **env)
{
	if (!node->next || node->next->type == AND || node->next->type == OR
		|| node->next->type == PIPE || node->next->type != WORD)
	{
		ft_putstr_cmd_fd("Minishell: syntax error near unexpected token `",
				2, &node->cmd[0], 0);
		return (ft_return_code("2", env));
	}
	return (0);
}

int	ft_exec_syntax_functions(t_cmd **cmd, t_envp **env)
{
	t_cmd	*curr;
	int		(*ft_tab[10])(t_cmd *, t_envp **);

	ft_init_ft_tab(ft_tab);
	curr = *cmd;
	while (curr)
	{
		if (!ft_tab[curr->type])
			curr = curr->next;
		else if (curr->type == NO_TYPE)
		{
			ft_putstr_cmd_fd("Minishell: syntax error near unexpected token `",
				2, &curr->cmd[0], 0);
			ft_return_code("2", env);
			return (-1);
		}
		else if (ft_tab[curr->type](curr, env) != 0)
			return (-1);
		else 
			curr = curr->next;
	}
	if(curr == *cmd)
		return (-1);
	return (0);
}

int	ft_init_ft_tab(int (*ft_tab[10])(t_cmd *, t_envp **))
{
	ft_tab[WORD] = ft_check_word;
	ft_tab[PIPE] = ft_check_pipe;
	ft_tab[AND] = ft_check_op;
	ft_tab[OR] = ft_check_op;
	ft_tab[R_IN] = ft_check_redir;
	ft_tab[R_OUT] = ft_check_redir;
	ft_tab[R_APPEND] = ft_check_redir;
	ft_tab[R_HEREDOC] = ft_check_redir;
	ft_tab[O_BRACKET] = ft_check_Obracket;
	ft_tab[C_BRACKET] = ft_check_Cbracket;
	return (0);
}
