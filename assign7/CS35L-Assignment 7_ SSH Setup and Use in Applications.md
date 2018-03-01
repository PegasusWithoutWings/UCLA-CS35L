# CS35L-Assignment 7: SSH Setup and Use in Applications

## Lab

### Prelude

Because I am using the latest version of Macbook Pro that does not even have USB port, it took me some extra trouble to set the Baglebone up. First I have to borrow a Windows laptop from my friend and then walk through the setup instructions posted in Piazza. Then, after I get it connected to UCLA_WIFI, I can finally connect to it wirelessly and start working on it on my own Macbook. The internal ip address of Baglebone can be found in the output of `ifconfig` with label `inet address` under `wlan0` section. 

```txt
wlan0     Link encap:Ethernet  HWaddr 2c:f7:f1:06:6b:54
          inet addr:128.97.180.16  Bcast:128.97.180.255  Mask:255.255.255.0
```

Now I can start working on our Baglebone on my own laptop.

### Server Setup

1. We executed `ssh-keygen` to generate public and private keys in our Baglebone server. I choose not to add passphrase.

1. Then we executed `sudo useradd -d /home/guest/ -m guest` to create a guest account for other people to remotely connect to. We also used `sudo passwd guest` to add a password to the guest account.

1. Then we created a `.ssh` directory for the new user by first changing directory into it with `cd /home/guest/` and then `sudo mkdir .ssh`.

1. Because we created `.ssh` with `sudo`, it is owned by `root` user. We now need to change the ownership on `.ssh` directory with `sudo chown -R guest .ssh`. Then we will change it permission with `sudo chmod 700 .ssh` so that it will not be edited by unintended users.

1. We will now disable password-based authentication so that users can only connect to the server through `ssh` agent. It ensures that only permitted users can access the server and reduce the risk of attacks from leaked passwords.

1. Asked David to text me his public key and then added it to the `authorized_keys` file under the `.ssh` directory of the guest account.

1. Asked Kevin to text me his public key and then added it to the `authorized_keys` file under the `.ssh` directory of the guest account.

### Client Setup

Now we will set up my Macbook Pro as a client to get connected to our Baglebone server. 

1. I have already generated public and private keys when working with Github, so I do not have to generate them again.

1. Then we copied the public key to the server for key-based authentication with `ssh-copy-id –i guest@128.97.180.16`.

1. We will now add the private key to authentication agent so that we will not need to type our password every time using `ssh`.

1. Finally we can `ssh` to the server with `ssh -X guest@128.97.180.16`.

1. The X forwarding works like a charm. I can run firefox on my Baglebone and see it on my own laptop.

### SSH into Teammate David's Beaglebone

1. David has his Beaglebone connected to CR-3760-wifi.

1. David run `ifconfig` in his Beaglebone to get the internal IP Address of his Beaglebone. It is `10.97.85.240`.

1. I texted David my public key and asked him to add my public key into his `authorized_keys` file under `.ssh` directory in the guest account.

1. After he finished, I executed `ssh guest@10.97.85.240` and successfully connected to David's Beaglebone.

### SSH into Teammate Kevin's Beaglebone

1. Kevin has his Beaglebone connected to CR-3760-wifi.

1. Kevin run `ifconfig` in his Beaglebone to get the internal IP Address of his Beaglebone. It is `10.97.85.236`.

1. I texted Kevin my public key and asked him to add my public key into his `authorized_keys` file under `.ssh` directory in the guest account.

1. After he finished, I executed `ssh guest@10.97.85.236` and successfully connected to Kevin's Beaglebone.

## Homework

- Suppose the other teams really had been observing all the bytes going across the network. Our resulting network would still be secure because all the information is encrypted and the tapper cannot decrypt the message without having access to the private key.

  - Even if I assumed the other teams had also tapped our keyboards and had observed all of my team's keystrokes, our network is still secure because we did not type out the private key during any step of the process. Thus, the attacker would still have no access to the private key and cannot decrypt the bytes going across.

  - However, if I assume that we are booting off USB and other teams temporarily had physical control of the USB, then we will be screwed. The attackers can carry out all kinds of attack through the USB port. For example, he can insert a USB drive that functions as a boot device and inject malware into my system during the boot process. That way, he will have complete control over our devices and the resulting network will surely not be secure.