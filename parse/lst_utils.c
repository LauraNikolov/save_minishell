#include "../minishell.h"

int	ft_lst_size(t_cmd *cmd)
{
	t_cmd	*save;
	int		i;

	i = 0;
	save = cmd;
	while (cmd)
	{
		cmd = cmd->next;
		i++;
	}
	cmd = save;
	return (i);
}

void	ft_free_envp_lst(t_envp **lst)
{
	t_envp	*curr;
	t_envp	*temp;

	curr = *lst;
	while (curr)
	{
		temp = curr->next;
		free(curr->var_name);
		free(curr->var_value);
		free(curr);
		curr = temp;
	}
	*lst = NULL;
}

void	ft_free_node(t_cmd *node)
{
	int	i;

	i = 0;
	while (node->cmd[i])
	{
		free(node->cmd[i]);
		i++;
	}
	free(node->cmd);
	free(node->path);
	free(node);
}
void	ft_free_lst(t_cmd *lst)
{
	t_cmd	*temp;

	while (lst)
	{
		temp = lst->next;
		ft_free_node(lst);
		lst = temp;
	}
}

int	ft_print_envp(t_envp **envp)
{
	t_envp	*curr;

	curr = *envp;
	while (curr)
	{
		if (!ft_strcmp(curr->var_name, "?"))
		{
			curr = curr->next;
			continue ;
		}
		printf("%s", curr->var_name);
		printf("=%s\n", curr->var_value);
		curr = curr->next;
	}
	return (0);
}

void	ft_print_lst(t_cmd *node)
{
	t_cmd	*curr;
	t_redir	*curr_redir;
	int		i;
	int		command_num;

	i = 1;
	command_num = 1;
	curr = node;
	while (curr)
	{
		printf("\nCommande n%d = %s\n", command_num, curr->cmd[0]);
		while (curr->cmd[i])
		{
			printf("Options n%d : %s\n", i, curr->cmd[i]);
			i++;
		}
		printf("Path = %s\n", curr->path);
		if (curr->redir)
		{
			curr_redir = curr->redir;
			while (curr_redir)
			{
				printf("redir = %s | type = ", curr_redir->redir);
				if (curr_redir->type == 4)
					printf(" R_IN\n");
				else if (curr_redir->type == 0)
					printf("INFILE\n");
				else if (curr_redir->type == 5)
					printf(" R_OUT\n");
				else if (curr_redir->type == 6)
					printf("R_APPEND\n");
				else if (curr_redir->type == 7)
					printf("HEREDOC\n");
				curr_redir = curr_redir->next;
			}
		}
		if (curr->type == 0)
			printf("WORD\n");
		else if (curr->type == 1)
			printf("PIPE\n");
		else if (curr->type == 2)
			printf("AND\n");
		else if (curr->type == 3)
			printf("OR\n");
		else if (curr->type == 8)
			printf("O_BRACKET\n");
		else if (curr->type == 9)
			printf("C_BRACKET\n");
		printf("\n----\n");
		command_num++;
		curr = curr->next;
		i = 1;
	}
}

t_cmd	*lst_last(t_cmd *node)
{
	t_cmd	*curr;

	curr = node;
	while (curr->next)
		curr = curr->next;
	return (curr);
}

t_redir	*lst_last_redir(t_redir *node)
{
	t_redir	*curr;

	if (!node)
		return (NULL);
	curr = node;
	while (curr->next)
		curr = curr->next;
	return (curr);
}

t_envp	*lst_envp_last(t_envp *node)
{
	t_envp	*curr;

	if (!node)
		return (NULL);
	curr = node;
	while (curr->next)
		curr = curr->next;
	return (curr);
}

void	add_to_lst(t_cmd **head, t_cmd *new_node)
{
	t_cmd	*last;

	if (!*head)
	{
		*head = new_node;
		return ;
	}
	last = lst_last(*head);
	new_node->prev = last;
	last->next = new_node;
}

void	add_to_envp_lst(t_envp **head, t_envp *new_node)
{
	t_envp	*last;

	if (!*head)
	{
		*head = new_node;
		return ;
	}
	last = lst_envp_last(*head);
	new_node->prev = last;
	last->next = new_node;
}
t_envp	*create_envp_node(char *var, int print_flag)
{
	t_envp	*envp;
	int		i;

	envp = NULL;
	envp = malloc(sizeof(t_envp));
	if (!envp || !var)
		return (NULL);
	i = 0;
	while (var[i] && var[i] != '=')
		i++;
	envp->var_name = ft_strndup(var, i);
	if (!var[i])
		envp->var_value = NULL;
	else if (var[i + 1])
		envp->var_value = ft_strdup(&var[i + 1]);
	envp->print_flag = print_flag;
	envp->next = NULL;
	envp->prev = NULL;
	return (envp);
}

t_cmd	*create_cmd_node2(t_cmd *new_node, char *cmd)
{
	if (!ft_strcmp(cmd, "||"))
		new_node->type = OR;
	else if (!ft_strcmp(cmd, "|"))
		new_node->type = PIPE;
	else if (!ft_strcmp(cmd, "&&"))
		new_node->type = AND;
	else if (!ft_strcmp(cmd, "&"))
		new_node->type = NO_TYPE;
	else if (!ft_strcmp(cmd, "("))
		new_node->type = O_BRACKET;
	else if (!ft_strcmp(cmd, ")"))
		new_node->type = C_BRACKET;
	else
		new_node->type = WORD;
	free(cmd);
	return (new_node);
}

t_cmd	*create_cmd_node(t_redir *redir, char *cmd, char c)
{
	t_cmd	*new_node;

	new_node = malloc(sizeof(t_cmd));
	if (!new_node)
		return (NULL);
	new_node->cmd = NULL;
	if (cmd)
		new_node->cmd = ft_split(cmd, " ");
	new_node->redir = redir;
	new_node->expand_flag = 0;
	new_node->next = NULL;
	new_node->prev = NULL;
	new_node->path = NULL;
	new_node->bool_bracket = NULL;
	if (c != '\'')
		new_node->expand_flag = 1;
	create_cmd_node2(new_node, cmd);
	return (new_node);
}
