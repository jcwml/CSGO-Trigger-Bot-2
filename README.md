# CSGO Trigger Bot v2

The original [CSGO-Trigger-Bot here](https://github.com/jcwml/CSGO-Trigger-Bot) has a pretty good trained network.

The difference is that in the original version I just kept generating and testing datasets until one of them came out working pretty well using the [TBVGG3 example project here](https://github.com/TFCNN/TFCNNv3) with nontarget data set to zero with the `NONTARGETS_ZERO` define and accidentally getting the normalisation wrong for the input images in the aim program which actually triggers the network into producing higher accuracy results oddly, and luckily enough! It came out, really quite well.

In this version I have modified the example project to such an extent that every time the network is retrained it should produce a slightly different network but every time it should have fairly good accuracy and misfire results.

Every time [`compile.sh`](compile.sh) is executed a new network will be trained and the aim program will be compiled and launched, or if you just want to launch the aim program with no training you can execute [`release.sh`](release.sh).

Enabling `#define SIGMOID_OUTPUT` in [aim.c](aim.c) may or may not improve the activation depending on the network trained, have a play around.

The supplied [network.save](network.save) is pretty good with a loss of `0.29`.

## Training Variability
Training can still be a bit touch and go, generally you want to aim for an `avg epoch loss` of 0.3 or less, but often the network will get stuck before that point, tests show this is primerily attributed to the random initialisation of the weights and then partly but less so, the random seed used for shuffling the training data. When a random seed is set, that seed is used for weight initialisation and then re-used for shuffling the data during training.

## Table of seeds
_These are all trained using an ADA8 network, OPTIM_NAG, and UNIFORM_GLOROT weight init. The training was set to end at a loss of less than 0.3 so it is possible that seeds in this table could reach a lower loss._
| Seed  | Epochs | Time Taken | loss |
| --- | --- | --- | --- |
| 1,947,939,716 | 15 | 252 sec (4.20 mins) | 0.29 |
| 3,681,819,285 | 16 | 268 sec (4.46 mins) | 0.29 |
| 2,205,898,327 | 22 | 358 sec (5.96 mins) | 0.29 |
| 2,780,854,223 | 33 | 877 sec (14.61 min)  | 0.29 |
| 636,220,169 | 57 | 890 sec (14.83 mins) | 0.29 |
| 3,677,601,131 | 59 | 1,544 sec (25.73 min) | 0.29 |
| 9,906,720 | 71 | 1,106 sec (18.43 mins) | 0.29 |
| 2,528,951,483 | 81 | 1,251 sec (20.85 mins) | 0.29 |
| 1,192,935,257 | 107 | 1,633 sec (27.21 mins) | 0.29 |

| Seed  | Epochs | Time Taken | loss |
| --- | --- | --- | --- |
| 1,947,939,716 | 109 | 1,283 sec (21.38 mins) | 0.23 |

