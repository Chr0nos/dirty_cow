#* ************************************************************************** *#
#*                                                                            *#
#*                                                        :::      ::::::::   *#
#*   Makefile                                           :+:      :+:    :+:   *#
#*                                                    +:+ +:+         +:+     *#
#*   By: snicolet <marvin@student.42.fr>            +#+  +:+       +#+        *#
#*                                                +#+#+#+#+#+   +#+           *#
#*   Created: 2018/05/14 23:10:15 by snicolet          #+#    #+#             *#
#*   Updated: 2018/05/14 23:12:08 by snicolet         ###   ########.fr       *#
#*                                                                            *#
#* ************************************************************************** *#

NAME=dirtycow
LIBFT=../libft/
INC=-I$(LIBFT)/include
LINKER=-L$(LIBFT) -lft -lpthread

all: $(NAME)

$(NAME):
	$(CC) main.c $(INC) $(LINKER) -o $(NAME)

fclean:
	$(RM) $(NAME)

re: fclean all
