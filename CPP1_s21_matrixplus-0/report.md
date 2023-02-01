# Simple Docker by @rdexter

## Contents

1. [Готовый докер](#part-1-готовый-докер)
2. [Операции с контейнером](#part-2-операции-с-контейнером)
3. [Dockle](#part-5-dockle)

## Part 1. Готовый докер

- Скачаем nginx из dockerhub `docker pull nginx`
![pull_image](images/01_01.png)
- Проверим наличие докер образа через `docker images`
![images](images/01_02.png)
- Запустим докер образ через `docker run -d [image_id|repository]`
![run](images/01_03.png)
- Проверим, что образ запустился через `docker ps`
![ps_run](images/01_04.png)
- Посмотрим информацию о контейнере: размер контейнера
`docker inspect [container_id|container_name] --size | grep "\"SizeRootFs\""` 
![inspect_size_container](images/01_05.png)
- Посмотрим информацию о контейнере: ip address
`docker inspect [container_id|container_name] | grep "\"IPAddress\"" -m1` 
![inspect_ip_address](images/01_06.png)
- Посмотрим информацию о контейнере: замапленные порты
`docker inspect [container_id|container_name] | grep "\"Ports\"" -A2   ` 
![inspect_ports](images/01_07.png)
- Остановим докер образ через `docker stop [container_id|container_name]`
![stop](images/01_08.png)
- Проверим, что образ остановился через `docker ps`
![ps_stop](images/01_09.png)
- Запустим докер с замапленными портами 80 и 443 на локальную машину через команду run:
`docker run -d -p 80:80 -p 443:80 nginx`
![run_mapping](images/01_10.png)
- Проверим, что образ остановился через `docker ps`
![ps_mapping](images/01_11.png)
- Проверим, что в браузере по адресу localhost:80 доступна стартовая страница nginx
![80_port](images/01_12.png)
- Проверим, что в браузере по адресу localhost:443 доступна стартовая страница nginx
![443_port](images/01_13.png)
- Перезапустим докер контейнер через `docker restart [container_id|container_name]`
![restart](images/01_14.png)
- Проверим любым способом, что контейнер запустился через `docker ps -a`
![ps_restart](images/01_15.png)

## Part 2. Операции с контейнером

- Прочитаем конфигурационный файл nginx.conf внутри докер образа через команду 
`docker exec silly_bose cat /etc/nginx/nginx.conf`
![cat_nginx_conf](images/02_01.png)
- Создадим на локальной машине файл nginx.conf и настроим в нем по пути /status 
отдачу страницы статуса сервера nginx
![touch_nginx.conf](images/02_02.png)
- Скопируем созданный файл nginx.conf внутрь докер образа через команду 
`docker cp nginx.conf silly_bose:/etc/nginx`
![copy_nginx](images/02_03.png)
- Перезапустим nginx внутри докер образа через команду `docker exec silly_bose nginx -s reload`
![reload_nginx](images/02_04.png)
- Проверим, что по адресу localhost:80/status отдается страничка со статусом сервера nginx 
![check_status](images/02_05.png)
- Экспортируем контейнер в файл container.tar через команду `docker export silly_bose > container.tar`
![export_container](images/02_06.png)
- Остановим контейнер `docker stop silly_bose`
![stop-container](images/02_07.png)
- Удалим образ через `docker rmi -f nginx`, не удаляя перед этим контейнеры
![delete_image](images/02_08.png)
- Удалим остановленный контейнер `docker rm silly_bose`
![delete_stop_container](images/02_09.png)
- Импортируем контейнер обратно через команду `docker import container.tar`
![import_container](images/02_10.png)
- Запустим импортированный контейнер, обязательно пишем `nginx -g 'daemon off;'`, 
либо entrypoint.sh, так как при экспорте копируется не вся информация
![run_import_container](images/02_11.png)
- Проверим, что по адресу localhost:80/status отдается страничка со статусом сервера nginx
![check_status](images/02_12.png)

## Part 5. Dockle
- Установим Dockle через brew. Видим типовые ошибки с удалением временных файлов после установки
утилит и использованием root вместо создания нового пользователя
![before_fix](images/05_01_before_fix.png)
- Создали новый файл Dockerfile_part5 и в нём устранили ошибки
![after_fix](images/05_02_after_fix.png)