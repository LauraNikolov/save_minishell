#include "../minishell.h"

static void	ft_echo_str(char **cmd, int option)
{
	int	i;

	i = 0;
	if (option)
		i = 1;
	while (cmd[++i])
	{
		ft_putstr_fd(cmd[i], 1);
		if (cmd[i + 1])
			ft_putchar_fd(' ', 1);
	}
	if (!option)
		ft_putchar_fd('\n', 1);
}

int	ft_echo(char **cmd, t_envp **env)
{
	int option;
	int i;

	if (!cmd[1] || !cmd || !*cmd)
	{
		ft_putchar_fd('\n', 2);
		return (ft_return_code("127", env));
	}
	option = 0;
	if (cmd[1][0] == '-' && cmd[1][1] == 'n')
	{
		option = 1;
		i = 2;
		while (cmd[1][i] == 'n')
			i++;
		if (cmd[1][i] != '\0')
			option = 0;
	}
	ft_echo_str(cmd, option);
	return (ft_return_code("0", env));
}