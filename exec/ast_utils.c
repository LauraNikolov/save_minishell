/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicolof <lnicolof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 15:45:40 by lnicolof          #+#    #+#             */
/*   Updated: 2024/07/23 12:03:32 by lnicolof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_ast	*create_ast_node(t_cmd *cmd, t_ast *parent, save_struct *t_struct)
{
	t_ast	*node;

	while (cmd->type == C_BRACKET || cmd->type == O_BRACKET)
	{
		cmd = cmd->next;
	}
	node = (t_ast *)malloc(sizeof(t_ast));
	if(!node)
		exit_error("malloc failed\n", t_struct);
	node->cmd = cmd;
	node->left = NULL;
	node->right = NULL;
	node->parent = parent;
	return (node);
}

// const char	*cmd_type_to_string(enum s_token_type type)
// {
// 	switch (type)
// 	{
// 	case AND:
// 		return ("AND");
// 	case OR:
// 		return ("OR");
// 	case PIPE:
// 		return ("PIPE");
// 	case WORD:
// 		return ("COMMAND");
// 		default:
// 		return ("UNKNOWN");
// 	}
// }

void	join_tree(t_ast *left, t_ast *right, t_ast *root)
{
	root->left = left;
	root->right = right;
}

// void	print_ast(t_ast *root, int depth, char prefix)
// {
// 	if (root == NULL)
// 	{
// 		return ;
// 	}
// 	if (depth > 0)
// 	{
// 		printf("%*s", depth * 4, "");
// 		printf("%c--- ", prefix);
// 	}
// 	if (root->cmd->cmd[0] && root->cmd->type == WORD)
// 	{
// 		printf("%s: %s\n", cmd_type_to_string(root->cmd->type),
// 			root->cmd->cmd[0]);
// 	}
// 	else
// 	{
// 		printf("%s\n", cmd_type_to_string(root->cmd->type));
// 	}
// 	print_ast(root->left, depth + 1, '|');
// 	print_ast(root->right, depth + 1, '`');
// }

static t_cmd	*find_logical_root(t_cmd *start, t_cmd *end)
{
	t_cmd	*current;
	t_cmd	*root;

	current = end;
	root = NULL;
	while (current != start)
	{
		if (current->type == AND || current->type == OR)
		{
			if (!root || count_parenthesis(current) < count_parenthesis(root))
				root = current;
		}
		current = current->prev;
	}
	return (root);
}

static t_cmd	*find_pipe_root(t_cmd *start, t_cmd *end)
{
	t_cmd	*current;

	current = end;
	while (current != start)
	{
		if (current->type == PIPE)
			return (current);
		current = current->prev;
	}
	return (NULL);
}

// Fonction principale
t_ast	*build_ast_recursive(t_cmd *start, t_cmd *end, t_ast *parent, save_struct *t_struct)
{
	t_cmd	*root;
	t_cmd	*right_start;
	t_ast	*root_node;
	t_ast	*gauche;
	t_ast	*droit;

	root = find_logical_root(start, end);
	if (!root)
		root = find_pipe_root(start, end);
	if (!root)
		return (create_ast_node(start, parent, t_struct));
	right_start = root->next;
	root_node = create_ast_node(root, parent, t_struct);
	gauche = build_ast_recursive(start, root->prev, root_node, t_struct);
	droit = build_ast_recursive(right_start, end, root_node, t_struct);
	join_tree(gauche, droit, root_node);
	return (root_node);
}
