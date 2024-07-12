#include "minishell.h"

void	ft_handler_signals(int signal)
{
	if (signal == SIGINT)
	{
		ft_putchar_fd('\n', 2);
	}
	else if (signal ==SIGQUIT )
	{
		ft_putstr_fd("\b\b  \b\b", 1);
	}
}

//int ft_signals()
//{
	//signal(SIGQUIT, SIG_IGN);
	//signal(SIGINT, ft_handler_signals);
//}

int	main(int ac, char **av, char **envp)
{
	char		*buffer;
	t_envp		*env;
	save_struct	*t_struct;

	(void)av;
	(void)ac;
	env = NULL;
	//ft_signal();
	ft_save_envp(envp, &env);
	while (1)
	{
		t_struct = malloc(sizeof(save_struct));
		if (!t_struct)
			return (ft_free_envp_lst(&env), 0);
		ft_memset(t_struct, 0, sizeof(*t_struct));
		buffer = readline((const char *)"minishell> ");
		if (!buffer)
			return (ft_free_envp_lst(&t_struct->envp), free(buffer), ft_all_free(t_struct), 0);
		add_history(buffer);
		ft_tokenize(buffer, t_struct, &env);
		//ft_print_lst(t_struct->cmd);
		ft_exec(t_struct, envp);
		free(buffer);
		ft_all_free(t_struct);
	}
	return(0);
}
