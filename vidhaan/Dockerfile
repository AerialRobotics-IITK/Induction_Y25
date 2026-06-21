FROM ubuntu:latest

RUN apt-get update && apt-get install -y g++

WORKDIR /app

COPY Banking.cpp .

RUN g++ -o smartbank Banking.cpp

CMD ["./smartbank"]