FROM debian:10.12-slim

WORKDIR /app

RUN apt-get update && apt-get upgrade -y && \
    apt-get install -y build-essential wget libreadline-dev texinfo

# Download, compile and install inetutils 2.0
RUN wget https://ftp.gnu.org/gnu/inetutils/inetutils-2.0.tar.gz && \
    tar -xzf inetutils-2.0.tar.gz && \
    cd inetutils-2.0 && \
    ./configure --disable-servers && \
    make && \
    make install && \
    cd .. && \
    rm -rf inetutils-2.0 inetutils-2.0.tar.gz

CMD ["tail", "-f", "/dev/null"]