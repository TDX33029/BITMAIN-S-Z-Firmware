//==================== FUN_000108f8 @ 000108f8 ====================

void FUN_000108f8(undefined4 param_1)

{
  FUN_000107f0("Useage:\r");
  FUN_000107bc("\t%s [option] [paramaters]\n\n",param_1);
  FUN_000107f0("\tOption:{-f} {-s} {-p} {-n} {-x} {-q}\n");
  FUN_000107f0(
              "\t\t-f [minerType] [fileName] [rootPublicKeyFile]: \n\t\t\tOnly Check If Filename was Valided.\n"
              );
  FUN_000107f0(
              "\t\t-s [minerType] [fileName] [rootPublicKeyFile]: \n\t\t\tCheck If Filename Was Valided, and Splite Bmu To \"/tmp/tmpfw/\"\n"
              );
  FUN_000107f0(
              "\t\t-p [minerType] [fileName] [rootPublicKeyFile]: \n\t\t\tCheck If Filename Was Valided, and Dump BmuComments\n"
              );
  FUN_000107f0(
              "\t\t-x [minerType] [fileName] [rootPublicKeyFile]: \n\t\t\tCheck If Filename Was FullSize BMU\n"
              );
  FUN_000107f0("\t\t-n [nandBinFile]: \n\t\t\tSplite SigImg To Single File To \"/tmp/tmpNand/\"\n");
  FUN_000107f0("\t\t-q: \n\t\t\tGenerate A 256Bytes 0xff File To \"/tmp/256BFF\"\n");
  FUN_000107f0("\tReturns:");
  FUN_000107f0("\t\t0: \n\t\t\tWell Done!\n");
  FUN_000107f0("\t\tOthers: \n\t\t\tSomething Wrong!\n");
  return;
}

