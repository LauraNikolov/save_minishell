/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicolof <lnicolof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 14:41:19 by lauranicolo       #+#    #+#             */
/*   Updated: 2024/08/01 15:17:46 by lnicolof         ###   ########.fr       */
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

extern int g_exit_status;

int		main(int argc, char **argv, char **envp);

// tokenisation
t_cmd	*create_cmd_node(t_redir *redir, char **cmd, char **exp_code);
char	**ft_strdup_array(char **cmd);
int		ft_str_is_alpha(char *s);
int		ft_quote_len(char *s, int len);
int		ft_inside_quote(char *s, t_data_parsing *data, char **cmd,
			int *new_cmd_index);
int		ft_tokenize(char *buffer, t_save_struct *tstruct, t_envp **env);
int		ft_check_double_symbols(char *s, char **cmd);
int		ft_exec_func(t_cmd **cmd, t_envp **env,
			t_save_struct *tstruct);
void	ft_init_ft_tab(int (*ft_tab[6])(t_cmd *, t_envp **,
				t_save_struct *));
void	ft_get_path(t_cmd *node, t_save_struct *tstruct);
int		ft_putstr_cmd_fd(char *s, int fd, char **str, int flag);
void	ft_clean_cmd_lst(t_cmd **lst, t_save_struct *tstruct);
char	*ft_search_var(char *var, t_envp **env);
int		ft_bad_expression(t_cmd *node);
int		ft_is_enum(t_cmd *node, char **error_node);
int		ft_is_enum2(t_cmd *node, char **error_node);
int		ft_check_word(t_cmd *node, t_envp **env, t_save_struct *tstruct);
int		ft_check_redir(t_cmd *node, t_envp **env);
int		ft_check_redir2(t_cmd *node, t_envp **env);
int		ft_check_pipe(t_cmd *node, t_envp **env, t_save_struct *tstruct);
int		ft_check_cbracket(t_cmd *node, t_envp **env, t_save_struct *tstruct);
int		ft_count_bracket(t_cmd *node, char **error_cmd, t_envp **env);
void	ft_wildcard(t_cmd **lst);
t_redir	*ft_redir(char *s, int len, t_data_parsing *data);
t_redir	*ft_handle_quote(char *s, int len, t_data_parsing *data, char **cmd);
int		ft_match(char *cmd, char *dir);
void	ft_cpy_expand(char *s, t_data_parsing *data, t_envp **env,
			char **exp_code);
int		ft_cpy_expand_2(char *s, char **exp, int *j, t_envp **env);
int		ft_expand_len(char *s, t_envp **env, char **exp_code);
int		ft_expand_math(char *s, t_envp **env, int len);
int		ft_bad_subst(char *s);

// lst_proto
void	ft_save_envp(char **envp_tab, t_envp **envp_lst);
int		ft_return_code(char *code, t_envp **env);
void	ft_create_token_lst(char *buffer, t_save_struct *tstruct);
void	add_to_lst(t_cmd **head, t_cmd *new_node);
void	add_to_envp_lst(t_envp **head, t_envp *new_node);
void	ft_remove_null_node(t_cmd **lst);
t_cmd	*lst_last(t_cmd *node);
t_redir	*lst_last_redir(t_redir *node);
void	add_to_redir_lst(t_redir **head, t_redir *new_node);
t_envp	*lst_envp_last(t_envp *node);
void	ft_free_node(t_cmd *node);
void	ft_print_lst(t_cmd *node);
t_envp	*create_envp_node(char *var_name, int print_flag);
void	ft_free_tab(char **split);
void	ft_free_lst(t_cmd *lst);
void	ft_free_envp_lst(t_envp **lst, t_envp **env);
void	ft_all_free(t_save_struct *tstruct, int flag, char **buff);
int		ft_lst_size(t_cmd *cmd);
void	ft_print_env(t_envp **env);
void	ft_sort_env(t_envp **env);
void	ft_free_redir(t_redir *redir);
t_redir	*create_redir_node(char *s);
void	ft_expand(t_cmd *node, t_envp **env, t_save_struct *tstruct);

