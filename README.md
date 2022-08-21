# CSGO Trigger Bot v2

The original [CSGO-Trigger-Bot here](https://github.com/jcwml/CSGO-Trigger-Bot) is probably going to have a better trained network than anything this version can produce.

The difference is that in the original version I just kept generating and testing datasets until one of them came out working pretty well using the [TBVGG3 example project here](https://github.com/TFCNN/TFCNNv3) with nontarget data set to zero with the `NONTARGETS_ZERO` define and accidentally getting the normalisation wrong for the input images in the aim program which actually triggers the network into producing higher accuracy results oddly, and luckily enough! It came out, really quite well.

In this version I have modified the example project to such an extent that every time the network is retrained it should produce a slightly different network but every time it should have fairly good accuracy and misfire results.

Every time [`compile.sh`](compile.sh) is executed a new network will be trained and the aim program will be compiled and launched, or if you just want to launch the aim program with no training you can execute [`release.sh`](release.sh).
