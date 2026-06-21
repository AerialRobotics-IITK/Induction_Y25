
# Use the ROS Humble base image but we can also use ubuntu since program is not using any ROS specific features
FROM ros:humble 
# install nano for editing files inside the container and g++ for compiling the C++ code
RUN apt-get update && apt-get install -y nano && rm -rf /var/lib/apt/lists/_*

# Install g++ for compiling the C++ code
RUN apt-get update && apt-get install -y g++ && \rm -rf /var/lib/apt/lists/_*

# setup the working directory 
WORKDIR /app

# copy the source code into the container
COPY config/Banking_dockerise.cpp /app/config/Banking_dockerise.cpp

# compile the C++ code to create the executable named smartbank
RUN g++ -std=c++17 config/Banking_dockerise.cpp -o smartbank

# run the command to execute the smartbank executable when the container starts
CMD ["./smartbank"]

##
 linux commands:
cd Desktop/Docker/Banking_dockerise_assign
  docker image  build -t smartbank .
docker images
docker run -it smartbank
./smartbank 
  {and the output finally displays} */