// General utils
int		ft_safe_malloc(char **s, int size, t_save_struct *tstruct);
void	ft_safe_free(char **s);
void	ft_override_content(char **s1, char *s2);
void	ft_swap_content(char **s1, char **s2);
int		ft_is_str(char c, char *s);
char	**ft_envp_to_char(t_envp **env, t_save_struct *tstruct);

// exec
void	ft_exec(t_save_struct *tstruct, char **envp);
int		ft_exec_single_cmd(t_save_struct *tstruct, char **envp);
void	ft_exec_multi_cmds(t_save_struct *tstruct, char **envp);
t_ast	*build_ast_recursive(t_cmd *start, t_cmd *end, t_ast *parent,
			t_save_struct *tstruct);
t_ast	*create_ast_node(t_cmd *node, t_ast *parent, t_save_struct *tstruct);
int		exec_ast_recursive(t_ast *root, char **envp, int return_value,
			t_save_struct *tstruct);
int		ft_exec_tree(t_ast *root);
int		exec_leaf(t_ast *root, char **envp, int return_value,
			t_save_struct *tstruct);
void	ft_parse_error(t_cmd *cmd);
int		redir_out(t_cmd *cmd);
int		redir_in(t_cmd *cmd);
int		apply_redir(t_cmd *cmd);
int		ft_execve_single_cmd(t_cmd *cmd, char ***envp, t_save_struct *tstruct);
void	manage_heredoc(t_cmd *cmd, t_save_struct *tstruct);
int		is_it_builtin(t_cmd *cmd, t_envp **env, t_save_struct *tstruct);
int		count_parenthesis(t_cmd *node);
int		recursive_free_ast(t_ast **ast);
int		ft_nbr_of_cmd(t_cmd *cmd);
char	*create_here_doc(char *str, char *limiter);
int		wait_for_child(t_cmd *cmd, int *return_value);
int		ft_execve_pipe(t_cmd *cmd, char **envp, t_ast *root,
			t_save_struct *tstruct);
int		get_return_code(t_cmd *cmd);
void	pipe_error(void);
int		dispatch_redir(t_redir *current, int *fd_in, int *fd_out);
int		open_redir_rappend(t_redir *current, int *fd_in, int *fd_out);
int		open_redir_rout(t_redir *current, int *fd_in, int *fd_out);
int		open_redir_in(t_redir *current, int *fd_in, int *fd_out);
void	exit_error(char *str, t_save_struct *tstruct);
int		ft_or(t_ast *root, char **envp, int return_value,
			t_save_struct *tstruct);
int		ft_or_recursive(t_ast *root, char **envp, int return_value,
			t_save_struct *tstruct);
int		ft_and(t_ast *root, char **envp, int return_value,
			t_save_struct *tstruct);
int		ft_and_recursive(t_ast *root, char **envp, int return_value,
			t_save_struct *tstruct);
int		ft_pipe(t_ast *root, char **envp, int return_value,
			t_save_struct *tstruct);
int		ft_pipe_recursive(t_ast *root, char **envp, int return_value,
			t_save_struct *tstruct);
t_cmd	*get_last_cmd(t_ast *node);
void redir_error(char *str);
int	ft_limiter(char *s1, char *s2);
void	heredoc_parent(pid_t pid, int file);

// BUILTINS
int		ft_fork_export(t_envp **env);
void	ft_compare_var(t_envp **env, char *var);
int		ft_handle_export_err(char *var);
int	ft_dispatch_builtin(t_cmd *cmd, t_save_struct *t_struct, int flag);
int		ft_export(t_cmd *cmd, t_envp **env);
int		ft_unset(char **var, t_envp **env);
int		ft_env(t_envp **envp);
int		ft_echo(t_cmd *cmd, t_envp **env, int flag);
int		ft_exit(t_save_struct *tstruct, t_envp **envp);
int		ft_print_envp(t_envp **envp);
int		ft_pwd(char **cmd, t_envp **envp);
int		ft_cd(t_save_struct *tstruct);
int	error_export2(char *var, t_envp **env);
// signal
void	ft_signal(int pid);
void	ft_handler_child_signals(int signal);
void	ft_handler_signals(int signal);

#endif