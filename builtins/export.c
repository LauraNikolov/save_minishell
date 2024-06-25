#include "../minishell.h"

void	ft_print_env(t_envp **env)
{
	t_envp	*curr;

	curr = *env;
	while (curr)
	{
		if (!ft_strcmp(curr->var_name, "?"))
		{
			curr = curr->next;
			continue ;
		}
		ft_putstr_fd("declare -x ", 1);
		if (curr->var_name[ft_strlen(curr->var_name) - 1] == '=')
		{
			ft_putstr_fd(curr->var_name, 1);
			write(1, "\"\"", 3);
			break ;
		}
		ft_putstr_fd(curr->var_name, 1);
		if (curr->var_value)
		{
			write(1, "=\"", 3);
			if (curr->var_value[0])
				ft_putstr_fd(curr->var_value, 1);
			write(1, "\"", 2);
		}
		write(1, "\n", 2);
		curr = curr->next;
	}
}

static void	ft_sort_env(t_envp **env, char **var)
{
	t_envp	*curr;

	if (!env || !*env || !*var)
		return ;
	curr = *env;
	while (curr->next)
	{
		if (ft_strcmp(curr->var_name, curr->next->var_name) > 0)
		{
			ft_swap_content(&curr->var_name, &curr->next->var_name);
			ft_swap_content(&curr->var_value, &curr->next->var_value);
			curr = *env;
		}
		else
			curr = curr->next;
	}
}

static void	ft_add_var(t_envp **env, char *var)
{
	t_envp	*curr;
	int		flag_add;
	int		equal;
	int		flag;

	flag = 0;
	flag_add = 0;
	if (ft_is_char(var, '='))
		equal = ft_strchr(var, '=');
	else
		equal = ft_strlen(var);
	curr = *env;
	// while(curr)
	// {
	// 	if(!strcmp(curr->var_name))
	// }
	while (curr)
	{
		printf("%s%s\n", curr->var_name, var);
		if (curr->var_name[ft_strlen(curr->var_name) - 1] == '=' && !ft_strncmp(curr->var_name, var, equal - 1))
			flag_add = 1;
		if (ft_strchr(curr->var_name, '=') && !ft_strncmp(curr->var_name, var, equal - 1))
		{
			flag = 1;
			flag_add = 1;
		}
		else if (!ft_strcmp(curr->var_name, var))
		{
			if (var[equal + 1])
				ft_override_content(&curr->var_value, &var[equal + 1]);
			flag_add = 1;
		}
		curr = curr->next;
	}
	if (var[equal] == '=' && !var[equal + 1])
		flag = 1;
	if (!flag_add)
		add_to_envp_lst(env, create_envp_node(var, flag));
}

static int	ft_fork_export(char **var, t_envp **env)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		ft_sort_env(env, var);
		ft_print_env(env);
		exit(0);
	}
	else
		wait(NULL);
	return (0);
}

int	ft_export(char **var, t_envp **env)
{
	int	i;

	if (!env || !*var)
		return (0);
	if (!var[1])
		return (ft_fork_export(var, env));
	i = 1;
	while (var[i])
	{
		if ((var[i][0] >= '0' && var[i][0] <= '9') || !ft_isalpha(var[i][0]))
		{
			ft_putstr_cmd_fd("Minishell : export: `", 2, NULL, 2);
			ft_putstr_cmd_fd(var[i], 2, NULL, 2);
			ft_putstr_cmd_fd("': not a valid identifier", 2, NULL, 0);
			ft_return_code("1", env);
		}
		else
			ft_add_var(env, var[i]);
		i++;
	}
	return (ft_return_code("0", env));
}
