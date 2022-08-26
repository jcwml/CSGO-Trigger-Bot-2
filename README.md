# CSGO Trigger Bot v2

The original [CSGO-Trigger-Bot here](https://github.com/jcwml/CSGO-Trigger-Bot) has a pretty good trained network.

The difference is that in the original version I just kept generating and testing datasets until one of them came out working pretty well using the [TBVGG3 example project here](https://github.com/TFCNN/TFCNNv3) with nontarget data set to zero with the `NONTARGETS_ZERO` define and accidentally getting the normalisation wrong for the input images in the aim program which actually triggers the network into producing higher accuracy results oddly, and luckily enough! It came out, really quite well.

In this version I have modified the example project to such an extent that every time the network is retrained it should produce a slightly different network but every time it should have fairly good accuracy and misfire results.

Every time [`compile.sh`](compile.sh) is executed a new network will be trained and the aim program will be compiled and launched, or if you just want to launch the aim program with no training you can execute [`release.sh`](release.sh).

Enabling `#define SIGMOID_OUTPUT` in [aim.c](aim.c) may or may not improve the activation depending on the network trained, have a play around.

The supplied [network.save](network.save) is pretty good with a loss of `0.225977`.

## Training Variability
Training can still be a bit touch and go, generally you want to aim for an `avg epoch loss` of 0.3 or less, but often the network will get stuck before that point, tests show this is primerily attributed to the random initialisation of the weights and then partly but less so, the random seed used for shuffling the training data. When a random seed is set, that seed is used for weight initialisation and then re-used for shuffling the data during training.

## Table of seeds
_These are all trained using an ADA8 network, OPTIM_NAG, and UNIFORM_GLOROT weight init. The training was set to end at a loss of less than 0.3 so it is possible that seeds in this table could reach a lower loss. Time Taken is not a standardised metric, sometimes I run more processes than my CPU can handle and cause each process to take longer, epochs is a better gauge of time taken._
| Seed  | Epochs | Time Taken | loss |
| --- | --- | --- | --- |
| 1185951401 | 13 | 222 sec (3.70 mins) | 0.29 |
| 1947939716 | 15 | 252 sec (4.20 mins) | 0.29 |
| 3681819285 | 16 | 268 sec (4.46 mins) | 0.29 |
| 2205898327 | 22 | 358 sec (5.96 mins) | 0.29 |
| 4028114920 | 24 | 391 sec (6.52 mins) | 0.29 |
| 1096038209 | 24 | 392 sec (6.53 mins) | 0.29 |
| 2780854223 | 33 | 877 sec (14.61 min)  | 0.29 |
| 636220169 | 57 | 890 sec (14.83 mins) | 0.29 |
| 3677601131 | 59 | 1,544 sec (25.73 min) | 0.29 |
| 9906720 | 71 | 1,106 sec (18.43 mins) | 0.29 |
| 2528951483 | 81 | 1,251 sec (20.85 mins) | 0.29 |
| 1192935257 | 107 | 1,633 sec (27.21 mins) | 0.29 |

| Seed  | Epochs | Time Taken | loss |
| --- | --- | --- | --- |
| 1931370444 | 55 | 1,455 sec (24.25 mins) | 0.23 |
| 2910301494 | 71 | 1,120 sec (18.67 mins) | 0.23 |
| 3721615797 | 81 | 2,258 sec (37.63 mins) | 0.23 |
| 2380237492 | 104 | 2,532 sec (42.20 mins) | 0.23 |
| 1947939716 | 109 | 1,283 sec (21.38 mins) | 0.23 |

| Seed  | Epochs | Time Taken | loss |
| --- | --- | --- | --- |
| 1931370444 | 116 | 1,701 sec (28.35 mins) | 0.22 |


