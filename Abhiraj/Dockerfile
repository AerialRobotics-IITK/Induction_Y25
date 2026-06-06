FROM gcc:latest
WORKDIR /usr/src/smartbank
COPY . .
RUN g++ -std=c++17 BankingSystem.cpp -o smartbank
CMD ["./smartbank"]
