#include "../minishell.h"

static void	ft_remove_var(t_envp *node)
{
	if (!node)
		return ;
	if (node->next)
		node->next->prev = node->prev;
	else
		node->prev->next = NULL;
	if (node->prev)
		node->prev->next = node->next;
	else
		node->next->prev = NULL;
	free(node->var_name);
	free(node->var_value);
	free(node);
}

int	ft_unset(char **var, t_envp **env)
{
	t_envp	*curr;
	t_envp	*temp;
	int		i;

	curr = *env;
	while (curr)
	{
		i = 0;
		temp = curr->next;
		while (var[++i])
		{
			if (!ft_strcmp(curr->var_name, var[i]))
			{
				if (!curr->prev)
				{
					*env = curr->next;
					(*env)->prev = NULL;
				}
				ft_remove_var(curr);
			}
		}
		curr = temp;
	}
	return (ft_return_code(ft_strdup("0"), env));
}
