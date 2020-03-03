# private-knn

Code for [Private Nearest Neighbors Classification](https://eprint.iacr.org/2018/289).

# Build Instructions
This repository uses the build system [Bazel](https://bazel.build/).
For each running time experiment in the paper, one docker image is built.
There are two methods to build these images:

## Provided docker image
We provide a docker image that contains all necessary external dependencies. The
only thing you need is a working docker instance on your system. You have to
pass the docker socket of your docker instance to the image, so it can build the
images and export them into your host's docker instance. To run the image,
simply do:

```
docker run -v /var/run/docker.sock:/var/run/docker.sock schoppmp/private-knn
```

### Manual build of the docker image
If you want, you can build the docker image that builds the experiments yourself. For this, you have to clone this repository, `cd` into it and run `docker build`:
```
docker build ./ --tag schoppmp/private-knn
```

## Manually
You can also build the images and everything else included in this repository
yourself via Bazel. While we tried to incorporate all dependencies into the
bazel build process, some have to be installed manually.

### Obliv-C Installation
Firstly, we use [Obliv-C](https://oblivc.org/) and you have to install its dependencies. Taken from their [the README](https://github.com/samee/obliv-c), these are the steps to take:

1. Installation of dependencies.
  * For Ubuntu: `sudo apt-get install ocaml libgcrypt20-dev ocaml-findlib opam m4`.
  * For Arch: `sudo pacman -S ocaml ocaml-findlib opam`

2. If you are using OPAM as your package manager, and this is the first time you are using it, set it up:
   ```
   opam init --compiler 4.06.0
   opam switch 4.06.0
   eval `opam config env`
   opam install camlp4 ocamlfind ocamlbuild batteries
   ```
   Version 4.06.0 just happened to be the most recent version when we tested. You can check what you have by running `opam switch list`, and try a more recent one.
   Note that this step seems to be unnecessary under Fedora.

*Obliv-C itself does not need to be build (you can skip step 3 and 4 of the Obliv-C README) as it will be built automatically by Bazel*

### Other dependencies
Besides the Obliv-C dependencies, the following are needed to build our project:
* `bazel`
* `git`
* `boost` 
* `boost-lib`
* `openssl`
* `cmake`
* `python2`
* `python3`
* `docker`

### Building

Finally, you have to clone this repository, `cd` into it build the project by running `bazel build //...`. To build
the experiments docker images and import them into your docker instance, do the
following:
```
bazel run //private_knn/experiments/idf_precomputation:idf_precomputation -- --norun
bazel run //private_knn/experiments/knn/two_party_phase:image -- --norun
bazel run //private_knn/experiments/knn/two_party_phase:non_uniform_sparsity -- --norun
bazel run //private_knn/experiments/knn/two_party_phase:realworld_nonzeros -- --norun
bazel run //private_knn/experiments/knn/multi_party_phase:topk -- --norun
```

# Running the experiments
After building, either manually or via the provided docker image, five tagged images should be available in your docker instance:
* `bazel/private_knn/experiments/idf_precomputation:idf_precomputation`
* `bazel/private_knn/experiments/knn/two_party_phase:image`
* `bazel/private_knn/experiments/knn/two_party_phase:non_uniform_sparsity`
* `bazel/private_knn/experiments/knn/two_party_phase:realworld_nonzeros`
* `bazel/private_knn/experiments/knn/multi_party_phase:topk`

Generally, to run the server for one of the experiments locally, do the following:
```
docker run -t --name <some name> --expose <some port> <image name> <image args>
```

To run the client for one of the experiments locally, do the following:
```
docker run -t --link <name of server container>:<name of server container> <image name> <image args>
```

The switch `-t` is required so that the parties can output result data
to stdout and `--name,--expose` respectively `--link` is required if
you want to try out the experiments locally.  Otherwise, you can
forward ports from the docker container to your host via `-p <host
port><container port>` and specify the IP of the server in the image
arguments via `--server.host <ip>` (*in both the client and server
invocations*). These Arguments (except `--server.host` which has to be
specified after, as it is an image argument) all have to come before
`<image name>` as all arguments after the image name are sent to the
binary that is run inside the container.


## Configuration
The experiments *Private IDF Precomputation* and *Sparse Matrix-Vector
Multiplication* can run for a very long time, especially if not run on
a powerful server, as we try many different parameters. To play around
with these images, you can try your own parameter combination by
changing the `.ini` files that control the experiments. The experiments
can be invoked with a different config file by specifing it as a volume
when starting both docker containers and pointing the binary to the
correct file via an environment variable:

```
docker run -t <other docker args omitted for brevity> -v <path_to_your_custom_config>:/<some name>.ini -e MPC_UTILS_CONFIG=/<some name>.ini <image name> <image args>
```

To see how the config files are structured, confront the toy config
and the default configs for each experiment, whose paths are specified
below.

Sadly, this does not work with the last phase, the *Top-k selection
phase* as it has hardcoded experiments at the moment. It should not
take as long however and more clearly show its progress, so you should
be able to play around with it even on a less powerful machine.

As an example you can run the toy example below locally with a new config `custom.ini`
that is in your current working directory by executing
```
docker run -t -v ./custom.ini:/custom.ini -e MPC_UTILS_CONFIG=/custom.ini --name private_knn_example_p0 --expose 12347 bazel/private_knn/experiments/knn/two_party_phase:image  --server.port 12347 --server.host private_knn_example_p0 --party 0
```
for the server and 
```
docker run -t -v ./custom.ini:/custom.ini -e MPC_UTILS_CONFIG=/custom.ini --link private_knn_example_p0:private_knn_example_p0 bazel/private_knn/experiments/knn/two_party_phase:image  --server.port 12347 --server.host private_knn_example_p0 --party 1
```
for the client.

## Toy example
There is a small example image, that just runs the sparse
matrix-vector multiplication for a single parameter combination. Its
config lies in
`private-knn/experiments/knn/two_party_phase/configs/knn.ini`. This
example should take around five minutes on a relatively new PC or
laptop.

The server can be run locally with 
```
docker run -t --name private_knn_example_p0 --expose 12347 bazel/private_knn/experiments/knn/two_party_phase:image  --server.port 12347 --server.host private_knn_example_p0 --party 0
```
and the client with 
```
docker run -t --link private_knn_example_p0:private_knn_example_p0 bazel/private_knn/experiments/knn/two_party_phase:image  --server.port 12347 --server.host private_knn_example_p0 --party 1
``` 

## Private IDF Precomputation
The config for this experiment lies in `private-knn/experiments/knn/idf_precomputation/id_precomputation.ini`.
To run the experiment of figure 5b locally, start the server via
```
docker run -t --name private_knn_idf_p0 --expose 12347 bazel/private_knn/experiments/idf_precomputation:idf_precomputation --server.port 12347 --server.host private_knn_idf_p0 --party 0
```
and the client via
```
docker run -t --link private_knn_idf_p0:private_knn_idf_p0 bazel/private_knn/experiments/idf_precomputation:idf_precomputation  --server.port 12347 --server.host private_knn_idf_p0 --party 1
``` 

## Sparse matrix-vector multiplicaton
The config for this experiment lies in `private-knn/experiments/knn/two_party_phase/configs/non_uniform_sparsity.ini`.
To run the experiment of figure 5a locally, start the server via
```
docker run -t --name private_knn_two_party1_p0 --expose 12347 bazel/private_knn/experiments/knn/two_party_phase:non_uniform_sparsity  --server.port 12347 --server.host private_knn_two_party1_p0 --party 0
```
and the client via
```
docker run -t --link private_knn_two_party1_p0:private_knn_two_party1_p0 bazel/private_knn/experiments/knn/two_party_phase:non_uniform_sparsity  --server.port 12347 --server.host private_knn_two_party1_p0 --party 1
``` 

To run the experiment of figure 6b locally, start the server via
```
docker run -t --name private_knn_two_party2_p0 --expose 12347 bazel/private_knn/experiments/knn/two_party_phase:realworld_nonzeros  --server.port 12347 --server.host private_knn_two_party2_p0  --party 0
```
and the client via
```
docker run -t --link private_knn_two_party2_p0:private_knn_two_party2_p0  bazel/private_knn/experiments/knn/two_party_phase:realworld_nonzeros  --server.port 12347 --server.host private_knn_two_party2_p0  --party 1
``` 

## Top-k selection phase
To run the experiment of figure 6a locally with 3 parties, start one server via
```
docker run -t --network host -e NUM_PARTIES=3 -e PARTY_ID=0
 bazel/private_knn/experiments/knn/multi_party_phase:topk
```
start another server via
```
docker run -t --network host -e NUM_PARTIES=3 -e PARTY_ID=1
 bazel/private_knn/experiments/knn/multi_party_phase:topk
```
and the client via
```
docker run -t --network host -e NUM_PARTIES=3 -e PARTY_ID=2
 bazel/private_knn/experiments/knn/multi_party_phase:topk
```
