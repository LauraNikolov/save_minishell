CFLAGS = -Wall -Wextra -Werror -g3
LDFLAGS = -lreadline
CC = cc

PARSEDIR = parse
EXECDIR = exec
LIBFTDIR = libft
BUILTINSDIR = builtins

PARSE_SRCS = $(wildcard $(PARSEDIR)/*.c)
BUILTINS_SRCS = $(wildcard $(BUILTINSDIR)/*.c)
EXEC_SRCS = $(wildcard $(EXECDIR)/*.c)
MAIN_SRCS = main.c

PARSE_OBJS = $(PARSE_SRCS:.c=.o)
BUILTINS_OBJS = $(BUILTINS_SRCS:.c=.o)
EXEC_OBJS = $(EXEC_SRCS:.c=.o)
MAIN_OBJS = $(MAIN_SRCS:.c=.o)

LIBFT = $(LIBFTDIR)/libft.a
NAME = minishell

$(LIBFT):
	@echo "Compiling libft..."
	@make -C $(LIBFTDIR)
	@echo "Libft compiled successfully"

$(NAME): $(LIBFT) $(PARSE_OBJS) $(BUILTINS_OBJS) $(EXEC_OBJS) $(MAIN_OBJS)
	@echo "Linking $(NAME)..."
	@$(CC) $(CFLAGS) -o $@ $(PARSE_OBJS) $(BUILTINS_OBJS) $(EXEC_OBJS) $(MAIN_OBJS) -L$(LIBFTDIR) -lft $(LDFLAGS)
	@echo "Compilation successful: $@"

all: $(NAME)

clean:
	@echo "Cleaning object files..."
	@rm -f $(PARSE_OBJS) $(BUILTINS_OBJS) $(EXEC_OBJS) $(MAIN_OBJS)
	@make -C $(LIBFTDIR) clean

fclean: clean
	@echo "Removing binary and object files..."
	@rm -f $(NAME)
	@make -C $(LIBFTDIR) fclean

re: fclean all

.PHONY: all clean fclean re
