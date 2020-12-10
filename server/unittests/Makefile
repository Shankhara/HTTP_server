NAME		= unittests
OBJS		= unittests.o Client.o Request.o Logger.o Server.o

CXX			= clang++
CXXFLAGS	= -Wall -Wextra -Werror
RM			= rm -f

$(NAME):	$(OBJS)
			$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

unittests.o : unittests.cpp unittests.hpp
			$(CXX)  -c unittests.cpp
Client.o : ../fds/Client.cpp ../fds/Client.hpp
			$(CXX) -c ../fds/Client.cpp
Request.o : ../Request.cpp ../Request.hpp 
			$(CXX) -c ../Request.cpp
Logger.o : ../Logger.cpp ../Logger.hpp
			$(CXX) -c ../Logger.cpp
Server.o : ../Server.cpp ../Server.hpp
			$(CXX) -c ../Server.cpp
clean:
			$(RM) $(OBJS) $(RM) *.gch ../*.gch ../fds/*.gch

fclean: 	clean
			$(RM) $(NAME)

re:			fclean $(NAME)

.PHONY: all clean fclean re
