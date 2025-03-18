run: build
	docker run -d --rm --name ft_ping -v $(shell pwd)/../../42adv_ft_ping/:/app ft_ping

build:
	docker build -t ft_ping .

exec:
	docker exec -it ft_ping bash

stop:
	docker stop ft_ping

fclean: stop
	docker image rm ft_ping

re: run build exec stop fclean
