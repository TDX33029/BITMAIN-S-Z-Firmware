//==================== FUN_0000e138 @ 0000e138 ====================

/* WARNING: Removing unreachable block (ram,0x0000e23c) */
/* WARNING: Removing unreachable block (ram,0x0000e268) */
/* WARNING: Removing unreachable block (ram,0x0000e274) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_0000e138(void)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  uint uVar4;
  
  iVar1 = func_0xffffcbec();
  if (iVar1 != 0) {
    uVar3 = func_0xffffc97c();
    func_0xffffde2c(s_g_g_g_0g__g_Pg__g_pg_g_g_g_g_g_g_00016943 + 0x11,uVar3);
    func_0xffffde2c(s_R_R_R_0R__R_PR__R_pR_R_R_R_R_R_R_00016403 + 0xd,0xa012);
    do {
    } while ((_DAT_e000002c & 8) == 0);
    func_0xffff9580();
  }
  _DAT_f8000008 = 0xdf0ddf0d;
  func_0xffffd6a4();
  func_0xffffd700();
  func_0xffffdbcc();
  func_0xffffdbd0(1,0x1b28,0);
  func_0xffffdbd0(2,0x1b0c,0);
  func_0xffffdbd0(3,0x1af0,0);
  func_0xffffdbd0(4,0x1ad4,0);
  func_0xffffdbd0(5,0x1ab8,0);
  func_0xffffdbd0(6,0x1a9c,0);
  func_0xffffde2c(s_g_g_g_0g__g_Pg__g_pg_g_g_g_g_g_g_00016943 + 0x29);
  func_0xffffde2c(s_h_h_h_0h__h_Ph__h_ph_h_h_h_h_h_h_00016983 + 0x19,0x7e2,3,
                  s_h_h_h_0h__h_Ph__h_ph_h_h_h_h_h_h_00016983 + 0xd,
                  s_h_h_h_0h__h_Ph__h_ph_h_h_h_h_h_h_00016983 + 0x31);
  _DAT_00200000 = 0xaa55aa55;
  _DAT_00100000 = 0xaa55aa55;
  iVar1 = func_0xffffc174();
  if (iVar1 == 1) {
    func_0xffffde2c(s_i_i_i_0i__i_Pi__i_pi_i_i_i_i_i_i_000169c3 + 0x11);
    func_0xffffde2c(s_R_R_R_0R__R_PR__R_pR_R_R_R_R_R_R_00016403 + 0xd,0xa002);
    do {
    } while ((_DAT_e000002c & 8) == 0);
    func_0xffff9580();
  }
  func_0xffffde2c(s_i_i_i_0i__i_Pi__i_pi_i_i_i_i_i_i_000169c3 + 0x25);
  func_0xffffa560();
  DAT_00021f94 = FUN_00000714(DAT_00028e74);
  if ((DAT_00021f94 & 0x1000) != 0) {
    func_0xffffde2c(s_j_j_j_0j__j_Pj__j_pj_j_j_j_j_j_j_00016a03 + 5);
  }
  _DAT_f8000258 = _DAT_f8000258 | 0x60000000;
  uVar4 = _DAT_f800025c & 7;
  if (uVar4 == 4) {
    func_0xffffde2c(s_j_j_j_0j__j_Pj__j_pj_j_j_j_j_j_j_00016a03 + 0x31);
    iVar1 = func_0xffffbe84();
    if (iVar1 != 0) {
      func_0xffffde2c(s_k_k_k_0k__k_Pk__k_pk_k_k_k_k_k_k_00016a43 + 5);
      func_0xffffde2c(s_R_R_R_0R__R_PR__R_pR_R_R_R_R_R_R_00016403 + 0xd,0xa00a);
      do {
      } while ((_DAT_e000002c & 8) == 0);
      func_0xffffa790();
    }
    DAT_00021bf8 = 0x2fa8;
    func_0xffffde2c(s_k_k_k_0k__k_Pk__k_pk_k_k_k_k_k_k_00016a43 + 0x19);
  }
  else if (uVar4 == 2) {
    func_0xffffde2c(s_k_k_k_0k__k_Pk__k_pk_k_k_k_k_k_k_00016a43 + 0x2d);
    func_0xffffc118();
    func_0xffffde2c(s_l_l_l_0l__l_Pl__l_pl_l_l_l_l_l_l_00016a83 + 1);
    DAT_00021bf8 = 0x31ac;
  }
  else if (uVar4 == 5) {
    func_0xffffde2c(s_l_l_l_0l__l_Pl__l_pl_l_l_l_l_l_l_00016a83 + 0x15);
    iVar1 = func_0xffffcff4(s_l_l_l_0l__l_Pl__l_pl_l_l_l_l_l_l_00016a83 + 0x29);
    if (iVar1 != 0) {
      func_0xffffde2c(s_l_l_l_0l__l_Pl__l_pl_l_l_l_l_l_l_00016a83 + 0x35);
      func_0xffffde2c(s_R_R_R_0R__R_PR__R_pR_R_R_R_R_R_R_00016403 + 0xd,0xa009);
      do {
      } while ((_DAT_e000002c & 8) == 0);
      func_0xffffa790();
    }
    DAT_00021bf8 = 0x4124;
    func_0xffffde2c(s_m_m_m_0m__m_Pm__m_pm_m_m_m_m_m_m_00016ac3 + 5);
  }
  else if (uVar4 == 6) {
    func_0xffffde2c(s_m_m_m_0m__m_Pm__m_pm_m_m_m_m_m_m_00016ac3 + 0x15);
    iVar1 = func_0xffffcff4(s_l_l_l_0l__l_Pl__l_pl_l_l_l_l_l_l_00016a83 + 0x29);
    if (iVar1 != 0) {
      func_0xffffde2c(s_m_m_m_0m__m_Pm__m_pm_m_m_m_m_m_m_00016ac3 + 0x2d);
      func_0xffffde2c(s_R_R_R_0R__R_PR__R_pR_R_R_R_R_R_R_00016403 + 0xd,0xa009);
      do {
      } while ((_DAT_e000002c & 8) == 0);
      func_0xffffa790();
    }
    DAT_00021bf8 = 0x4124;
    func_0xffffde2c(s_m_m_m_0m__m_Pm__m_pm_m_m_m_m_m_m_00016ac3 + 0x3d);
  }
  else if (uVar4 == 0) {
    func_0xffffde2c(s_n_n_n_0n__n_Pn__n_pn_n_n_n_n_n_n_00016b03 + 0x11);
    if ((_DAT_f800700c & 4) != 0) {
      func_0xffffcb4c();
      _DAT_f8000008 = 0xdf0ddf0d;
    }
    _DAT_f8000258 = _DAT_f8000258 & 0xfffffff;
    _DAT_f8000004 = 0x767b767b;
    FUN_0000e948();
  }
  else {
    func_0xffffde2c(s_n_n_n_0n__n_Pn__n_pn_n_n_n_n_n_n_00016b03 + 0x25);
    func_0xffffde2c(s_R_R_R_0R__R_PR__R_pR_R_R_R_R_R_R_00016403 + 0xd,0xa000);
    do {
    } while ((_DAT_e000002c & 8) == 0);
    func_0xffffa790();
  }
  func_0xffffde2c(s_n_n_n_0n__n_Pn__n_pn_n_n_n_n_n_n_00016b03 + 0x3d,DAT_0002105c);
  if ((DAT_0002105c + 0x1f000000U & 0xfeffffff) != 0 &&
      (DAT_0002105c != -0x4000000 && DAT_0002105c != -0x1ff00000)) {
    func_0xffffde2c(s_o_o_o_0o__o_Po__o_po_o_o_o_o_o_o_00016b43 + 0x1d);
    func_0xffffde2c(s_R_R_R_0R__R_PR__R_pR_R_R_R_R_R_R_00016403 + 0xd,0xa006);
    do {
    } while ((_DAT_e000002c & 8) == 0);
    func_0xffffa790();
  }
  if (DAT_0002105c == -0x1e000000) {
    func_0xffffde2c(s_o_o_o_0o__o_Po__o_po_o_o_o_o_o_o_00016b43 + 0x39);
    DAT_00021060 = 1;
  }
  DAT_00021f90 = 1;
  iVar2 = func_0xffffabcc();
  if (iVar2 != 0) {
    func_0xffffde2c(s_p_p_p_0p__p_Pp__p_pp_p_p_p_p_p_p_00016b83 + 0x11);
    func_0xffffa790();
  }
  uVar3 = func_0xffff9dc4();
  func_0xffffde2c(s_p_p_p_0p__p_Pp__p_pp_p_p_p_p_p_p_00016b83 + 0x31,uVar3);
  func_0xffffa8e0(uVar3);
  return iVar1;
}

