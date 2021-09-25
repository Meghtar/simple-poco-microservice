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
RUN mkdir -p /application/logs
WORKDIR /application
COPY --from=builder /build/bin/server /application/
COPY /lib/* /usr/local/lib/
COPY /config.ini /application/config.ini
ENV LD_LIBRARY_PATH=/usr/local/lib/

#EXPOSE 19999
ENTRYPOINT [ "/application/server" ]
# ENTRYPOINT [ "/bin/bash" ]
#prepare config.ini before
