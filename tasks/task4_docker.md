# Task 4: Assignment 4.1 (Dockerize OOPS Assignment)

Hey Y25s! Now that you have built the SmartBank Enterprise System, it's time to learn how to package and distribute your applications. Your next task is to learn **Docker** and containerize your OOPS assignment.

Docker allows developers to package applications with all of their dependencies into a single, standardized unit called a container. This ensures that your code runs exactly the same way on any machine—no more "it works on my machine!" excuses.

### Requirements

*   **Dockerize** your SmartBank Enterprise System.
*   Create a `Dockerfile` in the root of your project directory.
*   The `Dockerfile` must:
    1. Use an appropriate base image (e.g., `ubuntu`, `gcc`, or `alpine`).
    2. Copy your C++ source code into the container.
    3. Compile your C++ code during the image build process.
    4. Define the command to run your compiled executable when the container starts.
*   Ensure that running `docker build -t smartbank .` and `docker run -it smartbank` successfully launches your banking system.

### Frequently Asked Questions (FAQs)

**Q: I've never used Docker before. Where do I start?**
A: Start by reading the official [Docker documentation](https://docs.docker.com/get-started/) or checking out introductory tutorials on YouTube that were sent on Discord channel. Learn about what a `Dockerfile` is, and the basic commands: `docker build`, `docker images`, `docker run`, and `docker ps`.

**Q: Do I need to change my C++ code?**
A: No! You should not have to change the logic of your SmartBank application. You are simply writing a configuration file (`Dockerfile`) that tells Docker how to compile and run your existing code in an isolated environment.

**Q: How do I handle multiple `.cpp` and `.h` files in the Dockerfile?**
A: You can copy your project files into the container (using `COPY`), and then run your usual compilation command (like `g++ *.cpp -o bank`) using the `RUN` directive.

**Q: How do I interact with the program if it takes user input?**
A: When you run the container, make sure to use the `-it` flags (e.g., `docker run -it smartbank`). This runs the container interactively, allowing you to provide inputs to the terminal.

### Submission

Please push your `Dockerfile` (and any other necessary build scripts) to your branch and open your **Pull Requests (PRs)** by the **25th May EOD**.
