#include "../minishell.h"

int	redir_out(t_cmd *cmd)
{
	int		fd;
	t_redir	*current;
	t_redir	*save;

	current = cmd->redir;
	save = cmd->redir;
	fd = 1;
	if (!cmd->redir)
		return (-1);
	else
	{
		while (current)
		{
			if (current->type == R_OUT)
			{
				fd = open(current->next->redir, O_WRONLY | O_TRUNC | O_CREAT,
						0644);
			}
			else if (current->type == R_APPEND)
				fd = open(current->next->redir, O_WRONLY | O_APPEND | O_CREAT,
						0644);
			else
			{
				current = current->next;
				continue ;
			}
			if (fd == -1)
			{
				perror("minishell");
				return (-1);
			}
			if (!current->next->next)
				break ;
			else
				close(fd);
			current = current->next->next;
		}
	}
	cmd->redir = save;
	return (fd);
}
static void	ft_echo_str(t_cmd *cmd, int option, int i)
{
	//int	fd_in;
	//int fd_out;

	//fd_out = 1;
	//fd_in = 0;
	//if (cmd->redir)
	//{
		//if(cmd->std_in )
		//fd_in = redir_in(cmd);
		//if(fd_in != 0)
			//close(fd_in);
		//fd_out = redir_out(cmd);
		//if (fd_out == -1)
			//ft_putstr_fd("no such file or directory\n", 2);
	//}
	//if(cmd->std_in != 0)
		//close(cmd->std_in);
	while (cmd->cmd[i] && cmd->std_out != -1)
	{
		ft_putstr_fd(cmd->cmd[i], cmd->std_out);
		if (cmd->cmd[i + 1])
			ft_putchar_fd(' ', cmd->std_out);
		i++;
	}
	if (!option)
		ft_putchar_fd('\n', cmd->std_out);
	//if (cmd->std_out != 1)
		//close(cmd->std_out);
}

int	ft_echo(t_cmd *cmd, t_envp **env)
{
	int	option;
	int	i;
	int	j;

	// printf("\nJe passe dans echo\n");
	// printf("\nJ expand\n\n");
	// ft_expand(cmd, env);
	// ft_print_envp(env);

	// printf("cmd juste avant d'echo\n");
	// ft_print_lst(cmd);
	if (!cmd->cmd[1])
	{
		ft_putchar_fd('\n', 2);
		return (ft_return_code(ft_strdup("0"), env));
	}
	if (!cmd->cmd[1] || !cmd->cmd || !*cmd->cmd)
	{
		ft_putchar_fd('\n', 2);
		return (ft_return_code(ft_strdup("127"), env));
	}
	option = 0;
	i = 1;
	if (cmd->cmd[i][0] == '-')
	{
		while (cmd->cmd[i] && cmd->cmd[i][0] == '-')
		{
			if (cmd->cmd[i][0] == '-' && cmd->cmd[i][1] == 'n')
			{
				j = 1;
				while (cmd->cmd[i][j] && cmd->cmd[i][j] == 'n')
				{
					j++;
				}
				if (!cmd->cmd[i][j])
					option = 1;
				else
					break ;
			}
			else
				break ;
			i++;
		}
	}
	ft_echo_str(cmd, option, i);
	return (ft_return_code(ft_strdup("0"), env));
}
