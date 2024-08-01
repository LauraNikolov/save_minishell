/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_utils_3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicolof <lnicolof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 00:30:35 by renard            #+#    #+#             */
/*   Updated: 2024/08/01 14:30:08 by lnicolof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
		if (curr->cmd)
		{
			printf("\nCommande n%d = %s\n", command_num, curr->cmd[0]);
			while (curr->cmd[i])
			{
				printf("Options n%d : %s\n", i, curr->cmd[i]);
				i++;
			}
		}
		printf("Path = %s\n", curr->path);
		if (curr->redir)
		{
			curr_redir = curr->redir;
			while (curr_redir)
			{
				printf("redir = %s | type = ", curr_redir->redir);
				if (curr_redir->type == 6)
					printf(" R_IN\n");
				else if (curr_redir->type == 0)
					printf("INFILE\n");
				else if (curr_redir->type == 7)
					printf(" R_OUT\n");
				else if (curr_redir->type == 8)
					printf("R_APPEND\n");
				else if (curr_redir->type == 9)
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

void	ft_free_envp_lst(t_envp **lst, t_envp **env)
{
	t_envp	*curr;
	t_envp	*temp;

	if (!*lst)
		ft_free_envp_lst(env, NULL);
	curr = *lst;
	while (curr)
	{
		temp = curr->next;
		free(curr->var_name);
		free(curr->var_value);
		free(curr);
		curr = temp;
	}
}

void	ft_free_redir(t_redir *redir)
{
	t_redir	*tmp;

	while (redir)
	{
		tmp = redir->next;
		free(redir->redir);
		free(redir);
		redir = tmp;
	}
}

void	ft_free_node(t_cmd *node)
{
	if (node->cmd)
		ft_free_tab(node->cmd);
	free(node->path);
	free(node);
}

void	ft_free_lst(t_cmd *lst)
{
	t_cmd	*temp;

	while (lst)
	{
		temp = lst->next;
		if (lst->redir)
			ft_free_redir(lst->redir);
		ft_free_node(lst);
		lst = temp;
	}
}
