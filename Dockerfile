FROM ubuntu:latest

RUN apt-get update && apt-get install -y g++

WORKDIR /app

COPY . .

RUN g++ *.cpp -o bank

CMD ["./bank"]
