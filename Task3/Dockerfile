# Base image containing g++
FROM gcc:latest

# Create working directory
WORKDIR /app

# Copy all project files into container
COPY . .

# Compile the project
RUN g++ *.cpp -o smartbank

# Run executable when container starts
CMD ["./smartbank"]
