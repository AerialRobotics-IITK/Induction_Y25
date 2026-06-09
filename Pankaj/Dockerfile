# Use an official GCC image as the base image
FROM gcc:latest

# Set the working directory in the container
WORKDIR /usr/src/smartbank

# Copy the C++ source code into the container
COPY SmartBank.cpp .

# Compile the C++ code during the image build process
RUN g++ -o smartbank SmartBank.cpp

# Define the command to run the compiled executable when the container starts
CMD ["./smartbank"]
