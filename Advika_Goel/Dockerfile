FROM gcc:latest
WORKDIR /usr/src/smartbank
COPY . .
RUN g++ -std=c++17 banking_system.cpp -o bank_exec
CMD ["./bank_exec"]
