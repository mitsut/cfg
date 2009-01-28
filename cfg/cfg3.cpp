/*
 *  TOPPERS Software
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 *
 *  Copyright (C) 2007-2008 by TAKAGI Nobuhisa
 * 
 *  上記著作権者は，以下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 * 
 */
#include <fstream>
#include <sstream>
#include "toppers/diagnostics.hpp"
#include "toppers/s_record.hpp"
#include "toppers/macro_processor.hpp"
#include "toppers/itronx/checker.hpp"
#include "cfg.hpp"
#include <boost/spirit.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

namespace
{
  using toppers::text_line;
  typedef toppers::macro_processor::element element;
  typedef toppers::macro_processor::var_t var_t;
  typedef toppers::macro_processor::context context;

  /*!
   *  \brief  シンボルに対応するアドレスの取得
   *  \param[in]  line      行番号
   *  \param[in]  arg_list  マクロ実引数リスト
   *  \param[in]  p_ctx     マクロコンテキスト
   *  \retval     マクロ返却値
   */
  var_t bf_symbol( text_line const& line, std::vector< var_t > const& arg_list, context const* p_ctx )
  {
    using namespace toppers;
    using namespace toppers::itronx;

    if ( macro_processor::check_arity( line, arg_list.size(), 1, "SYMBOL" ) )
    {
      std::string symbol( macro_processor::to_string( arg_list[0], p_ctx ) );
      std::tr1::shared_ptr< checker > chk = boost::any_cast< std::tr1::shared_ptr< checker > >( toppers::global( "checker" ) );
      nm_symbol::entry entry = chk->find( symbol );
      if ( entry.type >= 0 )
      {
        element e;
        e.i = entry.address;
        return var_t( 1, e );
      }
    }
    return var_t();
  }

  /*!
   *  \brief  指定したアドレスに格納されている値の取得
   *  \param[in]  line      行番号
   *  \param[in]  arg_list  マクロ実引数リスト
   *  \param[in]  p_ctx     マクロコンテキスト
   *  \retval     マクロ返却値
   *
   *  第1引数にアドレスを、第2引数に読み込むバイト数を指定します。
   */
  var_t bf_peek( text_line const& line, std::vector< var_t > const& arg_list, context const* p_ctx )
  {
    using namespace toppers;
    using namespace toppers::itronx;

    if ( macro_processor::check_arity( line, arg_list.size(), 2, "PEEK" ) )
    {
      std::size_t address = static_cast< std::size_t >( macro_processor::to_integer( arg_list[0], p_ctx ) );
      std::size_t size = static_cast< std::size_t >( macro_processor::to_integer( arg_list[1], p_ctx ) );
      std::tr1::shared_ptr< checker > chk = boost::any_cast< std::tr1::shared_ptr< checker > >( toppers::global( "checker" ) );

      std::map< std::string, var_t >::const_iterator le_iter( p_ctx->var_map.find( "LITTLE_ENDIAN" ) );
      if ( le_iter != p_ctx->var_map.end() )
      {
        bool little_endian = !!( *le_iter->second.front().i );
        element e;
        e.i = chk->get( address, size, !!little_endian );
        return var_t( 1, e );
      }
    }
    return var_t();
  }

}

bool cfg3_main()
{
  using namespace toppers;
  using namespace toppers::itronx;

  std::string kernel( boost::any_cast< std::string& >( global( "kernel" ) ) );
  itronx::factory factory( kernel );

  // *.cfgとcfg1_out.srecの読み込み
  std::string input_file;
  try
  {
    input_file = boost::any_cast< std::string const& >( toppers::global( "input-file" ) );
  }
  catch ( boost::bad_any_cast& )
  {
    fatal( _( "no input files" ) );
  }
  std::string cfg1_out_name( boost::any_cast< std::string& >( global( "cfg1_out" ) ) );
  std::auto_ptr< cfg1_out > cfg1_out( factory.create_cfg1_out( cfg1_out_name ) );

  codeset_t codeset = boost::any_cast< codeset_t >( global( "codeset" ) );
  cfg1_out->load_cfg( input_file, codeset, *factory.get_static_api_info_map() );
  cfg1_out->load_srec();
  cfg1_out::static_api_map api_map( cfg1_out->merge() );

  // ID番号の割付け
  assign_id( api_map );

  std::auto_ptr< checker > p_checker( factory.create_checker() );
  std::tr1::shared_ptr< checker > chk( p_checker );
  toppers::global( "checker" ) = chk;
  std::string rom_image( boost::any_cast< std::string& >( toppers::global( "rom-image" ) ) );
  std::string symbol_table( boost::any_cast< std::string& >( toppers::global( "symbol-table" ) ) );
  chk->load_rom_image( rom_image, symbol_table );

  // テンプレートファイル
  boost::any template_file( global( "template-file" ) );
  if ( template_file.empty() )
  {
    // テンプレートファイルが指定されていなければ最低限のチェックのみ（後方互換性のため）
    // パラメータチェック
    if ( !chk->check( *cfg1_out ) )
    {
      return false;
    }
  }
  else
  {
    namespace fs = boost::filesystem;

    // テンプレート処理
    std::auto_ptr< macro_processor > mproc( factory.create_macro_processor( factory.get_hook_on_assign(), *cfg1_out, api_map ) );

    toppers::macro_processor::func_t func_info;
    func_info.name = "SYMBOL";
    func_info.f = &bf_symbol;
    mproc->add_builtin_function( func_info );

    func_info.name = "PEEK";
    func_info.f = &bf_peek;
    mproc->add_builtin_function( func_info );

    fs::path cfg_dir( boost::any_cast< std::string& >( global( "cfg-directory" ) ), fs::native );
    std::vector< std::string > include_paths = boost::any_cast< std::vector< std::string > >( global( "include-path" ) );
    include_paths.push_back( cfg_dir.empty() ? "." : cfg_dir.native_file_string() );

    toppers::text in_text;
    toppers::text pp_text;
    std::string file_name( boost::any_cast< std::string& >( template_file ) );

    in_text.set_line( file_name, 1 );
    std::ifstream ifs( file_name.c_str() );
    if ( ifs.is_open() )
    {
      in_text.append( ifs );
      macro_processor::preprocess( in_text, pp_text );
      mproc->evaluate( pp_text );
    }

    if ( get_error_count() > 0 )
    {
      return false;
    }
    // 出力ファイルがあるかどうか分からないが、一応セーブする。
    output_file::save();
  }

  std::cerr << _( "check complete" ) << std::endl;

  return true;
}
