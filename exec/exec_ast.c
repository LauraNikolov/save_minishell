/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lauranicoloff <lauranicoloff@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 18:33:30 by lnicolof          #+#    #+#             */
/*   Updated: 2024/07/13 12:33:05 by lauranicolo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <errno.h>



t_cmd *get_last_cmd(t_ast *node) {
    if (node == NULL) {
        return NULL; // Arbre vide ou fin de branche
    }
    if (node->left == NULL && node->right == NULL) {
        return node->cmd; // Retourne la commande du nœud actuel
    }
    if (node->right != NULL) {
        return get_last_cmd(node->right);
    }
    return get_last_cmd(node->left);
}


int get_return_code(t_cmd *cmd)
{
	int return_value;
	int status;

	return_value = 0;
	status = 0;
	waitpid(cmd->pid, &status, 0);
	if (WIFEXITED(status))
		return_value = WEXITSTATUS(status);
	while(wait(&status) > 0);
	return (return_value);
}

int	get_nbr_of_pipe(t_cmd *cmd)
{
	int	i;

	i = 1;
	while (cmd)
	{
		if (cmd->type == PIPE)
			i++;
		cmd = cmd->next;
	}
	return (i);
}

int	ft_execve_single_cmd(t_cmd *cmd, char ***envp, save_struct *t_struct)
{
	int		return_value;
	int		status;
	int 	test;
	char **new_envp;

	(void)t_struct;
	return_value = 0;
	if ((test = ft_dispatch_builtin(cmd, t_struct)) != -1)
	{
		new_envp = ft_envp_to_char(t_struct->envp);
		ft_free_tab(*envp);
		envp = NULL;
		envp = &new_envp;
		return (test);
	}
	cmd->pid = fork();
	if (cmd->pid == -1)
	{
		perror("fork");
		return (1);
	}
	if (cmd->pid == 0)
	{
		if (cmd->std_in != STDIN_FILENO)
		{
			dup2(cmd->std_in, STDIN_FILENO);
			close(cmd->std_in);
		}
		if (cmd->std_out != STDOUT_FILENO)
		{
			dup2(cmd->std_out, STDOUT_FILENO);
			close(cmd->std_out);
		}
		execve(cmd->path, cmd->cmd, *envp);		
		exit(-1);
	}
	else
	{
		status = 0;
		waitpid(cmd->pid, &status, 0);
		if (WIFEXITED(status))
		{
			if(status == -1)
				ft_parse_error(cmd);
			return_value = WEXITSTATUS(status);
		}
	}
	return (return_value);
}

// int	ft_wait_all_child(void)
// {
// 	int return_value = 0;
// 	int		status;
// 	pid_t	pid;

// 	dprintf(2, "salut je passe par la\n");
// 	while ((pid = waitpid(-1, &status, NULL)) > 0)
// 	{
// 		dprintf(2, "once\n");
// 		if (WIFEXITED(status))
// 			return_value = WEXITSTATUS(status);
// 	}
// 	if (pid == -1 && errno != ECHILD)
// 	{
// 		// Une erreur s'est produite avec waitpid
// 		perror("waitpid");
// 		return (-1);
// 	}
// 	return(return_value);
// }

int	ft_execve_pipe(t_cmd *cmd, char **envp, t_ast *root, save_struct *t_struct,
		t_ast *save_root)
{
	int		return_value;
	int test;

	(void)save_root;
	return_value = 0;
	cmd->pid = fork();
	if (cmd->pid == -1)
	{
		perror("fork");
		return (1);
	}
	if (cmd->pid == 0)
	{
		if (cmd->std_in != STDIN_FILENO)
		{
			dup2(cmd->std_in, STDIN_FILENO);
			close(cmd->std_in);
		}
		if (cmd->std_out != STDOUT_FILENO)
		{
			dup2(cmd->std_out, STDOUT_FILENO);
			close(cmd->std_out);
		}
		// Fermer les descripteurs de pipe inutilisés dans le processus enfant
		close(root->cmd->pipe[0]);
		if ((test = ft_dispatch_builtin(cmd, t_struct)) != -1)
			exit(test);
		else
		{
			if (execve(cmd->path, cmd->cmd, envp) == -1)
				ft_parse_error(cmd);
			else
				exit(-1);
		}
	}
	else
	{
		// Fermer les descripteurs inutilisés dans le processus parent
		close(root->cmd->pipe[1]);
		if (root->cmd->prev_fd != -1)
			close(root->cmd->prev_fd);
		if (cmd->std_out != STDOUT_FILENO)
			root->cmd->prev_fd = root->cmd->pipe[0];
	}
	return (return_value);
}

int	exec_leaf(t_ast *root, char **envp, t_ast *save_root, int return_value,
		save_struct *t_struct)
{
	t_cmd	*cmd1;
	t_cmd	*cmd2;

	// int		i;
	if (root->cmd->type == WORD)
		return (0);
	if (root->left->cmd->type == WORD && root->right->cmd->type == WORD)
	{
		cmd1 = root->left->cmd;
		cmd2 = root->right->cmd;
		if (root->cmd->type == PIPE)
		{
			if (pipe(root->cmd->pipe) == -1)
			{
				perror("pipe");
				return (1);
			}
			// cmd1: stdin est standard, stdout est l'entrée du pipe
			if (root->cmd->prev_fd == -1)
				cmd1->std_in = STDIN_FILENO;
			else
				cmd1->std_in = root->cmd->prev_fd;
			cmd1->std_out = root->cmd->pipe[1];
			apply_redir(cmd1);
			return_value = ft_execve_pipe(cmd1, envp, root, t_struct,
					save_root);
			// cmd2: stdin est la sortie du pipe précédent,
			if (pipe(root->cmd->pipe) == -1)
			{
				perror("pipe");
				return (1);
			}
			cmd2->std_in = root->cmd->prev_fd;
			// if(root != save_root)
			// cmd2->std_out = root->cmd->pipe[1];
			if (root != save_root->right)
				root->right->cmd->std_out = root->cmd->pipe[1];
			if (root == save_root)
				cmd2->std_out = STDOUT_FILENO;
			if (root->parent)
			{
				if (root->parent->cmd->type == OR
					|| root->parent->cmd->type == AND)
					cmd2->std_out = STDOUT_FILENO;
			}
			apply_redir(cmd2);
			return_value = ft_execve_pipe(cmd2, envp, root, t_struct,
					save_root);
			if (root == save_root || root->parent->cmd->type == OR || root->parent->cmd->type == AND)
			{
				if(root->cmd->prev_fd != -1)
					close(root->cmd->prev_fd);
				return_value = get_return_code(cmd2);
			}
		}
		else if (root->cmd->type == AND)
		{
			apply_redir(cmd1);
			return_value = ft_execve_single_cmd(cmd1, &envp, t_struct);
			if (return_value == 0)
			{
				apply_redir(cmd2);
				return_value = ft_execve_single_cmd(cmd2, &envp, t_struct);
			}
		}
		else if (root->cmd->type == OR)
		{
			apply_redir(cmd1);
			return_value = ft_execve_single_cmd(cmd1, &envp, t_struct);
			if (return_value != 0)
			{
				apply_redir(cmd2);
				return_value = ft_execve_single_cmd(cmd2, &envp, t_struct);
			}
		}
	}
	return (return_value);
}

int	ft_pipe_recursive(t_ast *root, char **envp, t_ast *save_root,
		int return_value, save_struct *t_struct)
{
	if (root->right->cmd->type == PIPE || root->right->cmd->type == AND
		|| root->right->cmd->type == OR)
	{
		root->right->cmd->prev_fd = root->left->cmd->prev_fd;
		return_value = exec_ast_recursive(root->right, envp, save_root,
				return_value, t_struct);
	}
	else
	{
		pipe(root->cmd->pipe);
		root->right->cmd->std_in = root->left->cmd->prev_fd;
		if (root != save_root->right)
			root->right->cmd->std_out = root->cmd->pipe[1];
		if (root == save_root)
			root->right->cmd->std_out = STDOUT_FILENO;
		apply_redir(root->right->cmd);
		return_value = ft_execve_pipe(root->right->cmd, envp, root, t_struct,
				save_root);
	}
	return (return_value);
}

int	ft_pipe(t_ast *root, char **envp, t_ast *save_root, int return_value,
		save_struct *t_struct)
{
	return (ft_pipe_recursive(root, envp, save_root, return_value, t_struct));
}

int	ft_and_recursive(t_ast *root, char **envp, t_ast *save_root,
		int return_value, save_struct *t_struct)
{
	if (root->right->cmd->type == PIPE || root->right->cmd->type == AND
		|| root->right->cmd->type == OR)
	{
		if (return_value == 0)
			return_value = exec_ast_recursive(root->right, envp, save_root,
					return_value, t_struct);
		else
			return (return_value);
	}
	else
	{
		if (return_value == 0)
		{
			apply_redir(root->right->cmd);
			return_value = ft_execve_single_cmd(root->right->cmd, &envp,
					t_struct);
		}
		else
			return (return_value);
	}
	return (return_value);
}

int	ft_and(t_ast *root, char **envp, t_ast *save_root, int return_value,
		save_struct *t_struct)
{
	return (ft_and_recursive(root, envp, save_root, return_value, t_struct));
}

int	ft_or_recursive(t_ast *root, char **envp, t_ast *save_root,
		int return_value, save_struct *t_struct)
{
	if (root->right->cmd->type == PIPE || root->right->cmd->type == AND
		|| root->right->cmd->type == OR)
	{
		if (return_value != 0)
		{
			return_value = exec_ast_recursive(root->right, envp, save_root,
					return_value, t_struct);
		}
		else
			return (return_value);
	}
	else
	{
		if (return_value != 0)
		{
			apply_redir(root->right->cmd);
			return_value = ft_execve_single_cmd(root->right->cmd, &envp,
					t_struct);
		}
		else
			return (return_value);
	}
	return (return_value);
}

int	ft_or(t_ast *root, char **envp, t_ast *save_root, int return_value,
		save_struct *t_struct)
{
	return (ft_or_recursive(root, envp, save_root, return_value, t_struct));
}

void	ft_handle_ast_recursive(t_ast *root, char **envp, t_ast *save_root,
		int *return_value, save_struct *t_struct)
{
	if(root->cmd->type == AND || root->cmd->type == OR)
	{
		if(root->left->cmd->type == WORD)
		{
			apply_redir(root->left->cmd);
			*return_value = ft_execve_single_cmd(root->left->cmd, &envp, t_struct);
		}	
	}

	if (root->cmd->type == AND)
	{
		// read_pipe(root->left->cmd->prev_fd);
		// if (root->left->cmd->type == PIPE || root->left->cmd->type == PIPE)
		// {
		// 	print_ast(root, 0, '|');
		// 	dprintf(2, "icicrecusrive\n");
		// 	if(root->cmd->prev_fd != -1)
		// 			close(root->cmd->prev_fd);
		// 	*return_value = get_return_code(get_last_cmd(root->left));
		// 	// int status;
		// 	// int pid;
		// 	// pid = 0;
		// 	// status = 0;
		// 	// while (pid != -1)
		// 	// {
		// 	// 	pid = wait(&status);
		// 	// 	if (WIFEXITED(status))
		// 	// 		*return_value = WEXITSTATUS(status);
		// 	// }
		// }
		if (*return_value == 0)
			*return_value = exec_ast_recursive(root->right, envp, save_root,
					*return_value, t_struct);
		else
			return ;
	}
	else if (root->cmd->type == OR)
	{
		// if (root->left->cmd->type == PIPE || root->right->cmd->type == PIPE)
		// {
		// 	if(root->cmd->prev_fd != -1)
		// 			close(root->cmd->prev_fd);
		// 	*return_value = get_return_code(get_last_cmd(root->left));
		// }
		// read_pipe(root->left->cmd->prev_fd);
		if (*return_value != 0)
			*return_value = exec_ast_recursive(root->right, envp, save_root,
					*return_value, t_struct);
		else
			return ;
	}
	else
	{
		root->right->cmd->prev_fd = root->left->cmd->prev_fd;
		*return_value = exec_ast_recursive(root->right, envp, save_root,
				*return_value, t_struct);
	}
}

void	ft_handle_exec(t_ast *root, char **envp, t_ast *save_root,
		int *return_value, save_struct *t_struct)
{
	//dprintf(2, "root->cmd->type == %d\n", root->cmd->type);
	if (root->cmd->type == PIPE)
	{
		pipe(root->cmd->pipe);
		root->right->cmd->std_in = root->left->cmd->prev_fd;
		if (root != save_root->right)
			root->right->cmd->std_out = root->cmd->pipe[1];
		if (root == save_root || root->parent->cmd->type == OR
			|| root->parent->cmd->type == AND)
			root->right->cmd->std_out = STDOUT_FILENO;
		apply_redir(root->right->cmd);
		*return_value = ft_execve_pipe(root->right->cmd, envp, root, t_struct,
				save_root);
		if(root == save_root || root->parent->cmd->type == OR || root->parent->cmd->type == AND)
		{
			//print_ast(root, 0, '|');
			if(root->cmd->prev_fd != -1)
					close(root->cmd->prev_fd);
			*return_value = get_return_code(get_last_cmd(root));
		}
		// if (root == save_root)
		// {

		// 	if(root->cmd->prev_fd != -1)
		// 			close(root->cmd->prev_fd);
		// 	int status;
		// 	int pid;
		// 	pid = 0;
		// 	status = 0;
		// 	while (pid != -1)
		// 	{
		// 		pid = wait(&status);
		// 		if (WIFEXITED(status))
		// 			*return_value = WEXITSTATUS(status);
		// 	}
		// }
	}
	else if (root->cmd->type == AND)
	{
		// if (root->left->cmd->type == PIPE || root->right->cmd->type == PIPE)
		// {
		// 	if(root->cmd->prev_fd != -1)
		// 			close(root->cmd->prev_fd);
		// 	*return_value = get_return_code(get_last_cmd(root->left));
		// 	// int status;
		// 	// int pid;
		// 	// pid = 0;
		// 	// status = 0;
		// 	// while (pid != -1)
		// 	// {
		// 	// 	pid = wait(&status);
		// 	// 	if (WIFEXITED(status))
		// 	// 		*return_value = WEXITSTATUS(status);
		// 	// 	dprintf(2, "return_value === %d\n", *return_value);
		// 	// }
		// }
		if (*return_value == 0)
		{
			apply_redir(root->right->cmd);
			*return_value = ft_execve_single_cmd(root->right->cmd, &envp,
					t_struct);
		}
	}
	else if (root->cmd->type == OR)
	{
		// if (root->left->cmd->type == PIPE || root->right->cmd->type == PIPE)
		// {
		// 	if(root->cmd->prev_fd != -1)
		// 			close(root->cmd->prev_fd);
		// 	*return_value = get_return_code(get_last_cmd(root->left));
		// 	// int status;
		// 	// int pid;
		// 	// pid = 0;
		// 	// status = 0;
		// 	// while (pid != -1)
		// 	// {
		// 	// 	pid = wait(&status);
		// 	// 	if (WIFEXITED(status))
		// 	// 		*return_value = WEXITSTATUS(status);
		// 	}
		// 	dprintf(2, "%d\n", *return_value);
			if (*return_value != 0)
			{
				apply_redir(root->right->cmd);
				*return_value = ft_execve_single_cmd(root->right->cmd, &envp,
						t_struct);
			}
		}
}


	

int	exec_ast_recursive(t_ast *root, char **envp, t_ast *save_root,
		int return_value, save_struct *t_struct)
{
	//print_ast(root, 0, '|');
	if (root == NULL)
		return (return_value);
	if (root->left->cmd->type == PIPE || root->left->cmd->type == AND
		|| root->left->cmd->type == OR)
		return_value = exec_ast_recursive(root->left, envp, save_root,
				return_value, t_struct);
	if (root->left->cmd->type == WORD && root->right->cmd->type == WORD)
		return_value = exec_leaf(root, envp, save_root, return_value, t_struct);
	else
	{
		if (root->right->cmd->type == PIPE || root->right->cmd->type == AND
			|| root->right->cmd->type == OR)
		{
			//dprintf(2, "ici\n");
			ft_handle_ast_recursive(root, envp, save_root, &return_value,
				t_struct);
		}
		else
		{	
			//dprintf(2, "la\n");
			ft_handle_exec(root, envp, save_root, &return_value, t_struct);
		}
	}
	return (return_value);
}

/*
 * une fonction qui execute une commande simple :
 * une fonction qui set le pipe
 * une fonction qui set le AND
 * une fonction qui set le OR
 * une fonction qui gere les enfants
 * une fonction qui gere le parent
 */
