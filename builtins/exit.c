/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicolof <lnicolof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 23:20:36 by lnicolof          #+#    #+#             */
/*   Updated: 2024/07/31 23:22:19 by lnicolof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_exit_3(t_save_struct *tstruct, t_envp **envp, char **code)
{
	if (ft_str_is_alpha(code[1]))
	{
		ft_putstr_cmd_fd("exit", 2, NULL, 0);
		ft_putstr_cmd_fd("Minishell: exit: ", 2, NULL, 2);
		ft_putstr_cmd_fd(code[1], 2, NULL, 2);
		ft_putstr_cmd_fd(": numeric argument required", 2, NULL, 0);
		ft_free_envp_lst(envp, NULL);
		ft_all_free(tstruct, 0, NULL);
	}
	else if (ft_str_isdigit(code[1]) || ft_atoll(code[1]) > LONG_MAX
		|| ft_atoll(code[1]) < 0)
	{
		ft_putstr_cmd_fd("Minishell: exit: ", 2, NULL, 2);
		ft_putstr_cmd_fd(code[1], 2, NULL, 2);
		ft_putstr_cmd_fd(": numeric argument required", 2, NULL, 0);
		ft_putstr_cmd_fd("Exit", 2, NULL, 0);
		ft_free_envp_lst(envp, NULL);
		ft_all_free(tstruct, 0, NULL);
	}
	exit(2);
}

static int	ft_exit_2(t_save_struct *tstruct, t_envp **envp, char **code)
{
	if (!ft_str_isdigit(code[1]) && !code[2])
	{
		ft_putstr_cmd_fd("exit", 2, NULL, 0);
		ft_putstr_cmd_fd("Minishell: exit: ", 2, NULL, 2);
		ft_putstr_cmd_fd(code[1], 2, NULL, 2);
		ft_putstr_cmd_fd(": numeric argument required", 2, NULL, 0);
		ft_free_envp_lst(envp, NULL);
		ft_all_free(tstruct, 0, NULL);
		exit(2);
	}
	else if (ft_str_isdigit(code[1]) && code[2])
	{
		ft_putstr_cmd_fd("exit", 2, NULL, 0);
		ft_putstr_cmd_fd("Minishell: exit: too many arguments", 2, NULL, 0);
		ft_return_code(ft_strdup("1"), envp);
		exit(1);
	}
	return (ft_exit_3(tstruct, envp, code));
}

static int	ft_valid_exit(t_save_struct *tstruct, char **code)
{
	int		return_code;
	int		tmp_code;
	t_envp	**envp;

	envp = &tstruct->envp;
	ft_putstr_cmd_fd("Exit", 2, NULL, 0);
	if (code[1][0] == '-')
	{
		tmp_code = ft_atoll(code[1]);
		return_code = (256 + tmp_code) % 256;
	}
	if (ft_atoi(code[1]) > 255 || ft_atoi(code[1]) < -255)
		return_code = ft_atoi(code[1]) % 256;
	else
		return_code = ft_atoi(code[1]);
	ft_free_envp_lst(envp, NULL);
	ft_all_free(tstruct, 0, NULL);
	exit(return_code);
}

int	ft_exit(t_save_struct *tstruct, t_envp **envp)
{
	char	**code;
	int		return_code;

	code = tstruct->cmd->cmd;
	if (!code[1])
	{
		ft_putstr_cmd_fd("Exit", 2, NULL, 0);
		return_code = ft_atoi(ft_search_var("?", envp));
		ft_free_envp_lst(envp, NULL);
		ft_all_free(tstruct, 0, NULL);
		exit(return_code);
	}
	if ((((code[1][0] == '+' || code[1][0] == '-')
		&& ft_str_isdigit(&code[1][1])) || ft_str_isdigit(code[1]))
		&& !code[2])
		ft_valid_exit(tstruct, code);
	return (ft_exit_2(tstruct, envp, code));
}
