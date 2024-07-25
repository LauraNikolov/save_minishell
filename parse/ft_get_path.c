#include "../minishell.h"

int	ft_get_path(t_cmd *node)
{
	char	*path;
	char	*absolute_path;
	char	**bin;
	int		i;
	int     flag;

	if (!node->cmd)
		return (0);
	path = getenv("PATH");
	if(!path)
		return(0);
	if(path)
		bin = ft_split(path, ":");
	flag = 0;
	i = 0;
	while (bin && bin[i])
	{
		absolute_path = ft_strjoin_path(bin[i], node->cmd[0]);
		if (access(absolute_path, F_OK) == 0)
		{
			node->path = absolute_path;
			flag = 1;
			break ;
		}
		free(absolute_path);
		i++;
	}
	ft_free_tab(bin);
	if (!flag)
	{
		ft_safe_free(&node->path);
		node->path = ft_strdup(node->cmd[0]);
	}
	return (0);
}
