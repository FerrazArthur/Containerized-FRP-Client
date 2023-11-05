ARG ALPINE_VERSION=3.18.4

ARG FRP_VERSION=0.52.3

# First step: install frp
FROM alpine:"$ALPINE_VERSION" AS installer

ARG FRP_VERSION
ARG OS=linux
ARG ARCH=amd64

RUN apk add --no-cache wget tar alpine-sdk 

RUN wget https://github.com/fatedier/frp/releases/download/v"$FRP_VERSION"/frp_"$FRP_VERSION"_"$OS"_"$ARCH".tar.gz && tar xvf frp_"$FRP_VERSION"_"$OS"_"$ARCH".tar.gz && mv frp_"$FRP_VERSION"_"$OS"_"$ARCH" frp && cd frp && rm frps frps.toml LICENSE

COPY src/c_version/main.c .
COPY input_provider/input_provider.sh /frp/

RUN gcc -o frp/quant1-frpc main.c

# Final image
FROM alpine:"$ALPINE_VERSION"

RUN apk add --no-cache openldap-clients && addgroup -S -g 10001 quant1_group && adduser -SH -u 10001 -G quant1_group quant1_frp_client

COPY --from=installer /frp/ /frp/

WORKDIR /frp/

RUN chown -R quant1_frp_client:quant1_group . && chmod 555 frpc quant1-frpc input_provider.sh && mv frpc /bin 

USER quant1_frp_client

ENTRYPOINT [ "sh" ]

CMD [ "-c", "./input_provider.sh | ./quant1-frpc non-interactive" ]
