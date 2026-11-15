//==================== FUN_0000ddd4 @ 0000ddd4 ====================

void FUN_0000ddd4(void)

{
  int iVar1;
  undefined8 in_d0;
  undefined4 uVar2;
  
  iVar1 = FUN_0000c540();
  if (iVar1 != 0) {
    *(undefined8 *)(iVar1 + 0x18) = in_d0;
    uVar2 = VectorFloatToSigned(in_d0,3);
    *(undefined4 *)(iVar1 + 0xc) = 3;
    *(undefined4 *)(iVar1 + 0x14) = uVar2;
  }
  return;
}

