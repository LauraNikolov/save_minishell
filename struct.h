/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicolof <lnicolof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 14:15:39 by melmarti          #+#    #+#             */
/*   Updated: 2024/07/31 22:42:02 by lnicolof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

# include "minishell.h"

typedef enum s_token_type
{
	WORD,
	PIPE,
	AND,
	OR,
	O_BRACKET,
	C_BRACKET,
	R_IN,
	R_OUT,
	R_APPEND,
	R_HEREDOC,
	NO_TYPE,
}					t_token_type;

typedef struct s_cmd
{
	pid_t			pid;
	char			**cmd;
	char			*path;
	int				prev_fd;
	int				std_out;
	int				std_in;
	int				pipe[2];
	int				return_value;
	int				*bool_bracket;
	char			*exp_code;
	struct s_redir	*redir;
	struct s_cmd	*next;
	struct s_cmd	*prev;
	struct s_envp	*env;
	t_token_type	type;
}					t_cmd;

typedef struct t_ast
{
	t_cmd			*cmd;
	struct t_ast	*left;
	struct t_ast	*right;
	struct t_ast	*parent;
}					t_ast;

typedef struct s_redir
{
	char			*redir;
	int				type;
	struct s_redir	*next;
}					t_redir;

typedef struct s_envp
{
	char			*var_name;
	char			*var_value;
	int				print_flag;
	struct s_envp	*next;
	struct s_envp	*prev;
}					t_envp;

typedef struct s_exec
{
	int				std_in;
	int				std_out;
	int				prev;
	int				return_value;
	int				pipe[2];
}					t_exec;

									
typedef struct s_save_struct
{
	char			*save_spaces;
	int				redir_flag;
	struct t_ast	*save_root;
	struct s_cmd	*cmd;
	struct t_ast	*ast;
	struct s_envp	*envp;
	struct s_exec	*exec;
	char **save_envp;
}					t_save_struct;

typedef struct s_data_parsing
{
	char			*buffer;
	char			*cmd;
	char			*exp;
	int				bufflen;
	int				cmd_index;
	int				i;
	int				k;
	t_save_struct	*t_struct;
}					t_data_parsing;

typedef struct s_vars
{
    t_envp **env;
    char **res;
    int *j;
    int *i;
    int len;
} t_vars;

#endif