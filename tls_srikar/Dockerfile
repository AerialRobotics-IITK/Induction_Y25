
FROM gcc:latest

WORKDIR /app

COPY c++_fixed /app/bank.cpp

RUN g++ -std=c++17 -o bank bank.cpp -lm

ENTRYPOINT ["/app/bank"]

