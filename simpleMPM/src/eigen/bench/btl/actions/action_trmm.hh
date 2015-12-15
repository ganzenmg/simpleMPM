//=====================================================
// File   :  action_matrix_matrix_product.hh
// Author :  L. Plagne <laurent.plagne@edf.fr)>
// Copyright (C) EDF R&D,  lun sep 30 14:23:19 CEST 2002
//=====================================================
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
#ifndef ACTION_TRMM
#define ACTION_TRMM
#include <string>

#include "../../../../eigen/bench/btl/generic_bench/init/init_function.hh"
#include "../../../../eigen/bench/btl/generic_bench/init/init_matrix.hh"
#include "../../../../eigen/bench/btl/generic_bench/init/init_vector.hh"
#include "../../../../eigen/bench/btl/generic_bench/utils/utilities.h"
#include "../../../../eigen/bench/btl/libs/STL/STL_interface.hh"

using namespace std;

template<class Interface>
class Action_trmm {

public :

  // Ctor

  Action_trmm( int size ):_size(size)
  {
    MESSAGE("Action_trmm Ctor");

    // STL matrix and vector initialization

    init_matrix<pseudo_random>(A_stl,_size);
    init_matrix<pseudo_random>(B_stl,_size);
    init_matrix<null_function>(X_stl,_size);
    init_matrix<null_function>(resu_stl,_size);

    for (int j=0; j<_size; ++j)
    {
      for (int i=0; i<j; ++i)
        A_stl[j][i] = 0;
      A_stl[j][j] += 3;
    }

    // generic matrix and vector initialization

    Interface::matrix_from_stl(A_ref,A_stl);
    Interface::matrix_from_stl(B_ref,B_stl);
    Interface::matrix_from_stl(X_ref,X_stl);

    Interface::matrix_from_stl(A,A_stl);
    Interface::matrix_from_stl(B,B_stl);
    Interface::matrix_from_stl(X,X_stl);

    _cost = 0;
    for (int j=0; j<_size; ++j)
    {
      _cost += 2*j + 1;
    }
    _cost *= _size;
  }

  // invalidate copy ctor

  Action_trmm( const  Action_trmm & )
  {
    INFOS("illegal call to Action_trmm Copy Ctor");
    exit(0);
  }

  // Dtor

  ~Action_trmm( void ){

    MESSAGE("Action_trmm Dtor");

    // deallocation

    Interface::free_matrix(A,_size);
    Interface::free_matrix(B,_size);
    Interface::free_matrix(X,_size);

    Interface::free_matrix(A_ref,_size);
    Interface::free_matrix(B_ref,_size);
    Interface::free_matrix(X_ref,_size);

  }

  // action name

  static inline std::string name( void )
  {
    return "trmm_"+Interface::name();
  }

  double nb_op_base( void ){
    return _cost;
  }

  inline void initialize( void ){

    Interface::copy_matrix(A_ref,A,_size);
    Interface::copy_matrix(B_ref,B,_size);
    Interface::copy_matrix(X_ref,X,_size);

  }

  inline void calculate( void ) {
      Interface::trmm(A,B,X,_size);
  }

  void check_result( void ){

    // calculation check

//     Interface::matrix_to_stl(X,resu_stl);
//
//     STL_interface<typename Interface::real_type>::matrix_matrix_product(A_stl,B_stl,X_stl,_size);
//
//     typename Interface::real_type error=
//       STL_interface<typename Interface::real_type>::norm_diff(X_stl,resu_stl);
//
//     if (error>1.e-6){
//       INFOS("WRONG CALCULATION...residual=" << error);
// //       exit(1);
//     }

  }

private :

  typename Interface::stl_matrix A_stl;
  typename Interface::stl_matrix B_stl;
  typename Interface::stl_matrix X_stl;
  typename Interface::stl_matrix resu_stl;

  typename Interface::gene_matrix A_ref;
  typename Interface::gene_matrix B_ref;
  typename Interface::gene_matrix X_ref;

  typename Interface::gene_matrix A;
  typename Interface::gene_matrix B;
  typename Interface::gene_matrix X;

  int _size;
  double _cost;

};


#endif



