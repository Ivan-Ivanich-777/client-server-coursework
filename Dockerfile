FROM ubuntu:22.04
RUN apt-get update && apt-get install -y qtbase5-dev qt5-qmake build-essential libssl-dev && rm -rf /var/lib/apt/lists/*
WORKDIR /app
COPY Server/ /app/
RUN rm -rf /app/Server
RUN qmake Server.pro && make
RUN ls -la
EXPOSE 33333
CMD ["./Server"]
