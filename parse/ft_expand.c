#include "../minishell.h"

int	ft_bad_subst(char *s)
{
	int	i;

	i = 0;
	while (s[i] && ft_isalnum(s[i]))
		i++;
	if (s[i] != '}' || i == 0)
		return (1);
	return (0);
}

int	ft_expand_len(char *s, char **exp, int exp_flag, t_envp **env)
{
	int		i;
	int		j;
	int		var_len;
	int		var_value_len;
	char	*var;
	char	*var_value;
	int		k;

	var_value_len = 0;
	j = 0;
	i = 0;
	while (s[i])
	{
		var_len = 0;
		if (s[i] == '$' && s[i + 1] == '{')
		{
			while (s[i + var_len] && s[i + var_len] != ' ')
				var_len++;
			if (ft_bad_subst(&s[i + 2]))
				ft_putstr_cmd_fd("minishell: bad_substition ", 2, &s, 0);
			var = ft_strndup(&s[i + 2], var_len - 3);
			var_value = ft_search_var(var, env);
			var_value_len += ft_strlen(var_value);
			if (var_value && exp_flag)
			{
				k = 0;
				while (var_value[k])
					(*exp)[j++] = var_value[k++];
			}
			i += var_len - 1;
			free(var);
			var_value = NULL;
		}
		else if (s[i] == '$' && (ft_isalnum(s[i + 1])))
		{
			while (s[i + var_len] && s[i + var_len] != ' ')
				var_len++;
			var = ft_strndup(&s[i + 1], var_len - 1);
			var_value = ft_search_var(var, env);
			var_value_len += ft_strlen(var_value);
			if (var_value && exp_flag)
			{
				k = 0;
				while (var_value[k])
					(*exp)[j++] = var_value[k++];
			}
			i += var_len - 1;
			free(var);
			var_value = NULL;
		}
		else if (exp_flag)
		{
			(*exp)[j] = s[i];
			j++;
		}
		i++;
	}
	return (i - var_len + var_value_len);
}

int	ft_expand(t_cmd *node, t_envp **env)
{
	int		i;
	char	*exp;

		i = -1;
		while (node->cmd[++i])
		{
			if (ft_is_char(node->cmd[i], '$') && node->expand_flag)
			{
				ft_safe_malloc(&exp, ft_expand_len(node->cmd[i], NULL, 0, env));
				exp[ft_expand_len(node->cmd[i], NULL, 0, env)] = '\0';
				ft_expand_len(node->cmd[i], &exp, 1, env);
				free(node->cmd[i]);
				node->cmd[i] = ft_strdup(exp);
				free(exp);
				exp = NULL;
			}
		}
	return (0);
}
