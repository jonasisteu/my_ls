CC = gcc

EXEC = my_ls

SRC = main.c utils.c components.c

CFLAGS = -Wall -Werror -Wextra

$(EXEC): $(SRC)
	$(CC) $(CFLAGS) -o $(EXEC) $(SRC)

all: $(EXEC)

clean:
	rm -rf $(EXEC)

# La commande fclean ne me sert à rien parce que je ne crée pas de .o, je n'ai donc que le programme "my_ls" à exécuter.

# fclean: clean
# 	rm -rf $(SRC)

re: clean all

# .PHONY: all clean fclean re