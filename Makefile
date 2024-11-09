
NAME		=	mod1
CXX			=	c++
FLAGS		=	-Wall -Wextra -Werror -std=c++98 -g3

SRCS		=	./src/main.cpp		\
				./src/WaterSurface.class.cpp

HEADERS		=	./include

OBJS		=	$(SRCS:.cpp=.o)

all:	$(NAME)

$(NAME): $(OBJS)
	@$(CXX) $(FLAGS) $(OBJS) -o $(NAME)
	@echo "Compiling $(NAME)"

%.o: %.cpp
	@$(CXX) $(FLAGS) -I $(HEADERS) -c  $< -o $@
	@echo "Compiling $<"

clean:
	@rm -rf $(OBJS)
	@echo "Cleaning..."

fclean: clean
	@rm -rf $(NAME)

re: fclean all

.PHONY : all clean fclean re
