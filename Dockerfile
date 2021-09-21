FROM poco_builder AS builder

RUN mkdir -p /build/src
COPY ./src /build/src
COPY ./Makefile /build/

WORKDIR /build
RUN mkdir bin
RUN ls ./src
RUN make
#g++ -std=c++2a src/main.cpp -o bin/server -lPocoFoundation -lPocoUtil -lPocoNet

# next stage

FROM ubuntu:20.04
WORKDIR /tmp
COPY --from=builder /build/bin/server /tmp/
COPY /lib/* /usr/local/lib/
ENV LD_LIBRARY_PATH=/usr/local/lib/

EXPOSE 19999
ENTRYPOINT [ "/tmp/server" ]
