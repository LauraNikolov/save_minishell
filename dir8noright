
# Colors
RED    := \033[0;31m
GREEN  := \033[0;32m
YELLOW := \033[0;33m
BLUE   := \033[0;34m
RESET  := \033[0m

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g3
LDFLAGS = -lreadline

# Directories
PARSEDIR = parse
EXECDIR = exec
LIBFTDIR = libft
BUILTINSDIR = builtins

# Source files
PARSE_SRCS = $(wildcard $(PARSEDIR)/*.c)
BUILTINS_SRCS = $(wildcard $(BUILTINSDIR)/*.c)
EXEC_SRCS = $(wildcard $(EXECDIR)/*.c)
MAIN_SRCS = main.c

# Object files
PARSE_OBJS = $(PARSE_SRCS:.c=.o)
BUILTINS_OBJS = $(BUILTINS_SRCS:.c=.o)
EXEC_OBJS = $(EXEC_SRCS:.c=.o)
MAIN_OBJS = $(MAIN_SRCS:.c=.o)

# Static library
LIBFT = $(LIBFTDIR)/libft.a

# Executable name
NAME = minishell

# ASCII Art
ascii_art:
	@echo -e "$(YELLOW)"
	@echo	"███╗   ███╗██╗███╗   ██╗██╗███████╗██╗  ██╗███████╗██╗     ██╗"  
	@echo	"████╗ ████║██║████╗  ██║██║██╔════╝██║  ██║██╔════╝██║     ██║"   
	@echo 	"██╔████╔██║██║██╔██╗ ██║██║███████╗███████║█████╗  ██║     ██║" 
	@echo	"██║╚██╔╝██║██║██║╚██╗██║██║╚════██║██╔══██║██╔══╝  ██║     ██║"     
	@echo	"██║ ╚═╝ ██║██║██║ ╚████║██║███████║██║  ██║███████╗███████╗███████╗"
	@echo	"╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝"                                                             
	@echo -e "$(RESET)"

# Targets
all: ascii_art $(NAME)

$(LIBFT):
	@echo -e "$(YELLOW)Compiling libft...$(RESET)"
	@make -C $(LIBFTDIR)
	@echo -e "$(GREEN)Libft compiled successfully$(RESET)"

$(NAME): $(LIBFT) $(PARSE_OBJS) $(BUILTINS_OBJS) $(EXEC_OBJS) $(MAIN_OBJS)
	@echo -e "$(YELLOW)Linking $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) -o $@ $^ -L$(LIBFTDIR) -lft $(LDFLAGS)
	@echo -e "$(GREEN)Compilation successful: $@$(RESET)"

# Pattern rule for object files
%.o: %.c
	@echo -e "$(BLUE)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	@echo -e "$(RED)Cleaning object files...$(RESET)"
	@rm -f $(PARSE_OBJS) $(BUILTINS_OBJS) $(EXEC_OBJS) $(MAIN_OBJS)

fclean: clean
	@echo -e "$(RED)Removing binary: $(NAME)...$(RESET)"
	@rm -f $(NAME)
	@make -C $(LIBFTDIR) fclean

re: fclean all

.PHONY: all clean fclean re ascii_art
