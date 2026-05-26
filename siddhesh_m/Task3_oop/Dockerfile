FROM gcc:latest
WORKDIR /usr/src/bank
COPY . .
RUN g++ bank.cpp -o bank
CMD ["./bank"]