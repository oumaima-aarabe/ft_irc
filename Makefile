NAME = ircserv
SRCS = client.cpp cmnds.cpp main.cpp nickname.cpp \
	password.cpp server.cpp username.cpp parse_auth.cpp \

SRCS = $(shell find . -name '*.cpp')

INC = $(shell find . -name '*.hpp')

OBJS = $(SRCS:.cpp=.o)

RM = rm -f

CPP = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -g -fsanitize=address

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