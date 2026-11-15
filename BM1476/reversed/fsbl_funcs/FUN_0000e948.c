//==================== FUN_0000e948 @ 0000e948 ====================

void FUN_0000e948(undefined4 param_1)

{
  coproc_moveto_Invalidate_Entire_Instruction(param_1);
  coproc_moveto_Instruction_cache(param_1,6);
  DataSynchronizationBarrier(0xf);
  InstructionSynchronizationBarrier(0xf);
  coproc_moveto_Control(0);
  InstructionSynchronizationBarrier(0xf);
  do {
    WaitForEvent();
  } while( true );
}

