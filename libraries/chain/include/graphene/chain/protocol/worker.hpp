/*
 * Copyright (c) 2015 Cryptonomex, Inc., and contributors.
 *
 * The MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#pragma once
#include <graphene/chain/protocol/base.hpp>
#include <graphene/chain/protocol/memo.hpp>

namespace graphene { namespace chain {

struct service_create_operation : public base_operation
{     
   struct fee_parameters_type { 
     uint64_t fee = 5000*GRAPHENE_BLOCKCHAIN_PRECISION; 
     uint64_t ufee = 0;      //SDR fee per operation
     uint64_t ufee_pkb = 0;  //SDR fee per kilobyte in memo
   };

   asset                fee;
   asset                ufee = asset( 0, GRAPHENE_SDR_ASSET_ID );  //fee in sdr

   account_id_type      owner;
   string               name;
   memo_group           p_memo;

   account_id_type   fee_payer()const { return owner; }
   void              validate()const;
   dualfee           calculate_fee(const fee_parameters_type& k)const;
};  

struct service_update_operation : public base_operation
{     
   struct fee_parameters_type { 
     uint64_t fee = 5000*GRAPHENE_BLOCKCHAIN_PRECISION; 
     uint64_t ufee = 0;      //SDR fee per operation
     uint64_t ufee_pkb = 0;  //SDR fee per kilobyte in memo
   };

   asset                fee;
   asset                ufee = asset( 0, GRAPHENE_SDR_ASSET_ID );  //fee in sdr

   account_id_type      owner;
   service_id_type      service_to_update;

   memo_group           p_memo;

   account_id_type   fee_payer()const { return owner; }
   void              validate()const;
   dualfee           calculate_fee(const fee_parameters_type& k)const;
};  

struct bid_request_create_operation : public base_operation
{     
   struct fee_parameters_type { 
     uint64_t fee = 5000*GRAPHENE_BLOCKCHAIN_PRECISION; 
     uint64_t ufee = 0;      //SDR fee per operation
     uint64_t ufee_pkb = 0;  //SDR fee per kilobyte in memo
   };

   asset                fee;
   asset                ufee = asset( 0, GRAPHENE_SDR_ASSET_ID );  //fee in sdr

   account_id_type      owner;
   string               name;

   flat_set<asset_id_type> assets;
   flat_set<account_id_type> providers;
   memo_group           p_memo;

   time_point_sec expiration = time_point_sec::maximum();

   account_id_type   fee_payer()const { return owner; }
   void              validate()const;
   dualfee           calculate_fee(const fee_parameters_type& k)const;
};

struct bid_request_expired_operation : public base_operation
{
  struct fee_parameters_type { };

  asset               fee;
  asset               ufee = asset( 0, GRAPHENE_SDR_ASSET_ID );  //fee in sdr

  bid_request_id_type bid_request_id;
  /** must be order->seller */
  account_id_type     fee_paying_account;

  account_id_type fee_payer()const { return fee_paying_account; }
  void            validate()const { FC_ASSERT( !"virtual operation" ); }
  /// This is a virtual operation; there is no fee
  dualfee         calculate_fee(const fee_parameters_type& k)const { return dualfee{0,0}; }
};

struct bid_request_cancel_operation : public base_operation
{
  struct fee_parameters_type { 
    uint64_t fee = 0; 
    uint64_t ufee = 0;      //SDR fee per operation
  };

  asset               fee;
  asset               ufee = asset( 0, GRAPHENE_SDR_ASSET_ID );  //fee in sdr

  bid_request_id_type bid_request_id;
  /** must be order->seller */
  account_id_type     fee_paying_account;

  account_id_type fee_payer()const { return fee_paying_account; }
  void            validate()const;
  dualfee         calculate_fee(const fee_parameters_type& k)const;
};

struct bid_create_operation : public base_operation
{     
   struct fee_parameters_type { 
     uint64_t fee = 5000*GRAPHENE_BLOCKCHAIN_PRECISION; 
     uint64_t ufee = 0;      //SDR fee per operation
     uint64_t ufee_pkb = 0;  //SDR fee per kilobyte in memo
   };

   asset                fee;
   asset                ufee = asset( 0, GRAPHENE_SDR_ASSET_ID );  //fee in sdr

   account_id_type      owner;
   string               name;

   bid_request_id_type  request;
   memo_data            p_memo;

