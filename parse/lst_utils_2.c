/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_utils_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicolof <lnicolof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 00:30:32 by renard            #+#    #+#             */
/*   Updated: 2024/08/01 13:27:15 by lnicolof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	if (var[i])
	{
		while (var[i] && var[i] != '=')
			i++;
		envp->var_name = ft_strndup(var, i);
	}
	else
		envp->var_name = NULL;
	envp->var_value = NULL;
	if (var[i + 1])
		envp->var_value = ft_strdup(&var[i + 1]);
	envp->print_flag = print_flag;
	envp->next = NULL;
	envp->prev = NULL;
	return (envp);
}

t_cmd	*create_cmd_node2(t_cmd *new_node, char **cmd)
{
	new_node->prev = NULL;
	new_node->path = NULL;
	new_node->bool_bracket = NULL;
	if (!ft_strcmp(*cmd, "||"))
		new_node->type = OR;
	else if (!ft_strcmp(*cmd, "|"))
		new_node->type = PIPE;
	else if (!ft_strcmp(*cmd, "&&"))
		new_node->type = AND;
	else if (!ft_strcmp(*cmd, "&"))
		new_node->type = NO_TYPE;
	else if (!ft_strcmp(*cmd, "("))
		new_node->type = O_BRACKET;
	else if (!ft_strcmp(*cmd, ")"))
		new_node->type = C_BRACKET;
	else
		new_node->type = WORD;
	free(*cmd);
	cmd = NULL;
	return (new_node);
}

t_cmd	*create_cmd_node(t_redir *redir, char **cmd, char **exp_code)
{
	t_cmd	*new_node;
	int		i;

	new_node = malloc(sizeof(t_cmd));
	if (!new_node)
		return (NULL);
	i = 0;
	while ((*cmd)[i] && (*cmd)[i] == ' ')
		i++;
	if (ft_strlen(*cmd) == (size_t)i)
		new_node->cmd = NULL;
	else
		new_node->cmd = ft_split(*cmd, " ");
	new_node->redir = redir;
	new_node->next = NULL;
	new_node->exp_code = NULL;
	if (exp_code && *exp_code)
	{
		new_node->exp_code = ft_strdup(*exp_code);
		ft_safe_free(exp_code);
	}
	return (create_cmd_node2(new_node, cmd), new_node);
}
