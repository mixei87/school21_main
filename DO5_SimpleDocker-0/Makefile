N1=rdexter_3
N2=rdexter_4
N3=nginx_rdexter
D1=/etc/nginx
D2=$(D1)/server
D3=server
D4=nginx
DOC1=Dockerfile_part4
DOC2=Dockerfile_part5

.PHONY: part3 part3_create part3_copy stop_part3 kill_part3 \
reload_part3 part4_1 part4_2 part4_stop part5 part6 part6_stop

part3: part3_create part3_copy

part3_create:
	docker run --name $(N1) --rm -d -p 81:81 nginx
	docker exec $(N1) apt update
	docker exec $(N1) apt install -y gcc libfcgi-dev spawn-fcgi procps
	docker exec $(N1) mkdir -p $(D2)

	
part3_copy:
	docker cp $(D4)/nginx.conf $(N1):$(D1)
	docker cp $(D3)/server.c $(N1):$(D2)
	docker exec $(N1) gcc $(D2)/server.c -o $(D2)/server.out -lfcgi 
	docker exec $(N1) spawn-fcgi -p 8080 $(D2)/server.out
	docker exec $(N1) nginx -s reload

stop_part3:
	docker stop $(N1)

kill_part3:
	docker exec -it $(N1) bash -c 'kill -- -1'

reload_part3: kill_part3 part3_copy

part4_create_image:
	cp $(D4)/nginx_without_status.conf $(D4)/nginx.conf
	docker build -t $(N3):latest -f $(DOC1) .
	docker images

part4_run:
	docker run --rm -p80:81 -d -it \
	-v ${PWD}/nginx/nginx.conf:/etc/nginx/nginx.conf --name $(N2) $(N3)

part4_stop:
	docker stop $(N2)

part4_copy_status:
	cp $(D4)/nginx_with_status.conf $(D4)/nginx.conf

part4_1: part4_create_image part4_run

part4_2: part4_stop part4_copy_status part4_run

part5:
	docker build -t $(N3):1.0 -f $(DOC2) .
	docker images
	dockle $(N3):1.0

list_cont=$(shell docker ps -a -q)

part6: part6_stop
	@if [ -n "$(list_cont)" ]; then\
		docker stop $(list_cont);\
		docker rm $(list_cont);\
	fi
	docker-compose up -d --build

part6_stop:
	docker-compose down