#include "../minishell.h"

int	ft_exit(save_struct *t_struct, t_envp **envp)
{
	char	**code;
	int		return_code;

	code = t_struct->cmd->cmd;
	if (!code[1])
	{
		ft_putstr_cmd_fd("Exit", 2, NULL, 0);
		return_code = ft_atoi(ft_search_var("?", envp));
		ft_free_envp_lst(envp);
		ft_all_free(t_struct);
		exit(return_code);
	}
	if ((((code[1][0] == '+' || code[1][0] == '-') && ft_str_isdigit(&code[1][1])) || ft_str_isdigit(code[1])) && !code[2])
	{
		ft_putstr_cmd_fd("Exit", 2, NULL, 0);
		if (code[1][0] == '-')
		{
			ft_free_envp_lst(envp);
			ft_all_free(t_struct);
			exit(156);
		}
		if (ft_atoi(code[1]) > 255)
		{
			return_code =  ft_atoi(code[1]) % 256;
			ft_free_envp_lst(envp);
			ft_all_free(t_struct);
			exit(return_code);
		}
		else 
		{
			return_code = ft_atoi(code[1]);
			ft_free_envp_lst(envp);
			ft_all_free(t_struct);
			exit(return_code);
		}
	}
	if (!ft_str_isdigit(code[1]) && !code[2])
	{
		ft_putstr_cmd_fd("exit", 2, NULL, 0);
		ft_putstr_cmd_fd("Minishell: exit: ", 2, NULL, 2);
		ft_putstr_cmd_fd(code[1], 2, NULL, 2);
		ft_putstr_cmd_fd(": numeric argument required", 2, NULL, 0);
		ft_free_envp_lst(envp);
		ft_all_free(t_struct);
		exit(2);
	}
	else if (ft_str_isdigit(code[1]) && code[2])
	{
		ft_putstr_cmd_fd("exit", 2, NULL, 0);
		ft_putstr_cmd_fd("Minishell: exit: too many arguments", 2, NULL, 0);
		ft_return_code(ft_strdup("1"), envp);
		exit(1);
	}
	else if (ft_str_is_alpha(code[1]))
	{
		ft_putstr_cmd_fd("exit", 2, NULL, 0);
		ft_putstr_cmd_fd("Minishell: exit: ", 2, NULL, 2);
		ft_putstr_cmd_fd(code[1], 2, NULL, 2);
		ft_putstr_cmd_fd(": numeric argument required", 2, NULL, 0);
		ft_free_envp_lst(envp);
		ft_all_free(t_struct);
		exit(156);
	}
	else if (ft_str_isdigit(code[1]) || ft_atoll(code[1]) > LONG_MAX
		|| ft_atoll(code[1]) < 0)
	{
		ft_putstr_cmd_fd("Minishell: exit: ", 2, NULL, 2);
		ft_putstr_cmd_fd(code[1], 2, NULL, 2);
		ft_putstr_cmd_fd(": numeric argument required", 2, NULL, 0);
		ft_putstr_cmd_fd("Exit", 2, NULL, 0);
		ft_free_envp_lst(envp);
		ft_all_free(t_struct);
		exit(2);
	}
	return (0);
}
