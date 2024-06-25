#include "minishell.h"

void	ft_handler_signals(int signal)
{
	if (signal == SIGINT)
	{
		ft_putchar_fd('\n', 2);
		// g_signal = 1;
	}
}
void	ft_all_free(save_struct *t_struct)
{
	if(t_struct->cmd)
		ft_free_lst(t_struct->cmd);
	if(t_struct->save_spaces)
		free(t_struct->save_spaces);
	if(t_struct)
		free(t_struct);
}

int	main(int ac, char **av, char **envp)
{
	char		*buffer;
	t_envp		*env;
	save_struct	*t_struct;

	(void)av;
	(void)ac;
	env = NULL;
	signal(SIGINT, ft_handler_signals);
	ft_save_envp(envp, &env);
	while (1)
	{
		t_struct = malloc(sizeof(save_struct));
		if (!t_struct)
			return (ft_free_envp_lst(&env), 0);
		ft_memset(t_struct, 0, sizeof(*t_struct));
		buffer = readline(CYAN "MINISHELL~ " RESET);
		if (!buffer)
			return (free(buffer), ft_all_free(t_struct), 0);
		add_history(buffer);
		ft_tokenize(buffer, t_struct, &env);
		// ft_dispatch_builtin(t_struct->cmd->cmd, t_struct);
		ft_exec(t_struct, envp);
		ft_print_lst(t_struct->cmd);
		// ft_envp_to_char(t_struct->envp);
		free(buffer);
		ft_all_free(t_struct);
	}
}
