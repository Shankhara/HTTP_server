NAME	= unittests
SRCS	= unittests.cpp
SRCS	+= ../fds/Client.cpp
SRCS	+= ../Request.cpp
SRCS	+= ../Logger.cpp
#SRCS	+= ../CGIExec.cpp

CFLAGS	= -Wall -Wextra -Werror
CC		= clang++

OBJDIR	= .objs
OBJS	=	$(addprefix $(OBJDIR)/, $(SRCS:.cpp=.o))

$(NAME): $(OBJS)
		${CC} ${CFLAGS} $(OBJS) -o $(NAME)

all:		$(NAME)

$(OBJS): | $(OBJDIR)

$(OBJDIR):
		@mkdir $(OBJDIR)
		@mkdir -p fds

${OBJDIR}/%.o: %.cpp
		${CC} ${CFLAGS} -c $< -o $@

clean:
			rm -rf $(OBJDIR) *.o

fclean:	clean
			rm -f $(NAME)

re:		fclean all

run: all
	@./server


.PHONY: all clean fclean re run
