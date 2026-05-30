FROM ubuntu:22.04
RUN apt-get update && apt-get install -y qtbase5-dev qt5-qmake build-essential libssl-dev && rm -rf /var/lib/apt/lists/*
WORKDIR /app
COPY . /app
RUN qmake Server.pro && make
EXPOSE 33333
CMD ["./Server"]
