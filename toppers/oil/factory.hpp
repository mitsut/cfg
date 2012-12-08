/*
 *  TOPPERS Software
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 *
 *  Copyright (C) 2007-2012 by TAKAGI Nobuhisa
 *  Copyright (C) 2010 by Meika Sugimoto
 * 
 *  �嵭����Ԥϡ��ʲ���(1)��(4)�ξ������������˸¤ꡤ�ܥ��եȥ���
 *  �����ܥ��եȥ���������Ѥ�����Τ�ޤࡥ�ʲ�Ʊ���ˤ���ѡ�ʣ������
 *  �ѡ������ۡʰʲ������ѤȸƤ֡ˤ��뤳�Ȥ�̵���ǵ������롥
 *  (1) �ܥ��եȥ������򥽡��������ɤη������Ѥ�����ˤϡ��嵭������
 *      ��ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ��꤬�����Τޤޤη��ǥ���
 *      ����������˴ޤޤ�Ƥ��뤳�ȡ�
 *  (2) �ܥ��եȥ������򡤥饤�֥������ʤɡ�¾�Υ��եȥ�������ȯ�˻�
 *      �ѤǤ�����Ǻ����ۤ�����ˤϡ������ۤ�ȼ���ɥ�����ȡ�����
 *      �ԥޥ˥奢��ʤɡˤˡ��嵭�����ɽ�����������Ѿ�浪��Ӳ���
 *      ��̵�ݾڵ����Ǻܤ��뤳�ȡ�
 *  (3) �ܥ��եȥ������򡤵�����Ȥ߹���ʤɡ�¾�Υ��եȥ�������ȯ�˻�
 *      �ѤǤ��ʤ����Ǻ����ۤ�����ˤϡ����Τ����줫�ξ�����������
 *      �ȡ�
 *    (a) �����ۤ�ȼ���ɥ�����ȡ����Ѽԥޥ˥奢��ʤɡˤˡ��嵭����
 *        �ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ����Ǻܤ��뤳�ȡ�
 *    (b) �����ۤη��֤��̤�������ˡ�ˤ�äơ�TOPPERS�ץ��������Ȥ�
 *        ��𤹤뤳�ȡ�
 *  (4) �ܥ��եȥ����������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������뤤���ʤ�»
 *      ������⡤�嵭����Ԥ����TOPPERS�ץ��������Ȥ����դ��뤳�ȡ�
 *      �ޤ����ܥ��եȥ������Υ桼���ޤ��ϥ���ɥ桼������Τ����ʤ���
 *      ͳ�˴�Ť����ᤫ��⡤�嵭����Ԥ����TOPPERS�ץ��������Ȥ�
 *      ���դ��뤳�ȡ�
 * 
 *  �ܥ��եȥ������ϡ�̵�ݾڤ��󶡤���Ƥ����ΤǤ��롥�嵭����Ԥ�
 *  ���TOPPERS�ץ��������Ȥϡ��ܥ��եȥ������˴ؤ��ơ�����λ�����Ū
 *  ���Ф���Ŭ������ޤ�ơ������ʤ��ݾڤ�Ԥ�ʤ����ޤ����ܥ��եȥ���
 *  �������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������������ʤ�»���˴ؤ��Ƥ⡤��
 *  ����Ǥ�����ʤ���
 * 
 */
/*!
 *  \file   toppers/oil/factory.hpp
 *  \brief  �����ͥ�ޤ��ϥ⥸�塼��˱������������֥������������˴ؤ���������
 *
 *  ���Υե�������������륯�饹
 *  \code
 *  class factory;
 *  \endcode
 */
#ifndef TOPPERS_OIL_FACTORY_HPP_
#define TOPPERS_OIL_FACTORY_HPP_

#include <memory>
#include <string>
#include <vector>
#include "toppers/macro_processor.hpp"
#include "toppers/oil/checker.hpp"
#include "toppers/oil/cfg1_out.hpp"

namespace toppers
{
  namespace oil
  {
    /*!
     *  \class  factory factory.hpp "toppers/oil/factory.hpp"
     *  \brief  �����ͥ�ޤ��ϥ⥸�塼��˱������������֥��������������饹
     */
    class factory
    {
    public:
      typedef oil::cfg1_out cfg1_out;
      typedef oil::checker checker;
      typedef std::vector<std::string> cfg_info;
      typedef struct {} component;  // ���ߡ�
      static bool const is_itronx = false;

      explicit factory( std::string const& kernel );
      virtual ~factory();
      std::vector<std::string> const* get_object_definition_info() const;
      cfg1_out::cfg1_def_table const* get_cfg1_def_table() const;
      std::auto_ptr< cfg1_out > create_cfg1_out( std::string const& filename ) const
      {
        return do_create_cfg1_out( filename );
      }
      std::auto_ptr< checker > create_checker() const
      {
        return do_create_checker();
      }
      std::auto_ptr< macro_processor > create_macro_processor( cfg1_out const& cfg1out, cfg1_out::cfg_obj_map const& obj_def_map ) const
      {
        return do_create_macro_processor( cfg1out, obj_def_map );
      }
      std::auto_ptr< macro_processor > create_macro_processor( cfg1_out const& cfg1out, std::vector< object_definition* > const& obj_array ) const
      {
        return do_create_macro_processor( cfg1out, obj_array );
      }
      std::auto_ptr< macro_processor > create_macro_processor( cfg1_out const& cfg1out, std::auto_ptr< component >& cmponent_ptr ) const
      {
        error( _( "with-software-components is not supported." ) );
        return std::auto_ptr< macro_processor >();
      }
      void swap( factory& other ) { do_swap( other ); }

      cfg_info const& get_cfg_info() const
      {
        return *get_object_definition_info();
      }
    protected:
      virtual void do_swap( factory& other );
      virtual std::auto_ptr< macro_processor > do_create_macro_processor( cfg1_out const& cfg1out, cfg1_out::cfg_obj_map const& obj_def_map ) const;
      virtual std::auto_ptr< macro_processor > do_create_macro_processor( cfg1_out const& cfg1out, std::vector< object_definition* > const& obj_array ) const;
    private:
      virtual std::auto_ptr< cfg1_out > do_create_cfg1_out( std::string const& filename ) const;
      virtual std::auto_ptr< checker > do_create_checker() const;

      std::string kernel_;
    };

  }
}

#endif  // ! TOPPERS_OIL_FACTORY_HPP_