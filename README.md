### Guide

For memory restriction, I'm run application in docker container in the next way:
1. Build from image:  
```docker build -t CONTAINER_NAME PATH_TO_DOCKERFILE```
2. Run container:  
```docker run -m 10M CONTAINER_NAME```
    