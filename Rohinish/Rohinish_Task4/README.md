# Rohinish Task4 - SmartBank Docker Setup

This folder contains a Dockerized copy of `Rohinish_Task3`, a C++17 SmartBank demo application.

## Requirements

- Docker
- Docker Compose plugin

## Build and run with Docker

```sh
docker build -t rohinish-task4-smartbank .
docker run --rm rohinish-task4-smartbank
```

## Build and run with Docker Compose

```sh
docker compose up --build
```

## Build locally without Docker

```sh
make
./smartbank
```

To clean local build artifacts:

```sh
make clean
```
