NAME = ircserv

SRCS =  ./authentication/nickname.cpp \
		./authentication/parse_auth.cpp \
		./authentication/password.cpp \
		./authentication/username.cpp \
		./channels/channel.cpp \
		./clients/client.cpp \
		./bot/TimeBot.cpp \
		./bot/DiceBot.cpp \
		./commands/Invite.cpp \
		./commands/Join.cpp \
		./commands/Kick.cpp \
		./commands/Mode.cpp \
		./commands/Nick.cpp \
		./commands/Notice.cpp \
		./commands/ParseCommands.cpp \
		./commands/Part.cpp \
		./commands/Pong.cpp \
		./commands/PrivMsg.cpp \
		./commands/Quit.cpp \
		./commands/Topic.cpp \
		./main.cpp \
		./server/create_server.cpp \
		./server/is_connection.cpp \
		./server/server.cpp \
		./utils/utils.cpp

INC = 	./headers/Channel.hpp \
		./headers/Client.hpp \
		./headers/Commands.hpp \
		./headers/Logger.hpp \
		./headers/Replies.hpp \
		./headers/Server.hpp \
		./headers/Utils.hpp

OBJS = $(SRCS:.cpp=.o)

RM = rm -f

CPP = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98

all: $(NAME)

$(NAME): $(OBJS)
	$(CPP) $(OBJS) $(CPPFLAGS) -o $(NAME)

%.o:%.cpp $(INC)
	$(CPP) $(CPPFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all