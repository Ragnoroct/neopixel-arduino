## Test Without Arduino
```bash
MSYS_NO_PATHCONV=1 docker run -it --rm -v "$PWD":/usr/src/app -w /usr/src/app gcc:4.9 bash
```