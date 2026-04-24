FROM ubuntu:22.04
ENV TZ=Europe/Moscow
RUN ln -snf /usr/share/zoneinfo/ /etc/localtime && echo  > /etc/timezone
RUN apt-get update && apt-get install -y qtbase5-dev qt5-qmake build-essential libsqlite3-dev sqlite3
WORKDIR /root/server/
COPY *.cpp /root/server/
COPY *.h /root/server/
COPY *.pro /root/server/
RUN qmake echoServer.pro && make
EXPOSE 33333
ENTRYPOINT ["./echoServer"]
