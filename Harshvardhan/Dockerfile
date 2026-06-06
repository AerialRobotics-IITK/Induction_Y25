FROM ubuntu:latest

RUN apt update && apt install -y g++

WORKDIR /app

COPY . .

RUN g++ bank.cpp -o bank

CMD ["./bank"]

