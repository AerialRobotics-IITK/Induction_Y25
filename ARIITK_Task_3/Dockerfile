# 1. Use a Linux image that already has C++ development tools installed
FROM alpine:3.19
RUN apk add --no-cache g++ build-base

WORKDIR /app

# 2. Copy your actual source code into the container
COPY main.cpp .

# 3. Compile it right here inside the Linux container
RUN g++ main.cpp -o smart_bank

# 4. Run the freshly compiled Linux binary
CMD ["./smart_bank"]
