FROM gcc:latest
WORKDIR /app
COPY SmartBank.cpp .
RUN g++ -std=c++11 SmartBank.cpp -o smartbank
CMD ["./smartbank"]
