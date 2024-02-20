ARG ALPINE_VERSION=3.18.4

ARG FRP_VERSION=0.52.3

# First step: install frp
FROM alpine:"$ALPINE_VERSION" AS installer

ARG FRP_VERSION
ARG OS=linux
ARG ARCH=amd64

RUN apk add --no-cache wget tar alpine-sdk make openldap-dev libressl-dev

RUN wget https://github.com/fatedier/frp/releases/download/v"$FRP_VERSION"/frp_"$FRP_VERSION"_"$OS"_"$ARCH".tar.gz \
&& tar xvf frp_"$FRP_VERSION"_"$OS"_"$ARCH".tar.gz \
&& mv frp_"$FRP_VERSION"_"$OS"_"$ARCH" frp \
&& cd frp \
&& rm frps frps.toml LICENSE

# Copies build related files
COPY src/ /src/
COPY Makefile Makefile

# Copies server configuration file
COPY ./server_config /frp/server_config

RUN make && mv my-frpc /frp/

# Final image
FROM alpine:"$ALPINE_VERSION"

# Debug tools
RUN apk add --no-cache curl bind-tools

RUN apk add --no-cache libldap libressl-dev && addgroup -S -g 10001 frp_group && adduser -SH -u 10001 -G frp_group frp_client

COPY --from=installer /frp/ /frp/

WORKDIR /frp/

RUN chown -R frp_client:frp_group . && chmod 555 frpc my-frpc && mv frpc /bin 

USER frp_client

ENTRYPOINT [ "sh" ]

CMD [ "-c", "./my-frpc" ]
