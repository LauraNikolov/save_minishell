#include "../minishell.h"

static int	ft_changepwd_var(t_envp **env, char *pwd, char *old_pwd)
{
	t_envp	*curr;

	curr = *env;
	while (curr)
	{
		if (!ft_strcmp(curr->var_name, "OLDPWD"))
			ft_override_content(&curr->var_value, old_pwd);
		else if (!ft_strcmp(curr->var_name, "PWD"))
			ft_override_content(&curr->var_value, pwd);
		curr = curr->next;
	}
	return (0);
}

static int	ft_write_cderr(char *path, t_envp **env)
{
	ft_putstr_cmd_fd("minishell : cd: ", 2, NULL, 2);
	ft_putstr_cmd_fd(path, 2, NULL, 2);
	ft_putstr_cmd_fd(": No such file or directory", 2, NULL, 0);
	return (ft_return_code(ft_strdup("1"), env));
}

int	ft_cd(t_save_struct *t_struct)
{
	char	*path;
	char	old_pwd[PATH_MAX];
	char	pwd[PATH_MAX];

	if (t_struct->cmd->cmd[2])
	{
		ft_putstr_cmd_fd("minishell : cd: too many arguments", 2, NULL, 0);
		return (ft_return_code(ft_strdup("1"), &t_struct->envp));
	}
	getcwd(old_pwd, PATH_MAX);
	if (t_struct->cmd->cmd[1][0] != '/')
		path = ft_strjoin_path(old_pwd, t_struct->cmd->cmd[1]);
	else
		path = t_struct->cmd->cmd[1];
	if (!opendir(path))
		return (ft_write_cderr(t_struct->cmd->cmd[1], &t_struct->envp));
	if (!chdir(path))
	{
		getcwd(pwd, PATH_MAX);
		ft_changepwd_var(&t_struct->envp, pwd, old_pwd);
	}
	else
		return (ft_return_code(ft_strdup("1"), &t_struct->envp));
	return (ft_return_code(ft_strdup("0"), &t_struct->envp), 0);
}
