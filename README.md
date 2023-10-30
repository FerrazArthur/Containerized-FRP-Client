# Q1-FRPC

Project aims to provide containers image that allows quant1 devs to enable personalized reverse proxy locally and share their applications globally, via quant1's FRP server.

## Usage

The intended use of the container described below is to configure it once, and then it can be started or stopped as needed.  

Since the configuration requires user input, a TTY is necessary.  
However, after the initial configuration, you can stop it and then restart it, and it should function properly in a detached state.

### Creating the container

```bash
docker container run --name Quant1-frpc -it --network host registry.quant1.com.br/arthur/quant1-frpc
```

At the end, it will generate a URL that can be used globally. This URL is obtained by applying an md5sum to your Q1 username and appending ".cloud.quant1.com.br" to it.

### Stop/starting it

To stop the execution

```bash
docker container stop Quant1-frpc
```

To restart the execution

```bash
docker container start Quant1-frpc
```
