ARG WAGI_VERSION=0.8.1
ARG UBUNTU_VERSION=jammy-20231004

FROM ubuntu:${UBUNTU_VERSION} AS downloader
ARG WAGI_VERSION
ARG OS=linux
ARG ARCH=amd64

# Update and install necessary tools
RUN apt-get update \
    && apt-get install -y curl wget \
    && apt-get clean \
    && apt-get auto-remove -y \
    && rm -rf /var/cache/apt/* \
    && rm -rf /var/lib/apt/lists/* \
    && rm -rf /tmp/*

COPY application/src /app
COPY application/entrypoint.sh /app

RUN wget https://github.com/deislabs/wagi/releases/download/v"$WAGI_VERSION"/wagi-v"$WAGI_VERSION"-"$OS"-"$ARCH".tar.gz \
    && tar -xvf wagi-v"$WAGI_VERSION"-"$OS"-"$ARCH".tar.gz \
    && mv wagi /app

FROM ubuntu:${UBUNTU_VERSION} AS base

COPY --from=downloader /app /app

WORKDIR /app

RUN groupadd -r -g 10001 wagi_g \
    && useradd --no-log-init --system --uid 10001 --gid wagi_g --no-create-home wagi \
    && chown -R wagi:wagi_g . \
    && chmod 555 wagi entrypoint.sh \
    && mv wagi /usr/local/bin

RUN printf "deb http://security.ubuntu.com/ubuntu focal-security main" | cat > /etc/apt/sources.list 

# Add dependencies for wagi (they are downgraded)
RUN apt-get update \
    && apt-get install -y --allow-downgrades openssl=1.1.1f-1ubuntu2.21 \
    && ldconfig \
    && apt-get clean \
    && apt-get auto-remove -y \
    && rm -rf /var/cache/apt/* \
    && rm -rf /var/lib/apt/lists/* \
    && rm -rf /tmp/*


USER wagi

ENTRYPOINT [ "bash" ]

CMD [ "-c", "./entrypoint.sh" ]
