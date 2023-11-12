ARG ALPINE_VERSION=3.18.4

ARG FRP_VERSION=0.52.3

# First step: install frp
FROM alpine:"$ALPINE_VERSION" AS installer

ARG FRP_VERSION
ARG OS=linux
ARG ARCH=amd64

RUN apk add --no-cache wget tar alpine-sdk make openldap-dev

RUN wget https://github.com/fatedier/frp/releases/download/v"$FRP_VERSION"/frp_"$FRP_VERSION"_"$OS"_"$ARCH".tar.gz && tar xvf frp_"$FRP_VERSION"_"$OS"_"$ARCH".tar.gz && mv frp_"$FRP_VERSION"_"$OS"_"$ARCH" frp && cd frp && rm frps frps.toml LICENSE

COPY src/ /src/
COPY Makefile Makefile

RUN make && mv quant1-frpc /frp/

# Final image
FROM alpine:"$ALPINE_VERSION"

RUN apk add libldap && addgroup -S -g 10001 quant1_group && adduser -SH -u 10001 -G quant1_group quant1_frp_client

COPY --from=installer /frp/ /frp/

WORKDIR /frp/

RUN chown -R quant1_frp_client:quant1_group . && chmod 555 frpc quant1-frpc && mv frpc /bin 

USER quant1_frp_client

ENTRYPOINT [ "sh" ]

CMD [ "-c", "./quant1-frpc -i" ]
