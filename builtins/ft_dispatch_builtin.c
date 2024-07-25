#include "../minishell.h"

int	ft_dispatch_builtin(t_cmd *cmd, save_struct *t_struct)
{
	if (!t_struct->cmd->cmd)
		return (-1);
	ft_expand(cmd, &t_struct->envp);
	if (!ft_strcmp(cmd->cmd[0], "echo"))
		return (ft_echo(cmd, &t_struct->envp));
	else if (!ft_strcmp(cmd->cmd[0], "export"))
		return (ft_export(cmd, &t_struct->envp));
	else if (!ft_strcmp(cmd->cmd[0], "exit"))
		return (ft_exit(t_struct, &t_struct->envp));
	else if (!ft_strcmp(cmd->cmd[0], "unset"))
		return (ft_unset(cmd->cmd, &t_struct->envp));
	else if (!ft_strcmp(cmd->cmd[0], "env"))
		return (ft_env(&t_struct->envp));
	else if (!ft_strcmp(cmd->cmd[0], "pwd"))
		return (ft_pwd(cmd->cmd, &t_struct->envp));
	else if (!ft_strcmp(cmd->cmd[0], "cd"))
		return (ft_cd(t_struct));
	return (-1);
}

int is_it_builtin(t_cmd *cmd, t_envp **env)
{
	if(!cmd->cmd || !cmd->cmd[0])
		return(0);
	ft_expand(cmd, env);
	ft_get_path(cmd);
	ft_remove_null_node(&cmd);
	if (!ft_strcmp(cmd->cmd[0], "echo"))
		return(1);
	else if (!ft_strcmp(cmd->cmd[0], "exit"))
		return(1);
	else if (!ft_strcmp(cmd->cmd[0], "pwd"))
		return(1);
	else if (!ft_strcmp(cmd->cmd[0], "env"))
		return(1);
	else if (!ft_strcmp(cmd->cmd[0], "unset"))
		return(1);
	else if (!ft_strcmp(cmd->cmd[0], "cd"))
		return(1);
	else if (!ft_strcmp(cmd->cmd[0], "export"))
		return(1);
	else
		return(0);
}
