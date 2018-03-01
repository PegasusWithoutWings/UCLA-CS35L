# Homework

- Suppose the other teams really had been observing all the bytes going across the network. Our resulting network would still be secure because all the information is encrypted and the tapper cannot decrypt the message without having access to the private key.

  - Even if I assumed the other teams had also tapped our keyboards and had observed all of my team's keystrokes, our network is still secure because we did not type out the private key during any step of the process. Thus, the attacker would still have no access to the private key and cannot decrypt the bytes going across.

  - If the private key is stored in the USB drive, and the system in USB is not very well protected, then the attacker can gain access to the private key and impair the security of the network.

- The biggest loophole in the process is that the key zipped in the tarball can theoretically belong to anyone. If I am the attacked, I can just zip my own key together with malicious code together, encrypt it with my own key, and then send the tarball to the target. Because the malicious code is encrypted with my own key, `gpg --verify` cannot detect any abnormality. The key here is to verify that the public key belongs to the intended sender of the message. Instead of just zipping it in the tarball, it should be exchanged in a trusted way before the information exchange.