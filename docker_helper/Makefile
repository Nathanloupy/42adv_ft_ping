run: build
	docker run -d --name ft_ping -v $(shell pwd)/../:/app ft_ping

build:
	docker build -t ft_ping .

exec:
	docker exec -it ft_ping bash

stop:
	docker stop ft_ping

fclean: stop
	docker rm ft_ping
	docker image rm ft_ping
 
re: run build exec stop fclean
