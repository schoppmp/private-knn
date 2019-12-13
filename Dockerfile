## This dockerfile is completely optional, used if you do
## not want to install the dependencies. It will build 
## a docker image that can build all experiments in this repo and 
## export them into your docker instance. See README.md for instructions.

FROM schoppmp/bazel-oblivc-builder@sha256:a3b3bfe8f52bd530450aa08e747e6e4d2d9540b5d6fc4dae25d0298d62ca4ee1

# TODO: change to git clone maybe in run.sh so that it gets the actual
COPY ./ /private-knn
COPY run_build.sh /run_build.sh

WORKDIR /

ENTRYPOINT [""]
CMD ["bash", "/run_build.sh"]
