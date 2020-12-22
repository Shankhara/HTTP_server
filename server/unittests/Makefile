VPATH		= ../:../fds/
VPATH		+= ../:../parsing/

NAME		= unittests
OBJS		= unittests.o Client.o Request.o Logger.o Server.o CGIExec.o CGIResponse.o Utils.o testRequest.o testCGI.o Listener.o Parsing.o Ut.o testParsing.o

CXX			= clang++
CXXFLAGS	= -Wall -Wextra -Werror -g3
RM			= rm -f

$(NAME):	$(OBJS)
			$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

unittests.o : unittests.hpp
testRequest.o: testRequest.cpp
testParsing.o: testParsing.cpp
testCGI.o: testCGI.cpp
Client.o : ../fds/Client.hpp
Utils.o : ../Utils.hpp
Request.o : ../Request.hpp 
Logger.o : ../Logger.hpp
Server.o : ../Server.hpp
Listener.o : ../fds/Listener.hpp
#Response.o : ../Response.hpp
CGIExec.o : ../CGIExec.hpp
CGIResponse.o : ../fds/CGIResponse.hpp
Parsing.o : ../parsing/Parsing.hpp
Ut.o: ../parsing/Ut.hpp

cgi:
 			@wget -nc https://projects.intra.42.fr/uploads/document/document/2465/ubuntu_cgi_tester -O /tmp/ubuntu_cgi_tester

clean:
			$(RM) $(OBJS)

fclean: 	clean
			$(RM) $(NAME)

re:			cgi fclean $(NAME)

tests: re
			./$(NAME)
			@$(RM) $(OBJS)

.PHONY: all clean fclean re
