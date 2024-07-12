#include "../minishell.h"


int redir_out(t_cmd *cmd)
{
    int fd;
    //char *last_redir;
    t_redir *current = cmd->redir;
    t_redir *save = cmd->redir;

    fd = -1;
    if(!cmd->redir)
        return(-1);
    else
    {
        while(current)
        {
            if(current->type == R_OUT)
            { 
                fd = open(current->next->redir, O_WRONLY | O_TRUNC | O_CREAT, 0644);
            }
            else if(current->type == R_APPEND)
                fd = open(current->next->redir, O_WRONLY| O_APPEND | O_CREAT, 0644);
            else
            {
                current = current->next;
                continue;
            }
            if(fd == -1)
            {
                perror("minishell");
                return(-1);
            }
            if(!current->next->next)
                break;
            else
                close(fd);
            current = current->next->next;
        }
    }
     cmd->redir = save;
    return(fd);
}
static void	ft_echo_str(t_cmd *cmd, int option)
{
	int	i;
	int fd;

	fd = 1;
	i = 0;
	if(cmd->redir)
	{
		fd = redir_out(cmd);
		if(fd == -1)
			ft_putstr_fd("no such file or directory", 2);
	}
	if (option)
		i = 1;
	while (cmd->cmd[++i])
	{
		ft_putstr_fd(cmd->cmd[i], fd);
		if (cmd->cmd[i + 1])
			ft_putchar_fd(' ', fd);
	}
	if (!option)
		ft_putchar_fd('\n', fd);
	if(fd != 1)
		close(fd);
}

int	ft_echo(t_cmd *cmd, t_envp **env)
{
	int option;
	int i;

	if (!cmd->cmd[1] || !cmd->cmd || !*cmd->cmd)
	{
		ft_putchar_fd('\n', 2);
		return (ft_return_code("127", env));
	}
	option = 0;
	if (cmd->cmd[1][0] == '-' && cmd->cmd[1][1] == 'n')
	{
		option = 1;
		i = 2;
		while (cmd->cmd[1][i] == 'n')
			i++;
		if (cmd->cmd[1][i] != '\0')
			option = 0;
	}
	ft_echo_str(cmd, option);
	return (ft_return_code("0", env));
}