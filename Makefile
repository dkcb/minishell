# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tbouma <tbouma@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/07/13 16:26:13 by tiemen            #+#    #+#              #
#    Updated: 2022/09/14 10:42:52 by tbouma           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := minishell
OBJFILES := \
			obj/src/b_cd.o \
			obj/src/b_echo.o \
			obj/src/b_env.o  \
			obj/src/b_exit.o \
			obj/src/b_export.o \
			obj/src/b_pwd.o \
			obj/src/b_unset.o  \
			obj/src/builtins.o \
			obj/src/env_var_list.o \
			obj/src/executils.o \
			obj/src/execution.o \
			obj/src/expand_variables_heredoc.o \
			obj/src/expand_variables.o \
			obj/src/expand_variables2.o \
			obj/src/free.o \
			obj/src/get_next_line_utils.o \
			obj/src/get_next_line.o \
			obj/src/heredoc.o \
			obj/src/linked_list.o \
			obj/src/linked_list2.o \
			obj/src/main.o \
			obj/src/parsing_make_cmd_struct.o \
			obj/src/parsing_cmd_lines.o \
			obj/src/parsing_make_exec_line.o \
			obj/src/parsing_redir_heredoc.o \
			obj/src/parsing_redir_open_fd.o \
			obj/src/parsing_redir_utils.o \
			obj/src/parsing_redir.o \
			obj/src/parsing_tokens.o \
			obj/src/parsing_tokens2.o \
			obj/src/path_command.o \
			obj/src/signals.o \
			obj/src/testing.o \
			obj/src/utils.o \
			obj/src/utils2.o
			
LIBFT := includes/libft/libft.a
HEADERS := -I libft/
CFLAGS := -Wall -Wextra  -fsanitize=address -g3 #-Werror
FLAGS_DEBUG := -fsanitize=address -g3
CC := gcc

all: $(LIBFT) $(NAME)

$(LIBFT): 
	make -C ./includes/libft

$(NAME): $(OBJFILES)
	cp $(LIBFT) $(NAME)
	$(CC) $(CFLAGS) $(OBJFILES) $(LIBFT) $(HEADERS) -o $(NAME) -lreadline -L ~/.brew/opt/readline/lib
#  -L /usr/lib/x86_64-linux-gnu/

obj/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) -o $@ $< -I ~/.brew/opt/readline/include
#-I /usr/include/readline/



clean:
	rm -f $(OBJFILES)
	make clean -C ./includes/libft

fclean:	clean
	rm -f $(NAME)
	make fclean -C ./includes/libft

re: fclean all

run: $(NAME)
	./$(NAME)

.PHONY: clean all fclean re