   time_point_sec expiration = time_point_sec::maximum();
   
   account_id_type   fee_payer()const { return owner; }
   void              validate()const;
   dualfee           calculate_fee(const fee_parameters_type& k)const;
};

struct bid_expired_operation : public base_operation
{
  struct fee_parameters_type {};

  asset               fee;
  asset               ufee = asset( 0, GRAPHENE_SDR_ASSET_ID );  //fee in sdr

  bid_id_type         bid_id;
  /** must be bid->owner */
  account_id_type     fee_paying_account;

  account_id_type fee_payer()const { return fee_paying_account; }
  void            validate()const { FC_ASSERT( !"virtual operation" ); }
  /// This is a virtual operation; there is no fee
  dualfee      calculate_fee(const fee_parameters_type& k)const { return dualfee{0,0}; }
};

struct bid_cancel_operation : public base_operation
{
  struct fee_parameters_type { 
    uint64_t fee = 0;
    uint64_t ufee = 0;      //SDR fee per operation
  };

  asset               fee;
  asset               ufee = asset( 0, GRAPHENE_SDR_ASSET_ID );  //fee in sdr

  bid_id_type         bid_id;
  /** must be bid->owner */
  account_id_type     fee_paying_account;

  account_id_type fee_payer()const { return fee_paying_account; }
  void            validate()const;
  dualfee         calculate_fee(const fee_parameters_type& k)const;
};

struct fee_pay_operation : public base_operation
{
  struct fee_parameters_type { };

  asset               fee;
  asset               ufee = asset( 0, GRAPHENE_SDR_ASSET_ID );

  asset               paid_fee;
  asset               paid_ufee;
  /** must be bid->owner */
  account_id_type     fee_from_account;
  account_id_type     fee_to_account;

  account_id_type fee_payer()const { return fee_from_account; }
  void            validate()const { FC_ASSERT( !"virtual operation" ); }
  /// This is a virtual operation; there is no fee
  dualfee      calculate_fee(const fee_parameters_type& k)const { return dualfee{0,0}; }
};

} }

FC_REFLECT( graphene::chain::service_create_operation::fee_parameters_type, (fee)(ufee)(ufee_pkb) )
FC_REFLECT( graphene::chain::service_create_operation,
            (fee)(ufee)(owner)(name)(p_memo) )

FC_REFLECT( graphene::chain::service_update_operation::fee_parameters_type, (fee)(ufee)(ufee_pkb) )
FC_REFLECT( graphene::chain::service_update_operation,
            (fee)(ufee)(owner)(service_to_update)(p_memo) )

FC_REFLECT( graphene::chain::bid_request_create_operation::fee_parameters_type, (fee)(ufee)(ufee_pkb) )
FC_REFLECT( graphene::chain::bid_request_create_operation,
            (fee)(ufee)(owner)(name)(assets)(providers)(p_memo)(expiration) )

FC_REFLECT( graphene::chain::bid_request_expired_operation::fee_parameters_type, )
FC_REFLECT( graphene::chain::bid_request_expired_operation,(fee)(ufee)(fee_paying_account)(bid_request_id) )
            
FC_REFLECT( graphene::chain::bid_request_cancel_operation::fee_parameters_type, (fee)(ufee) )
FC_REFLECT( graphene::chain::bid_request_cancel_operation,(fee)(ufee)(fee_paying_account)(bid_request_id) )

FC_REFLECT( graphene::chain::bid_create_operation::fee_parameters_type, (fee)(ufee)(ufee_pkb) )
FC_REFLECT( graphene::chain::bid_create_operation,
            (fee)(ufee)(owner)(name)(request)(p_memo)(expiration) )

FC_REFLECT( graphene::chain::bid_expired_operation::fee_parameters_type, )
FC_REFLECT( graphene::chain::bid_expired_operation,(fee)(ufee)(fee_paying_account)(bid_id) )

FC_REFLECT( graphene::chain::bid_cancel_operation::fee_parameters_type, (fee)(ufee) )
FC_REFLECT( graphene::chain::bid_cancel_operation,(fee)(ufee)(fee_paying_account)(bid_id) )

FC_REFLECT( graphene::chain::fee_pay_operation::fee_parameters_type, )
FC_REFLECT( graphene::chain::fee_pay_operation,(fee)(ufee)(paid_fee)(paid_ufee)(fee_from_account)(fee_to_account) )
