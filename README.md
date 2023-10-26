# Q1-FRPC

Project aims to provide containers image that allows quant1 devs to enable personalized reverse proxy locally and share their applications globally, via quant1's FRP server.

## Usage

You can configure and start your frpc with the image provided with this repo and the command:

```bash
docker container run --rm -it --network host registry.quant1.com.br/arthur/q1-frpc/main <your q1 login>
```

Then proceed to configure the client.
