/*

   BLIS    
   An object-based framework for developing high-performance BLAS-like
   libraries.

   Copyright (C) 2014, The University of Texas at Austin

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    - Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    - Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    - Neither the name of The University of Texas at Austin nor the names
      of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
   HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include "blis.h"


// NOTE: The values handled here may be queried even before bli_init()
// is called!



// -- General library information ----------------------------------------------

// This string gets defined via -D on the command line when BLIS is compiled.
// This string is (or rather, should be) only used here.
static char* bli_version_str       = BLIS_VERSION_STRING;
static char* bli_int_type_size_str = STRINGIFY_INT( BLIS_INT_TYPE_SIZE );

char* bli_info_get_version_str( void )              { return bli_version_str; }
char* bli_info_get_int_type_size_str( void )        { return bli_int_type_size_str; }



// -- General configuration-related --------------------------------------------

gint_t bli_info_get_int_type_size( void )           { return BLIS_INT_TYPE_SIZE; }
gint_t bli_info_get_num_fp_types( void )            { return BLIS_NUM_FP_TYPES; }
gint_t bli_info_get_max_type_size( void )           { return BLIS_MAX_TYPE_SIZE; }
gint_t bli_info_get_page_size( void )               { return BLIS_PAGE_SIZE; }
gint_t bli_info_get_simd_num_registers( void )      { return BLIS_SIMD_NUM_REGISTERS; }
gint_t bli_info_get_simd_size( void )               { return BLIS_SIMD_SIZE; }
gint_t bli_info_get_simd_align_size( void )         { return BLIS_SIMD_ALIGN_SIZE; }
gint_t bli_info_get_stack_buf_max_size( void )      { return BLIS_STACK_BUF_MAX_SIZE; }
gint_t bli_info_get_stack_buf_align_size( void )    { return BLIS_STACK_BUF_ALIGN_SIZE; }
gint_t bli_info_get_heap_addr_align_size( void )    { return BLIS_HEAP_ADDR_ALIGN_SIZE; }
gint_t bli_info_get_heap_stride_align_size( void )  { return BLIS_HEAP_STRIDE_ALIGN_SIZE; }
gint_t bli_info_get_pool_addr_align_size( void )    { return BLIS_POOL_ADDR_ALIGN_SIZE; }
gint_t bli_info_get_enable_stay_auto_init( void )
{
#ifdef BLIS_ENABLE_STAY_AUTO_INITIALIZED
	return 1;
#else
	return 0;
#endif
}
gint_t bli_info_get_enable_blas2blis( void )
{
#ifdef BLIS_ENABLE_BLAS2BLIS
	return 1;
#else
	return 0;
#endif
}
gint_t bli_info_get_enable_cblas( void )
{
#ifdef BLIS_ENABLE_CBLAS
	return 1;
#else
	return 0;
#endif
}
gint_t bli_info_get_blas2blis_int_type_size( void ) { return BLIS_BLAS2BLIS_INT_TYPE_SIZE; }



// -- Kernel implementation-related --------------------------------------------


// -- Level-3 kernel definitions --

char* bli_info_get_gemm_ukr_impl_string( ind_t method, num_t dt )
      { return bli_gks_l3_ukr_impl_string( BLIS_GEMM_UKR,       method, dt ); }
char* bli_info_get_gemmtrsm_l_ukr_impl_string( ind_t method, num_t dt )
      { return bli_gks_l3_ukr_impl_string( BLIS_GEMMTRSM_L_UKR, method, dt ); }
char* bli_info_get_gemmtrsm_u_ukr_impl_string( ind_t method, num_t dt )
      { return bli_gks_l3_ukr_impl_string( BLIS_GEMMTRSM_U_UKR, method, dt ); }
char* bli_info_get_trsm_l_ukr_impl_string( ind_t method, num_t dt )
      { return bli_gks_l3_ukr_impl_string( BLIS_TRSM_L_UKR,     method, dt ); }
char* bli_info_get_trsm_u_ukr_impl_string( ind_t method, num_t dt )
      { return bli_gks_l3_ukr_impl_string( BLIS_TRSM_U_UKR,     method, dt ); }



// -- Memory pool-related ------------------------------------------------------

gint_t bli_info_get_mk_pool_size( void ) { return bli_mem_pool_size( BLIS_BUFFER_FOR_A_BLOCK ); }
gint_t bli_info_get_kn_pool_size( void ) { return bli_mem_pool_size( BLIS_BUFFER_FOR_B_PANEL ); }
gint_t bli_info_get_mn_pool_size( void ) { return bli_mem_pool_size( BLIS_BUFFER_FOR_C_PANEL ); }



// -- BLIS implementation query (level-3) --------------------------------------

char* bli_info_get_gemm_impl_string( num_t dt )  { return bli_ind_oper_get_avail_impl_string( BLIS_GEMM,  dt ); }
char* bli_info_get_hemm_impl_string( num_t dt )  { return bli_ind_oper_get_avail_impl_string( BLIS_HEMM,  dt ); }
char* bli_info_get_herk_impl_string( num_t dt )  { return bli_ind_oper_get_avail_impl_string( BLIS_HERK,  dt ); }
char* bli_info_get_her2k_impl_string( num_t dt ) { return bli_ind_oper_get_avail_impl_string( BLIS_HER2K, dt ); }
char* bli_info_get_symm_impl_string( num_t dt )  { return bli_ind_oper_get_avail_impl_string( BLIS_SYMM,  dt ); }
char* bli_info_get_syrk_impl_string( num_t dt )  { return bli_ind_oper_get_avail_impl_string( BLIS_SYRK,  dt ); }
char* bli_info_get_syr2k_impl_string( num_t dt ) { return bli_ind_oper_get_avail_impl_string( BLIS_SYR2K, dt ); }
char* bli_info_get_trmm_impl_string( num_t dt )  { return bli_ind_oper_get_avail_impl_string( BLIS_TRMM,  dt ); }
char* bli_info_get_trmm3_impl_string( num_t dt ) { return bli_ind_oper_get_avail_impl_string( BLIS_TRMM3, dt ); }
char* bli_info_get_trsm_impl_string( num_t dt )  { return bli_ind_oper_get_avail_impl_string( BLIS_TRSM,  dt ); }

