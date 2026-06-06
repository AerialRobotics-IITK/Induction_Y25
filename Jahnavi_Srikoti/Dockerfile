FROM gcc:latest
WORKDIR /usr/src/smartbank
COPY . .
RUN g++ *.cpp -o smartbank
CMD ["./smartbank"]
