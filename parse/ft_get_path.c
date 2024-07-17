#include "../minishell.h"

int	ft_get_path(t_cmd *node)
{
	char	*path;
	char	*absolute_path;
	char	**bin;
	int		i;

	if (!node->cmd)
		return (0);
	path = getenv("PATH");
	bin = ft_split(path, ":");
	i = 0;
	while (bin[i])
	{
		absolute_path = ft_strjoin_path(bin[i], node->cmd[0]);
		if (access(absolute_path, F_OK) == 0)
		{
			node->path = absolute_path;
			break ;
		}
		free(absolute_path);
		i++;
	}
	ft_free_tab(bin);
	return (0);
}
