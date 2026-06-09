FROM ubuntu:latest
ENV DEBIAN_FRONTEND=noninteractive

RUN apt update && apt install -y g++

WORKDIR /bank
COPY oops.cpp .
RUN g++ oops.cpp -o oops

CMD ["./oops"]
