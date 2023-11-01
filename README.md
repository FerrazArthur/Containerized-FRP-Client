# Quant1-FRPC

This project provides a Docker container image that allows Quant1 developers to enable a personalized reverse proxy locally and share their applications globally via Quant1's FRP server.

## Usage

The intended use of this branch is using Docker Compose and a `.credentials` file, making it easier to manage. 

### Create credentials file

Create a `.credentials` file in the same directory as your `docker-compose.yaml` file. The `.credentials` file should contain your Quant1 username and password separated by a space, for example:

```bash
cat <my user> <my password> > .credentials
```

### Up and go

With that file and the docker-compose.yml in the same folder, you can then start the app with

```bash
docker compose up -d #you may want to ommit the -d flag in the first run, to make sure there were no errors and to get your acess url as well
```