FROM gcc:14-bookworm AS builder

WORKDIR /app

COPY Makefile ./
COPY include ./include
COPY src ./src

RUN make clean && make LDFLAGS="-static-libstdc++ -static-libgcc"

FROM debian:bookworm-slim AS runtime

ENV LANG=C.UTF-8
WORKDIR /app

COPY --from=builder /app/smartbank ./smartbank

CMD ["./smartbank"]
