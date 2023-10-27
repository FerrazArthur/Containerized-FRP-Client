# Q1-FRPC

Project aims to provide containers image that allows quant1 devs to enable personalized reverse proxy locally and share their applications globally, via quant1's FRP server.

## Usage

The container use described below is intended to be configured once and then it can be stopped / started on demand.

As the configuration envolves input from user, it needs a TTY. However, after configuring you can stop it and restart, it should work fine as detached.

### Creating the container

```bash
docker container run --name Q1-frpc -it --network host registry.quant1.com.br/arthur/q1-frpc/main
```

At the end, it'll output a url you can use globally.

### Stop/starting it

To stop the execution

```bash
docker container stop Q1-frpc
```

To restart the execution

```bash
docker container start Q1-frpc
```
