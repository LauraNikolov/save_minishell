#include "../minishell.h"

int	ft_env(t_envp **envp)
{
	if (!*envp || !envp)
	{
		ft_putstr_fd("minishell , environment is empty or null", 2);
		ft_return_code(ft_strdup("127"), envp);
	}
	ft_print_envp(envp);
	return (0);
}
