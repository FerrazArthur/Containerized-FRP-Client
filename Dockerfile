FROM alpine

ARG VERSION=0.52.3
ARG OS=linux
ARG ARCH=amd64

RUN apk add wget tar openldap-clients

RUN wget https://github.com/fatedier/frp/releases/download/v"$VERSION"/frp_"$VERSION"_"$OS"_"$ARCH".tar.gz && tar xvf frp_"$VERSION"_"$OS"_"$ARCH".tar.gz && cd frp_"$VERSION"_"$OS"_"$ARCH" && rm frps frps.toml LICENSE

WORKDIR frp_"$VERSION"_"$OS"_"$ARCH"

COPY *.sh /frp_"$VERSION"_"$OS"_"$ARCH"

RUN chmod 755 frpc *.sh && mv frpc /bin

ENTRYPOINT [ "sh" ]

CMD [ "./main.sh" ]
