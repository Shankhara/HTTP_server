NAME	= webserver
SRCS	= main.cpp
SRCS	+= Server.cpp
SRCS	+= Logger.cpp
SRCS	+= fds/Listener.cpp
#SRCS	+= Request.cpp
#SRCS	+= fds/CGIExec.cpp
SRCS	+= fds/Client.cpp
CFLAGS	= -Wall -Wextra -Werror -g3 -std=c++98
CC		= clang++

OBJDIR	= .objs
OBJS	=	$(addprefix $(OBJDIR)/, $(SRCS:.cpp=.o))

$(NAME): $(OBJS)
		${CC} ${CFLAGS} $(OBJS) -o $(NAME)

all:		$(NAME)

$(OBJS): | $(OBJDIR)

$(OBJDIR):
		@mkdir $(OBJDIR)
		@mkdir -p $(OBJDIR)/fds

${OBJDIR}/%.o: %.cpp
		${CC} ${CFLAGS} -c $< -o $@

clean:
			rm -rf $(OBJDIR)

fclean:	clean
			rm -f $(NAME)

re:		fclean all

run: all
	@./webserver
	@rm -f ./webserver

debug: all
	@valgrind --track-fds=yes --leak-check=full ./webserver -v
	@rm -f ./webserver

tests:
	@make -C ./unittests/
	@./unittests/unittests


.PHONY: all clean fclean re run tests
