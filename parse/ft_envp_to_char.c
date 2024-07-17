#include "../minishell.h"

static int	ft_lst_env_size(t_envp *env)
{
	int	i;

	i = 0;
	while (env)
	{
		env = env->next;
		i++;
	}
	return (i);
}

static int	ft_var_size(t_envp *node)
{
	int	i;
	int	len;

	len = 0;
	i = 0;
	if (!node)
		return (0);
	if (node->var_name)
		while (node->var_name[i])
		{
			i++;
			len++;
		}
	i = 0;
	if (node->var_value)
		while (node->var_value[i])
		{
			i++;
			len++;
		}
	return (len + 1);
}

char	**ft_envp_to_char(t_envp *env)
{
	char	**envp;
	int		j;
	int		k;
	int		l;

	l = 0;
	j = -1;
	envp = malloc(sizeof(char *) * (ft_lst_env_size(env)));
	if (!envp)
		return (NULL);
	while (env)
	{
		envp[++j] = malloc(sizeof(char) * (ft_var_size(env) + 1));
		if (!envp[j])
			return (NULL);
		k = 0;
		l = 0;
		while (env->var_name[l])
			envp[j][k++] = env->var_name[l++];
		envp[j][k++] = '=';
		l = 0;
		if (env->var_value)
		{
			while (env->var_value[l])
				envp[j][k++] = env->var_value[l++];
		}
		envp[j][k] = '\0';
		env = env->next;
	}
	envp[j] = NULL;
	return (envp);
}
