/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_parse.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicolof <lnicolof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 11:56:13 by lnicolof          #+#    #+#             */
/*   Updated: 2024/07/17 12:33:25 by lnicolof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <sys/wait.h>

void ft_parse_error(t_cmd *cmd)
{
	if(ft_strchr(cmd->cmd[0], '/') == -1)
	{
		dprintf(2, "%s: command not found\n", cmd->cmd[0]);
		exit(127);
	}
	else
	{
		dprintf(2 ,"minishell: %s: No such file or directory\n", cmd->cmd[0]);
		exit(126);
	}
}

int	ft_exec_single_cmd(save_struct *t_struct, char **envp)
{
	int return_value;
	pid_t pid;

	if ((return_value = ft_dispatch_builtin(t_struct->cmd, t_struct)) !=
		-1)
	{
		return (return_value);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (1);
	}
	if (pid == 0)
	{
		if(execve(t_struct->cmd->path, t_struct->cmd->cmd, envp) == -1)
			ft_parse_error(t_struct->cmd);
	}
	else
	{
		int status;
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			return_value = WEXITSTATUS(status);
	}
	return(return_value);
}

void close_fds(t_cmd *cmd_list)
{
    t_cmd *current = cmd_list; // Remplacez t_cmd par le type de votre structure de commande

    while (current)
    {
        // Ferme std_out si ce n'est pas la sortie standard
        if (current->std_out != 1)
        {
            close(current->std_out);
            current->std_out = 1; // Réinitialise à la sortie standard
        }

		// Ferme std_in si ce n'est pas l'entrée standard
		if(current->std_in != 0)
		{
			close(current->std_in);
			current->std_in = 0;
		}

		if(current->type == R_HEREDOC)
			free(current->next->redir);
        // Ajoutez ici des vérifications similaires pour std_in si nécessaire

        current = current->next; // Passe à la commande suivante dans la liste
	}
}


void destroy_tmp_file(t_cmd *cmd)
{
	t_cmd *current;
	t_redir *current_redir;

	current = cmd;
	while(current)
	{
		if(current->redir)
		{
			current_redir = current->redir;
			while(current_redir)
			{
				if(current_redir->type == R_HEREDOC)
					unlink(current_redir->next->redir);
				current_redir = current_redir->next->next;
			}
		}
		current = current->next;
	}
}

int ft_nbr_of_cmd(t_cmd *cmd)
{
	t_cmd *current;
	int i;

	current = cmd;
	i = 0;
	while(current)
	{
		if(current->type == WORD)
			i++;
		current = current->next;
	}
	return(i);
}

int recursive_free_ast(t_ast *ast)
{
	if(ast->left)
		recursive_free_ast(ast->left);
	if(ast->right)
		recursive_free_ast(ast->right);
	free(ast);
	return(0);
}



	void ft_exec(save_struct *t_struct, char **envp)
	{
		int cmd_size;
		int return_value;
		(void)return_value;

		cmd_size = ft_nbr_of_cmd(t_struct->cmd);
		if (cmd_size == 1)
		{
			t_struct->cmd->std_in = 0;
			t_struct->cmd->std_out = 1;
			manage_heredoc(t_struct->cmd);
			//apply_redir(t_struct->cmd);
			return_value = ft_execve_single_cmd(t_struct->cmd, &envp, t_struct);
			close_fds(t_struct->cmd);
			ft_return_code(ft_itoa(return_value), &t_struct->envp);
			destroy_tmp_file(t_struct->cmd);
		}
		else
		{
			manage_heredoc(t_struct->cmd);
			ft_exec_multi_cmds(t_struct, envp);
			close_fds(t_struct->cmd);
			destroy_tmp_file(t_struct->cmd);
			recursive_free_ast(t_struct->ast);
		}
	}
	