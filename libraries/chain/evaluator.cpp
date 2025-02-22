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
#include <graphene/chain/database.hpp>
#include <graphene/chain/evaluator.hpp>
#include <graphene/chain/exceptions.hpp>
#include <graphene/chain/hardfork.hpp>
#include <graphene/chain/is_authorized_asset.hpp>
#include <graphene/chain/transaction_evaluation_state.hpp>

#include <graphene/chain/asset_object.hpp>
#include <graphene/chain/account_object.hpp>
#include <graphene/chain/fba_object.hpp>
#include <graphene/chain/committee_member_object.hpp>
#include <graphene/chain/market_evaluator.hpp>
#include <graphene/chain/protocol/fee_schedule.hpp>

#include <fc/uint128.hpp>

namespace graphene { namespace chain {
database& generic_evaluator::db()const { return trx_state->db(); }

   operation_result generic_evaluator::start_evaluate( transaction_evaluation_state& eval_state, const operation& op, bool apply )
   { try {
      trx_state   = &eval_state;
      //check_required_authorities(op);
      auto result = evaluate( op );

      if( apply ) result = this->apply( op );
      return result;
   } FC_CAPTURE_AND_RETHROW() }

   void generic_evaluator::prepare_fee(account_id_type account_id, asset fee, asset ufee)
   {
      const database& d = db();

      fee_from_account = fee;
      FC_ASSERT( fee.amount >= 0 );

      ufee_from_account = ufee;
      FC_ASSERT( ufee.amount >= 0 );

      fee_paying_account = &account_id(d);
      fee_paying_account_statistics = &fee_paying_account->statistics(d);

      FC_ASSERT( fee_from_account.asset_id == asset_id_type() );
      FC_ASSERT( ufee_from_account.asset_id == GRAPHENE_SDR_ASSET_ID );
//      core_fee_paid = fee_from_account.amount;
   }

   void generic_evaluator::pay_fee()
   { try {
     //SM!!! todo: check processing ufee_from_account
      if( !trx_state->skip_fee ) {
         database& d = db();
         /// TODO: db().pay_fee( account_id, core_fee );
         d.modify(*fee_paying_account_statistics, [&](account_statistics_object& s)
         {
            s.pay_fee( fee_paying_account->get_id(), fee_from_account, ufee_from_account, d);
         });
      }
   } FC_CAPTURE_AND_RETHROW() }

   sdualfee generic_evaluator::calculate_fee_for_operation(const operation& op) const
   {
     return db().current_fee_schedule().calculate_fee( op );
   }
   void generic_evaluator::db_adjust_balance(const account_id_type& fee_payer, asset fee_from_account)
   {
     db().adjust_balance(fee_payer, fee_from_account);
   }

} }
