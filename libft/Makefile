NAME = libft.a

CC = gcc
CFLAGS = -Wall -Wextra -Werror
AR = ar rc
RM = rm -f

FILES = ft_memset \
		ft_bzero \
		ft_memcpy \
		ft_memmove \
		ft_memchr \
		ft_memcmp \
		ft_strlen \
		ft_strlcpy \
		ft_strlcat \
		ft_strcat \
		ft_strchr \
		ft_strrchr \
		ft_is_char \
		ft_strnstr \
		ft_strncmp \
		ft_strndup \
		ft_strcmp \
		ft_atoi \
		ft_atoll \
		ft_isalpha \
		ft_str_isdigit \
		ft_isdigit \
		ft_isalnum \
		ft_isascii \
		ft_isprint \
		ft_toupper \
		ft_tolower \
		ft_count_tab \
		ft_strdup \
		ft_calloc \
		ft_substr \
		ft_strjoin \
		ft_jointab \
		ft_strjoin_path \
		ft_strdup_array \
		ft_strtrim \
		ft_split \
		ft_itoa \
		ft_strmapi \
		ft_striteri \
		ft_putchar_fd \
		ft_putstr_fd \
		ft_putendl_fd \
		ft_putnbr_fd \
		get_next_line \
		get_next_line_utils \

FILES_BONUS = ft_lstnew \
			ft_lstadd_front \
			ft_lstsize \
			ft_lstlast \
			ft_lstadd_back \
			ft_lstdelone \
			ft_lstclear \
			ft_lstiter \
			ft_lstmap \

SRCS_DIR = ./
SRCS = $(addprefix $(SRCS_DIR), $(addsuffix .c, $(FILES)))
SRCS_BONUS = $(addprefix $(SRCS_DIR), $(addsuffix .c, $(FILES_BONUS)))

OBJS_DIR = ./
OBJS = $(addprefix $(OBJS_DIR), $(addsuffix .o, $(FILES)))
OBJS_BONUS = $(addprefix $(OBJS_DIR), $(addsuffix .o, $(FILES_BONUS)))

all: $(NAME)
	@echo "libft compiled"

.o: .c 
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	@$(AR) $(NAME) $(OBJS) 

bonus: $(OBJS_BONUS)
	@$(AR) $(NAME) $(OBJS_BONUS)
	@echo "libft bonus compiled"
	
clean:
	@$(RM) $(OBJS) $(OBJS_BONUS)
	@echo "cleaned"

fclean: clean
	@$(RM) $(NAME)
	@echo "fully cleaned"

re: fclean all
	@echo "recompiled"
