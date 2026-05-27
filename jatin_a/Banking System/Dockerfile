FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt update && apt install -y g++

WORKDIR /app

COPY . .

RUN g++ BankingSystem.cpp -o bank

CMD ["./bank"]
