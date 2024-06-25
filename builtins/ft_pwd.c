#include "../minishell.h"

int	ft_pwd(t_envp **envp)
{
	int		i;
	char	*path;

	path = ft_search_var("PWD", envp);
	i = -1;
	if (path)
	{
		while (path[++i])
			write(1, &path[i], 1);
		write(1, "\n", 1);
	}
	else
		return (ft_return_code("127", envp));
	return (ft_return_code("0", envp));
}
