# sshd-server

[![en](https://img.shields.io/badge/lang-en-red)](README.md) [![pt-br](https://img.shields.io/badge/lang-pt--br-green)](README.pt-br.md)

## Configurations

```bash
AllowTcpForwarding yes                                                        
GatewayPorts yes                                                              
X11Forwarding yes  
Subsystem       sftp    internal-sftp
PermitRootLogin yes                                                  
PasswordAuthentication yes
```

```bash
ssh user@my-app.test
```
