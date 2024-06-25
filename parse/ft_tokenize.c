#include "../minishell.h"

int	ft_tokenize(char *buffer, save_struct *t_struct, t_envp **env)
{
	t_cmd	*curr;
	int		bool_bracket;

	bool_bracket = 0;
	t_struct->envp = *env;
	ft_create_token_lst(buffer, t_struct);
	curr = t_struct->cmd;
	while (curr)
	{
		curr->bool_bracket = &bool_bracket;
		curr = curr->next;
	}
	ft_remove_null_node(&(t_struct->cmd));
	ft_clean_cmd_lst(&(t_struct->cmd), t_struct);
	ft_wildcard(&(t_struct->cmd));
	if (ft_exec_syntax_functions(&(t_struct->cmd), &(t_struct->envp)) == -1)
		return (-1);
	bool_bracket = 0;
	return (0);
}
