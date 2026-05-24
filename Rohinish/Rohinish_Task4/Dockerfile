FROM gcc:14 AS builder

WORKDIR /app

COPY Makefile ./
COPY include ./include
COPY src ./src

RUN make clean && make

FROM debian:bookworm-slim AS runtime

ENV LANG=C.UTF-8
WORKDIR /app

COPY --from=builder /app/smartbank ./smartbank

CMD ["./smartbank"]
