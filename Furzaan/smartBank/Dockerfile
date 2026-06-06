FROM alpine:latest
RUN apk add --no-cache g++

WORKDIR /app
COPY Assignment3.cpp .

RUN g++ -std=c++17 Assignment3.cpp -o smartbank

CMD ["./smartbank"]
