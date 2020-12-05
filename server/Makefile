NAME	= server
SRCS	= main.cpp
SRCS	+= Server.cpp
SRCS	+= Client.cpp
SRCS	+= Logger.cpp
#SRCS	+= Request.cpp
CFLAGS	= -Wall -Wextra -Werror -g3
CC		= clang++

OBJDIR	= .objs
OBJS	=	$(addprefix $(OBJDIR)/, $(SRCS:.cpp=.o))

$(NAME): $(OBJS)
		${CC} ${CFLAGS} $(OBJS) -o $(NAME)

all:		$(NAME)

$(OBJS): | $(OBJDIR)

$(OBJDIR):
		@mkdir $(OBJDIR)

${OBJDIR}/%.o: %.cpp
		${CC} ${CFLAGS} -c $< -o $@

clean:
			rm -rf $(OBJDIR)

fclean:	clean
			rm -f $(NAME)

re:		fclean all

run: all
	@./server

tests:
	@make -C ./unittests/
	@./unittests/unittests


.PHONY: all clean fclean re run tests
