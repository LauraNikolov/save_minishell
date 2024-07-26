/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicolof <lnicolof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 14:41:19 by lauranicolo       #+#    #+#             */
/*   Updated: 2024/07/25 17:12:17 by lnicolof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include "struct.h"
# include <dirent.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>
# define CYAN "\x1b[36m"
# define RESET "\x1b[0m"

// ast utils
// void ft_build_ast_node()// ! TODO

int		main(int argc, char **argv, char **envp);

// libft TODO replace b the submodule

// tokenisation
t_cmd	*create_cmd_node(t_redir *redir, char **cmd, char c, save_struct *t_struct);
char	**ft_strdup_array(char **cmd);
int		ft_str_is_alpha(char *s);
int		ft_quote_len(char *s, int len);
int		ft_tokenize(char *buffer, save_struct *t_struct, t_envp **env);
int		ft_check_double_symbols(char *s, char **cmd);
int		ft_exec_syntax_functions(t_cmd **cmd, t_envp **env);
void	ft_init_ft_tab(int (*ft_tab[6])(t_cmd *, t_envp **));
int		ft_get_path(t_cmd *node);
t_redir	*ft_handle_quote(char *s, char **cmd, int len, save_struct *t_struct,
			int bufflen);
int		ft_putstr_cmd_fd(char *s, int fd, char **str, int flag);
void	ft_clean_cmd_lst(t_cmd **lst, save_struct *t_struct);
char	*ft_search_var(char *var, t_envp **env);

// Parsing
void	ft_wildcard(t_cmd **cmd);

// lst_proto
void	ft_save_envp(char **envp_tab, t_envp **envp_lst);
int		ft_return_code(char *code, t_envp **env);
void	ft_create_token_lst(char *buffer, save_struct *t_struct);
void	add_to_lst(t_cmd **head, t_cmd *new_node);
void	add_to_envp_lst(t_envp **head, t_envp *new_node);
void	ft_remove_null_node(t_cmd **lst);
t_cmd	*lst_last(t_cmd *node);
t_redir	*lst_last_redir(t_redir *node);
void	add_to_redir_lst(t_redir **head, t_redir *new_node);
t_envp	*lst_envp_last(t_envp *node);
void	ft_free_node(t_cmd *node);
t_envp	*create_envp_node(char *var_name, int print_flag);
void	ft_print_lst(t_cmd *node); // A SUPP A LA FIN
void	ft_free_tab(char **split);
void	ft_free_lst(t_cmd *lst);
void	ft_free_envp_lst(t_envp **lst);
void	ft_all_free(save_struct *t_struct);
int		ft_lst_size(t_cmd *cmd);
void	ft_print_env(t_envp **env);
void	ft_sort_env(t_envp **env);
void	ft_free_redir(t_redir *redir);
t_redir	*create_redir_node(char *s);

// General utils
int		ft_safe_malloc(char **s, int size);
void    ft_safe_free(char **s);
void	ft_override_content(char **s1, char *s2);
void	ft_swap_content(char **s1, char **s2);
int		ft_is_str(char c, char *s);
char	**ft_envp_to_char(t_envp *env);
int		ft_var_len(char *s, int brace_flag);

// expand
void 	ft_expand(t_cmd *node, t_envp **env);
// exec
void	ft_exec(save_struct *t_struct, char **envp);
int		ft_exec_single_cmd(save_struct *t_struct, char **envp);
void	ft_exec_multi_cmds(save_struct *t_struct, char **envp);
t_ast	*build_ast_recursive(t_cmd *start, t_cmd *end, t_ast *parent, save_struct *t_struct);
t_ast	*create_ast_node(t_cmd *node, t_ast *parent, save_struct *t_struct);
void	print_ast(t_ast *root, int depth, char prefix);
int		exec_ast_recursive(t_ast *root, char **envp,
			int return_value, save_struct *t_struct);
int		ft_exec_tree(t_ast *root);
int		exec_leaf(t_ast *root, char **envp, int return_value,
			save_struct *t_struct);
void	ft_parse_error(t_cmd *cmd);
int		redir_out(t_cmd *cmd);
int		redir_in(t_cmd *cmd);
int	    apply_redir(t_cmd *cmd);
int		ft_execve_single_cmd(t_cmd *cmd, char ***envp, save_struct *t_struct);
void	manage_heredoc(t_cmd *cmd, save_struct *t_struct);
int     is_it_builtin(t_cmd *cmd, t_envp **env);
int	count_parenthesis(t_cmd *node);
int	recursive_free_ast(t_ast **ast);
int	ft_nbr_of_cmd(t_cmd *cmd);
char	*create_here_doc(char *str, char *limiter);
int wait_for_child(t_cmd *cmd, int *return_value);int	
ft_execve_pipe(t_cmd *cmd, char **envp, t_ast *root, save_struct *t_struct);
int get_return_code(t_cmd *cmd);
void pipe_error(void);
int	dispatch_redir(t_redir *current, int *fd_in, int *fd_out);
int	open_redir_rappend(t_redir *current, int *fd_in, int *fd_out);
int	open_redir_rout(t_redir *current, int *fd_in, int *fd_out);
int	open_redir_in(t_redir *current, int *fd_in, int *fd_out);
void exit_error(char *str, save_struct *t_struct);
int	ft_or(t_ast *root, char **envp, int return_value, save_struct *t_struct);
int	ft_or_recursive(t_ast *root, char **envp, int return_value,
		save_struct *t_struct);
int	ft_and(t_ast *root, char **envp, int return_value, save_struct *t_struct);
int	ft_and_recursive(t_ast *root, char **envp, int return_value,
		save_struct *t_struct);
int	ft_pipe(t_ast *root, char **envp, int return_value, save_struct *t_struct);
int	ft_pipe_recursive(t_ast *root, char **envp, int return_value,
		save_struct *t_struct);
t_cmd	*get_last_cmd(t_ast *node);

// BUILTINS
int		ft_dispatch_builtin(t_cmd *cmd, save_struct *t_struct);
int		ft_export(t_cmd *cmd, t_envp **env);
int		ft_unset(char **var, t_envp **env);
int		ft_env(t_envp **envp);
int		ft_echo(t_cmd *cmd, t_envp **env);
int		ft_exit(save_struct *t_struct, t_envp **envp);
int		ft_print_envp(t_envp **envp);
int	    ft_pwd(char **cmd, t_envp **envp);
int		ft_cd(save_struct *t_struct);

// signal 
int ft_signal(int pid);
void ft_handler_child_signals(int signal);
void	ft_handler_signals(int signal);

// Faire appel a la fonction ft_get_path avant ou pendant l execution,
// y rajouter une fonction pour la gestion d erreurs ?

#endif