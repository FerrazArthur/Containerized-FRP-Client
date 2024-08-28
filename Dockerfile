ARG ALPINE_VERSION=3.18.4

ARG FRP_VERSION=0.60.0

# First step: install frp
FROM alpine:"$ALPINE_VERSION" AS installer

ARG FRP_VERSION
ARG OS=linux
ARG ARCH=amd64

RUN apk add --no-cache wget tar

RUN wget https://github.com/fatedier/frp/releases/download/v"$FRP_VERSION"/frp_"$FRP_VERSION"_"$OS"_"$ARCH".tar.gz \
&& tar xvf frp_"$FRP_VERSION"_"$OS"_"$ARCH".tar.gz \
&& mv frp_"$FRP_VERSION"_"$OS"_"$ARCH" frp \
&& cd frp \
&& rm frps frps.toml frpc.toml LICENSE

# Copies server configuration file
COPY ./test-env-config/frpc.toml /frp/frpc.toml

# Final image
FROM alpine:"$ALPINE_VERSION"

RUN addgroup -S -g 10001 client_group && adduser -SH -u 10001 -G client_group frp_client

# Debug tools
RUN apk add --no-cache curl bind-tools

COPY --from=installer /frp/ /frp/

WORKDIR /frp/

RUN chown -R frp_client:client_group . && chmod 555 frpc && mv frpc /bin 

USER frp_client

ENTRYPOINT [ "sh" ]

CMD ["-c", "frpc -c frpc.toml"]

