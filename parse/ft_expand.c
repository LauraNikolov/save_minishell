#include "../minishell.h"

static int	ft_bad_subst(char *s)
{
	int	i;

	i = 0;
	while (s[i] && ft_isalnum(s[i]))
		i++;
	if (s[i] != '}' || i == 0)
		return (1);
	return (0);
}

static int	ft_expand_len(char *s, char **exp, int exp_flag, t_envp **env)
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

static int ft_count_return_code(char *s, t_envp **env)
{
	int count;
	int i;

	count = 0;
	i = 0;
	while(s[i])
	{
		if(!ft_strncmp(&s[i], "$?", 2))
		{
			count += ft_strlen(ft_search_var("?", env));
			count -= 2;
		}
		i++;
	}
	return(count + ft_strlen(s));
}

static int ft_match_return_val(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (!ft_strncmp(&s[i], "$?", 2))
			return (1);
		i++;
	}
	return (0);
}

static char *ft_expand_return_var(char *s, t_envp **env)
{
	int i;
	int j;
	int k;
	char *res;
	char *var_value;

	j = 0;
	k = 0;
	i = 0;

	if (ft_match_return_val(s))
	{
		res = malloc(sizeof(char) * (ft_count_return_code(s, env) + 1));
		i = 0;
		while(s[i])
		{
			if (!ft_strncmp(&s[i], "$?", 2))
			{
				var_value = ft_search_var("?", env);
				k = 0;
				while(var_value[k])
					res[j++] = var_value[k++];
				i += 2;
			}
			res[j++] = s[i++];
		}
		res[j] = '\0';
		return(ft_strdup(res));
	}
	return (ft_strdup(s));
}

void	ft_expand(t_cmd *node, t_envp **env)
{
	int		i;
	char	*exp;

	i = -1;
	while (node->cmd[++i])
	{
		if (ft_is_char(node->cmd[i], '$') && node->expand_flag)
		{
			exp = ft_strdup(node->cmd[i]);
			free(node->cmd[i]);
			node->cmd[i] = ft_expand_return_var(exp, env);
			ft_safe_free(&exp);
			ft_safe_malloc(&exp, ft_expand_len(node->cmd[i], NULL, 0,
				env) + 1);
			exp[ft_expand_len(node->cmd[i], NULL, 0, env)] = '\0';
			ft_expand_len(node->cmd[i], &exp, 1, env);
			free(node->cmd[i]);
			node->cmd[i] = ft_strdup(exp);
			ft_safe_free(&exp);
		}
	}
}
