FROM ubuntu:20.04 as builder

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update -y && \
    apt-get upgrade -y && \
    apt-get dist-upgrade -y

RUN apt-get install -y --no-install-recommends \
            build-essential \
            cmake \
            git \
            ca-certificates

ADD . /opt/sources
WORKDIR /opt/sources
RUN mkdir build && \
    cd build && \
    cmake .. && \
    make && make install

FROM ubuntu:20.04

WORKDIR /usr/bin
COPY --from=builder /tmp/bin/SFC .

ENTRYPOINT ["/usr/bin/SFC"]
